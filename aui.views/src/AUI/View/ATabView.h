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

//
// Created by alex2 on 24.10.2020.
//

#pragma once

#include "APageView.h"
#include "ALabel.h"

class API_AUI_VIEWS ATabButtonView: public ALabel {
private:
    bool mCurrent = false;

public:
    ATabButtonView() {

    }
    ATabButtonView(const AString& text): ALabel(text) {

    }

    void getCustomCssAttributes(AMap<AString, AVariant>& map) override;
    void setCurrent(bool current);
};

class API_AUI_VIEWS ATabButtonRow: public AViewContainer {
private:
    _<AViewContainer> mContents;

public:
    ATabButtonRow();
    void setCurrent(size_t i, bool current);
    void addTab(const AString& name);

    const _<AViewContainer>& getContents() const {
        return mContents;
    }
};

class API_AUI_VIEWS ATabView: public AViewContainer {
private:
    _<APageView> mPageView;
    _<ATabButtonRow> mRow;

public:
    ATabView();

    void addTab(const _<AView>& view, const AString& name = {});
    void setTabId(unsigned tabId);

};


