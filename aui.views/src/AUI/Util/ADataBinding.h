/**
 * =====================================================================================================================
 * Copyright (c) 2021 Alex2772
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 
 * Original code located at https://github.com/aui-framework/aui
 * =====================================================================================================================
 */

#pragma once

#include <type_traits>
#include <AUI/Common/SharedPtr.h>
#include <AUI/Common/ASignal.h>


template<typename View, typename FieldType>
struct ADataBindingDefault {
public:
    /**
     * Called then view linked with field.
     * @param view
     */
    static void setup(const _<View>& view) {}

    static ASignal<FieldType>(View::*getGetter()) {
        return nullptr;
    }
    static void(View::*getSetter())(const FieldType& v) {
        return nullptr;
    }
};

template <typename Model>
class ADataBinding;

template<typename Model, typename Klass, typename ModelField, typename Getter, typename Setter>
class ADataBindingLinker {
private:
    ADataBinding<Model>* mBinder;
    void(Klass::*mSetterFunc)(Setter);
    std::decay_t<ModelField>(Model::*mField);
    ASignal<std::decay_t<Getter>>(Klass::*mGetter);

public:
    ADataBindingLinker(ADataBinding<Model>* binder, ASignal<std::decay_t<Getter>>(Klass::*getter), void (Klass::*setterFunc)(Setter),
                       std::decay_t<ModelField>(Model::*field)):
            mBinder(binder), mGetter(getter), mSetterFunc(setterFunc), mField(field) {}

    ADataBinding<Model>* getBinder() const {
        return mBinder;
    }

    auto getSetterFunc() const {
        return mSetterFunc;
    }

    auto getField() const {
        return mField;
    }

    auto getGetter() const {
        return mGetter;
    }
};
template<typename Model, typename Data>
class ADataBindingLinker2 {
private:
    ADataBinding<Model>* mBinder;
    Data(Model::*mField);

public:
    ADataBindingLinker2(ADataBinding<Model>* binder, Data(Model::*field)) : mBinder(binder), mField(field) {}

    ADataBinding<Model>* getBinder() const {
        return mBinder;
    }

    auto getField() const {
        return mField;
    }
};

/**
 * Data binding implementation.
 * <p>
 * If const reference of your model passed, ADataBinding will create and manage its own copy of your model.
 * </p>
 * <p>
 * If pointer of your model passed, ADataBinding will reference to your model and write directly to your model. When
 * ADataBinding is destructed the pointer will not be deleted.
 * </p>
 *
 * <p>Example:</p>
 * <code>
 * _new&lt;ATextField&gt;() && dataBinding(&User::username)
 * </code>
 * <p>This code will bind ATextField with username field in the User model.</p>
 *
 * @tparam Model Your model type.
 */
template <typename Model>
class ADataBinding: public AObject {
private:
    using Applier = std::function<void(const Model& model, unsigned)>;
    ADeque<Applier> mLinkAppliers;

    Model* mModel = nullptr;
    bool mOwning = false;

    void* mExcept = nullptr;

public:

    ADataBinding() = default;
    explicit ADataBinding(const Model& m)
    {
        setModel(m);
    }
    explicit ADataBinding(Model* m)
    {
        setModel(m);
    }

    virtual ~ADataBinding() {
        if (mOwning) {
            delete mModel;
        }
    }
    template<typename View, typename ModelField, typename SetterArg>
    ADataBindingLinker<Model, View, std::decay_t<ModelField>, std::decay_t<ModelField>, SetterArg> operator()(ModelField(Model::*field), void(View::*setterFunc)(SetterArg)) {
        assert(setterFunc != nullptr);
        return ADataBindingLinker<Model, View, std::decay_t<ModelField>, std::decay_t<ModelField>, SetterArg>(this, nullptr, setterFunc, field);
    }
    template<typename View, typename ModelField, typename GetterRV, typename SetterArg>
    ADataBindingLinker<Model, View, std::decay_t<ModelField>, GetterRV, SetterArg> operator()(ModelField(Model::*field),
                                                                                              ASignal<GetterRV>(View::*getter),
                                                                                              void(View::*setterFunc)(SetterArg) = nullptr) {
        assert((getter != nullptr || setterFunc != nullptr) &&
               "implement ADataBindingDefault for your view in order to use default binding");

        return ADataBindingLinker<Model, View, std::decay_t<ModelField>, GetterRV, SetterArg>(this, getter, setterFunc, field);
    }

    template<typename Data>
    ADataBindingLinker2<Model, Data> operator()(Data(Model::*field)) {
        return ADataBindingLinker2<Model, Data>(this, field);
    }
    const Model& getModel() const {
        return *mModel;
    }
    Model& getEditableModel() {
        return *mModel;
    }
    void setModel(const Model& model) {
        if (mOwning) {
            delete mModel;
        }
        mOwning = true;
        mModel = new Model(model);
        update();
    }

    void setModel(Model* model) {
        if (mOwning) {
            delete mModel;
        }
        mOwning = false;
        mModel = model;
        update();
    }

    const void* getExclusion() const {
        return mExcept;
    }

    void update(void* except = nullptr, unsigned field = -1) {
        mExcept = except;
        for (auto& applier : mLinkAppliers) {
            applier(*mModel, field);
        }
        emit modelChanged;
    }

    void addApplier(const Applier& applier) {
        mLinkAppliers << applier;
        if (mModel) {
            mLinkAppliers.last()(*mModel, -1);
        }
    }

signals:
    /**
     * \brief Data in the model has changed.
     */
    emits<> modelChanged;
};

template<typename Klass1, typename Klass2, typename Model, typename ModelField, typename GetterRV, typename SetterArg>
_<Klass1> operator&&(const _<Klass1>& object, const ADataBindingLinker<Model, Klass2, ModelField, GetterRV, SetterArg>& linker) {
    union converter {
        unsigned i;
        decltype(linker.getField()) p;
    };
    if (linker.getGetter()) {
        AObject::connect(object.get()->*(linker.getGetter()), linker.getBinder(), [object, linker](const GetterRV& data) {
            assert(("please setModel for ADataBinding" && &linker.getBinder()->getEditableModel()));
            object->setSignalsEnabled(false);
            linker.getBinder()->getEditableModel().*(linker.getField()) = data;
            converter c;
            c.p = linker.getField();
            linker.getBinder()->update(object.get(), c.i);
            object->setSignalsEnabled(true);
        });
    }

    if (linker.getSetterFunc()) {
        linker.getBinder()->addApplier([object, linker](const Model& model, unsigned field) {
            converter c;
            c.p = linker.getField();
            if (c.i == field || field == -1) {
                if (object.get() != linker.getBinder()->getExclusion()) {
                    (object.get()->*(linker.getSetterFunc()))(model.*(linker.getField()));
                }
            }
        });
    }

    return object;
}

template<typename View, typename Model, typename Data>
_<View> operator&&(const _<View>& object, const ADataBindingLinker2<Model, Data>& linker) {
    ADataBindingDefault<View, Data>::setup(object);
    object && (*linker.getBinder())(linker.getField(),
                                    ADataBindingDefault<View, Data>::getGetter(),
                                    ADataBindingDefault<View, Data>::getSetter());
    return object;
}
