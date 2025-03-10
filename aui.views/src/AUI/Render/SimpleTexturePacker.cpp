﻿/**
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

#include "SimpleTexturePacker.h"


Util::SimpleTexturePacker::SimpleTexturePacker() {

}

Util::SimpleTexturePacker::~SimpleTexturePacker() {

}

void Util::SimpleTexturePacker::onResize(AImage& data, Util::dim side) {
	if (mImage) {
		mImage = AImage::resize(*mImage, static_cast<uint16_t>(side), static_cast<uint16_t>(side));
	}
	else {
		AVector<uint8_t> c;
		c.resize(static_cast<size_t>(side * side * data.getBytesPerPixel()));
		mImage = AImage(c, side, side, data.getFormat());
	}
}

void Util::SimpleTexturePacker::onInsert(AImage& data, const Util::dim& x, const Util::dim& y) {
	AImage::copy(data, *mImage, x, y);
}

glm::vec4 Util::SimpleTexturePacker::insert(AImage& data) {
	return TexturePacker::insert(data, (dim)data.getWidth(), (dim)data.getHeight());
}

