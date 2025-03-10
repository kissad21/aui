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

#include <AUI/api.h>

class AView;
class AAssHelper;

namespace ass::decl {
    enum class DeclarationSlot {
        NONE,

        TRANSFORM_SCALE,
        TRANSFORM_OFFSET,

        SHADOW,
        RENDER_OVERFLOW,
        IMAGE_RENDERING,
        BACKGROUND_SOLID,
        BACKGROUND_IMAGE,
        BORDER,
        TEXT_SHADOW,

        COUNT,
    };

    struct API_AUI_VIEWS IDeclarationBase {
    public:
        virtual void applyFor(AView* view) {};
        virtual void renderFor(AView* view) {};
        virtual bool isNone() { return false; }
        [[nodiscard]] virtual DeclarationSlot getDeclarationSlot() const {
            return DeclarationSlot::NONE;
        }
    };
    template<typename DeclarationStruct>
    struct Declaration: IDeclarationBase {
    protected:
        DeclarationStruct mDeclarationStruct;
    public:
        Declaration(const DeclarationStruct& data): mDeclarationStruct(data) {}
    };
}

#include "AUI/View/AView.h"
#include <AUI/ASS/unset.h>