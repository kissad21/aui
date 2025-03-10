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

#include <AUI/Common/AString.h>
#include <AUI/Common/AMap.h>
#include <AUI/Common/AVector.h>
#include "AUI/Common/AVariant.h"
#include "AJsonElement.h"
#include <AUI/IO/IOutputStream.h>

class IJsonElement {
protected:
    AString formatMiscastException(const AString& message);

public:
	virtual ~IJsonElement() = default;

	virtual bool isVariant() = 0;
	virtual bool isObject() = 0;
	virtual bool isArray() = 0;
	virtual bool isNull() {
	    return false;
	}

	[[nodiscard]] virtual AVariant& asVariant() = 0;
	[[nodiscard]] virtual AMap<AString, AJsonElement>& asObject() = 0;
	[[nodiscard]] virtual AVector<AJsonElement>& asArray() = 0;
    virtual void serialize(IOutputStream& os) const = 0;
    virtual void serialize(IOutputStream&& os) const {
        serialize(os);
    }
};