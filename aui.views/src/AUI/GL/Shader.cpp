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

//
// Created by alex2772 on 25.07.2018.
//

#include "Shader.h"
#include "State.h"

#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <AUI/Logging/ALogger.h>


GL::Shader::Shader() {
    for (int32_t& uniform : mUniforms)
    {
        uniform = UniformState::UNINITIALIZED;
    }
	mProgram = glCreateProgram();
}

void GL::Shader::load(const AString& vertex, const AString& fragment, const AVector<AString>& attribs, const AString& version)
{
#if AUI_PLATFORM_ANDROID
	AString prefix = "precision mediump float;"
					 "precision mediump int;"
					 ;
#elif AUI_PLATFORM_APPLE
    AString prefix;
#else
	AString prefix;
	if (version.empty()) {
	    prefix = "#version 120\n";
	} else {
        prefix = "#version " + version + "\n";
    }
#endif
	mVertex = load(prefix + vertex, GL_VERTEX_SHADER);
	mFragment = load(prefix + fragment, GL_FRAGMENT_SHADER);

	unsigned index = 0;
	for (auto& s : attribs)
		bindAttribute(index++, s);
	
	compile();
}

GL::Shader::~Shader() {
	glDetachShader(mProgram, mVertex);
	glDetachShader(mProgram, mFragment);
	glDeleteShader(mVertex);
	glDeleteShader(mFragment);
	glDeleteProgram(mProgram);
}


uint32_t GL::Shader::load(const AString& data, uint32_t type) {
	uint32_t shader = glCreateShader(type);
	std::string code = data.toStdString();
	assert(!code.empty());

#if AUI_PLATFORM_APPLE
    if (type == GL_VERTEX_SHADER) {

        code =
"#define attribute in\n"
"#define varying out\n"
+ code;
    } else {
        code =
"#define varying in\n"
"out vec4 resultColor;\n"
"#define gl_FragColor resultColor\n"
"#define texture2D texture\n"
+ code;
    }
#endif

#if AUI_PLATFORM_IOS
    code = "#version 300 es\n"
            "precision mediump float;\n"
            "precision mediump int;\n"
                     + code;
#elif AUI_PLATFORM_MACOS
    code = "#version 150\n" + code;
#endif

	const char* c = code.c_str();
	glShaderSource(shader, 1, &c, nullptr);
	glCompileShader(shader);
	int st;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &st);

	{
		char buf[8192];
		GLsizei len;
		glGetShaderInfoLog(shader, sizeof(buf), &len, buf);
		if (len) {
			ALogger::warn(buf);
		}
	}
	if (!st) {
		throw AException("Failed to compile shader:\n" + code);
	}

	return shader;
}

void GL::Shader::compile() {
	glAttachShader(mProgram, mVertex);
	glAttachShader(mProgram, mFragment);
	glLinkProgram(mProgram);
}

void GL::Shader::use() const {
	GL::State::useProgram(mProgram);
	currentShader() = const_cast<GL::Shader*>(this);
}

void GL::Shader::bindAttribute(uint32_t index, const AString& name) {
    if (name.empty())
        return;
	glBindAttribLocation(mProgram, index, name.toStdString().c_str());
}


void GL::Shader::set(const GL::Shader::Uniform& uniform, glm::mat4 value) const {
	auto loc = getLocation(uniform);
	if (loc >= 0)
		glUniformMatrix4fv(loc, 1, GL_FALSE, &(value[0][0]));
}

void GL::Shader::set(const GL::Shader::Uniform& uniform, glm::dmat4 value) const {
	auto loc = getLocation(uniform);
	if (loc >= 0) {
#if AUI_PLATFORM_ANDROID || AUI_PLATFORM_IOS
		glm::mat4 fvalue = value;
		glUniformMatrix4fv(loc, 1, GL_FALSE, &(fvalue[0][0]));
#else
		glUniformMatrix4dv(loc, 1, GL_FALSE, &(value[0][0]));
#endif
	}
}

void GL::Shader::set(const GL::Shader::Uniform& uniform, float value) const {
	auto loc = getLocation(uniform);
	if (loc >= 0)
		glUniform1f(loc, value);
}

void GL::Shader::set(const GL::Shader::Uniform& uniform, glm::vec2 value) const {
	auto loc = getLocation(uniform);
	if (loc >= 0)
		glUniform2f(loc, value.x, value.y);
}

void GL::Shader::set(const GL::Shader::Uniform& uniform, glm::vec3 value) const {
	auto loc = getLocation(uniform);
	if (loc >= 0)
		glUniform3f(loc, value.x, value.y, value.z);
}

void GL::Shader::set(const GL::Shader::Uniform& uniform, glm::vec4 value) const {
	auto loc = getLocation(uniform);
	if (loc >= 0)
		glUniform4f(loc, value.x, value.y, value.z, value.a);
}

void GL::Shader::set(const GL::Shader::Uniform& uniform, int value) const {
	auto loc = getLocation(uniform);
	if (loc >= 0)
		glUniform1i(loc, value);
}

void GL::Shader::set(const GL::Shader::Uniform& uniform, double value) const {
    auto loc = getLocation(uniform);
    if (loc >= 0) {
#if AUI_PLATFORM_ANDROID || AUI_PLATFORM_IOS
		glUniform1f(loc, value);
#else
		glUniform1d(loc, value);
#endif
	}
}

int32_t GL::Shader::getLocation(const GL::Shader::Uniform& uniform) const {
    int32_t& location = mUniforms[uniform.getId()];
    if (location == UniformState::UNINITIALIZED) {
        location = glGetUniformLocation(mProgram, uniform.getUniformName());
        //assert(location != -1);
    }
    return location;
}

/*
void GL::Shader::setArray(const GL::Shader::Uniform& uniform, const Vector<float>& value) {
	for (size_t i = 0; i < value.size(); ++i) {
		set(uniform + "[" + AString::number(i) + "]", value[i]);
	}
}*/
unsigned GL::Shader::Uniform::next() {
    static unsigned id = 0;
    return id++;
}
