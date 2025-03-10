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
// Created by alex2 on 06.12.2020.
//

#include <AUI/Util/UIBuildingHelpers.h>
#include <AUI/ASS/ASS.h>
#include <AUI/View/AButton.h>
#include <AUI/View/ALabel.h>
#include <AUI/View/AAbstractTextField.h>
#include "AScrollbar.h"
#include "ASpacer.h"


AScrollbar::AScrollbar(LayoutDirection direction) :
    mScrollButtonTimer(_new<ATimer>(80)),
    mDirection(direction) {

    mForwardButton = _new<AScrollbarButton>();
    mBackwardButton = _new<AScrollbarButton>();

    connect(mForwardButton->mousePressed, me::scrollForward);
    connect(mBackwardButton->mousePressed, me::scrollBackward);

    switch (direction) {
        case LayoutDirection::HORIZONTAL:
            setLayout(_new<AHorizontalLayout>());

            mForwardButton << ".scrollbar_right";
            mBackwardButton << ".scrollbar_left";
            break;
        case LayoutDirection::VERTICAL:
            setLayout(_new<AVerticalLayout>());
            mForwardButton << ".scrollbar_down";
            mBackwardButton << ".scrollbar_up";
            break;
    }
    mHandle = _new<AScrollbarHandle>();

    addView(mBackwardButton);
    addView(mOffsetSpacer = _new<AScrollbarOffsetSpacer>() let { it->setMinimumSize({0, 0}); });
    addView(mHandle);
    addView(_new<ASpacer>() let { it->setMinimumSize({0, 0}); });
    addView(mForwardButton);

    ui_thread {
        setScroll(0);
    };
}

void AScrollbar::setOffset(size_t o) {
    switch (mDirection) {
        case LayoutDirection::HORIZONTAL:
            mOffsetSpacer->setSize(o, 0);
            break;
        case LayoutDirection::VERTICAL:
            mOffsetSpacer->setSize(0, o);
            break;
    }
}

void AScrollbar::setScrollDimensions(size_t viewportSize, size_t fullSize) {
    mViewportSize = viewportSize;
    mFullSize = fullSize;

    updateScrollHandleSize();
}

void AScrollbar::updateScrollHandleSize() {
    float scrollbarSpace = 0;

    switch (mDirection) {
        case LayoutDirection::HORIZONTAL:
            scrollbarSpace = getWidth() - (mBackwardButton->getTotalOccupiedWidth() + mForwardButton->getTotalOccupiedWidth());
            break;
        case LayoutDirection::VERTICAL:
            scrollbarSpace = getHeight() - (mBackwardButton->getTotalOccupiedHeight() + mForwardButton->getTotalOccupiedHeight());
            break;
    }

    size_t o = glm::max(float(15_dp), scrollbarSpace * mViewportSize / mFullSize);

    if (o < scrollbarSpace) {
        setEnabled();
        mHandle->setVisibility(Visibility::VISIBLE);
        switch (mDirection) {
            case LayoutDirection::HORIZONTAL:
                mHandle->setFixedSize({o, mHandle->getHeight()});
                break;
            case LayoutDirection::VERTICAL:
                mHandle->setFixedSize({mHandle->getWidth(), o});
                break;
        }
    } else {
        mHandle->setVisibility(Visibility::GONE);
        emit scrolled(mCurrentScroll = 0);
        setDisabled();
    }

    updateScrollHandleOffset(getMaxScroll());
}

void AScrollbar::setScroll(int scroll) {
    updateScrollHandleSize();
    int max = getMaxScroll();
    auto newScroll = glm::clamp(scroll, 0, max);
    if (mCurrentScroll != newScroll) {
        mCurrentScroll = newScroll;

        updateScrollHandleOffset(max);


        emit scrolled(mCurrentScroll);
    }
}

void AScrollbar::updateScrollHandleOffset(int max) {
    float availableSpace = getAvailableSpaceForSpacer();


    int handlePos = float(mCurrentScroll) / max * availableSpace;

    switch (mDirection) {
        case LayoutDirection::HORIZONTAL:
            mOffsetSpacer->setFixedSize(glm::ivec2{handlePos, 0});
            break;
        case LayoutDirection::VERTICAL:
            mOffsetSpacer->setFixedSize(glm::ivec2{0, handlePos});
            break;
    }
    updateLayout();
    redraw();
}

void AScrollbar::onMouseWheel(const glm::ivec2& pos, const glm::ivec2& delta) {
    AViewContainer::onMouseWheel(pos, delta);
    // scroll 3 lines of text
    setScroll(mCurrentScroll + delta.y * 11_pt * 3 / 120);
}

void AScrollbar::scrollForward() {
    setScroll(mCurrentScroll + 10);
    mScrollButtonTimer->start();
    connect(mScrollButtonTimer->fired, this, [&] {
        if (AInput::isKeyDown(AInput::LButton)) {
            setScroll(mCurrentScroll + 10);
        } else {
            mScrollButtonTimer->stop();
            AObject::disconnect();
        }
    });
}

void AScrollbar::scrollBackward() {
    setScroll(mCurrentScroll - 10);
    mScrollButtonTimer->start();
    connect(mScrollButtonTimer->fired, this, [&] {
        if (AInput::isKeyDown(AInput::LButton)) {
            setScroll(mCurrentScroll - 10);
        } else {
            mScrollButtonTimer->stop();
            AObject::disconnect();
        }
    });
}

void AScrollbar::onMousePressed(glm::ivec2 pos, AInput::Key button) {
    AViewContainer::onMousePressed(pos, button);
}

void AScrollbar::handleScrollbar(int s) {
    setScroll(mCurrentScroll + s * getMaxScroll() / getAvailableSpaceForSpacer());
}

int AScrollbar::getMaxScroll() {
    return mFullSize - mViewportSize + 15_dp;
}

float AScrollbar::getAvailableSpaceForSpacer() {

    switch (mDirection) {
        case LayoutDirection::HORIZONTAL:
            return getWidth() - (mBackwardButton->getTotalOccupiedWidth() + mForwardButton->getTotalOccupiedWidth() + mHandle->getTotalOccupiedWidth());

        case LayoutDirection::VERTICAL:
            return getHeight() - (mBackwardButton->getTotalOccupiedHeight() + mForwardButton->getTotalOccupiedHeight() + mHandle->getTotalOccupiedHeight());

    }
    return 0;
}

void AScrollbarHandle::onMouseMove(glm::ivec2 pos) {
    AView::onMouseMove(pos);
    if (mDragging) {
        dynamic_cast<AScrollbar*>(getParent())->handleScrollbar(pos.y - mScrollOffset);
    }
}

void AScrollbarHandle::onMousePressed(glm::ivec2 pos, AInput::Key button) {
    AView::onMousePressed(pos, button);
    mScrollOffset = pos.y;
    mDragging = true;
}

void AScrollbarHandle::onMouseReleased(glm::ivec2 pos, AInput::Key button) {
    AView::onMouseReleased(pos, button);
    mDragging = false;
}
