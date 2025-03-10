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

#include <AUI/UITest.h>
#include <AUI/Util/UIBuildingHelpers.h>
#include <AUI/View/ATextField.h>

using namespace ass;

std::ostream& operator<<(std::ostream& o, const ACursorSelectable::Selection& e) noexcept{
    o << "Selection";
    if (e.end == -1) {
        o << "{" << e.begin << "}";
    } else {
        o << "(" << e.begin << ";" << e.end << "]";
    }
    return o;
}


class UITextField: public testing::UITest {
public:
protected:
    void SetUp() override {
        UITest::SetUp();

        class TestWindow: public AWindow {
        public:
            TestWindow() {
                setContents(Centered {
                        _new<ATextField>() with_style { FixedSize { 300_dp, {} } } let {
                            it->setText("hello world!");
                        }
                });
                pack();
            }
        };

        _new<TestWindow>()->show();
    }

    void TearDown() override {
        UITest::TearDown();
    }
};

struct SelectionMatchesAssert {
    ACursorSelectable::Selection selection;

    SelectionMatchesAssert(unsigned begin, unsigned end) : selection{begin, end} {}
    SelectionMatchesAssert(unsigned s) : selection{s, static_cast<unsigned>(-1)} {}

    bool operator()(const _<AView>& view) const {
        if (auto t = _cast<ACursorSelectable>(view)) {
            EXPECT_EQ(t->getSelection(), selection);
        }
        return true;
    }
};

using selectionMatches = SelectionMatchesAssert;

/**
 * Checks that then clicking at the left border of the text field, cursor jumps to the first symbol.
 */
TEST_F(UITextField, CursorAppearsAtTheStart) {
    By::type<ATextField>().perform(click({0_dp, 0_dp})) // click at the left border
                          .check(selectionMatches(0));
}

/**
 * Checks that then clicking at the right border of the text field, cursor jumps to the last symbol.
 */
TEST_F(UITextField, CursorAppearsAtTheEnd) {
    By::type<ATextField>().perform(click({299_dp, 0_dp})) // click at the right border
                          .check(selectionMatches(12));
}

/**
 * Checks that when doubleclicking a whole word is selected
 */
TEST_F(UITextField, DoubleClickWordSelection1) {
    By::type<ATextField>().perform(doubleClick({20_dp, 0_dp})) // click at the first word
                          .check(selectionMatches(0, 5));
}

/**
 * Checks that when doubleclicking a whole word is selected
 */
TEST_F(UITextField, DoubleClickWordSelection2) {
    By::type<ATextField>().perform(doubleClick({54_dp, 0_dp})) // click at the second word
                          .check(selectionMatches(6, 12));
}

/**
 * Checks cursor position when clicking between 'l' and 'o'.
 */
TEST_F(UITextField, CursorClickPos1) {
    By::type<ATextField>().perform(click({23_dp, 0_dp})) // hardcoded mouse position
            .check(selectionMatches(4));
}
/**
 * Checks cursor position when clicking between 'o' and 'r'.
 */
TEST_F(UITextField, CursorClickPos2) {
    By::type<ATextField>().perform(click({51_dp, 0_dp})) // hardcoded mouse position
            .check(selectionMatches(8));
}
