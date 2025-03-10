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
// Created by alex2 on 31.08.2020.
//

#include <gtest/gtest.h>
#include <AUI/Traits/algorithms.h>
#include <AUI/Common/AVector.h>

void performSearchOn(const AVector<int>::const_iterator& begin, const AVector<int>::const_iterator& end) {
    auto theoreticalComplexity = std::log2(end - begin) + 1; // for complexity check

    // perform search of the every value in the range
    for (auto it = begin; it != end; ++it) {
        auto searchingFor = *it;
        size_t calledTimes = 0; // for complexity check

        auto resultToCheck = aui::binary_search(begin, end, [&](const AVector<int>::const_iterator& it) {
            ++calledTimes;
            if (*it == searchingFor) {
                return 0;
            }
            if (*it > searchingFor) return -1;
            return 1;
        });

        // perform result check
        ASSERT_EQ(*resultToCheck, searchingFor);
        if (*resultToCheck != searchingFor) {
            ADD_FAILURE() << "Check failed!";
        }

        // check we fit into the log2(size) complexity
        ASSERT_LE(calledTimes, theoreticalComplexity);
    }

    // also check if we search for *unexisting* value the algorithm results end arg
    ASSERT_TRUE((aui::binary_search(begin, end, [](const AVector<int>::const_iterator& it) { return -1; }) == end)) << "binary_search found unexisting element (wtf?)";
    ASSERT_TRUE((aui::binary_search(begin, end, [](const AVector<int>::const_iterator& it) { return  1; }) == end)) << "binary_search found unexisting element (wtf?)";
}

void performSearchOn(const AVector<int>& v) {
    performSearchOn(v.begin(),  v.end());
}

TEST(Algorithms, BinarySearch1) {
    AVector<int> dataset = { 1, 2, 7, 9, 11, 21, 33, 64, 96, 102, 299, 412414, 58989129, 58989131, 58989153 };

    // search all variants starting with empty range and ending with full vector
    for (auto it = dataset.begin(); ; ++it) {
        performSearchOn(dataset.begin(), it);

        // detect the end of the vector
        if (it == dataset.end()) {
            break;
        }
    }

    // try to confuse binary search algorithm
    dataset[0] = 228;
    ASSERT_TRUE(aui::binary_search(dataset.begin(), dataset.end(), [](const AVector<int>::const_iterator& it) { return -1; }) == dataset.end()) << "binary_search found unexisting element (wtf?)";
}
