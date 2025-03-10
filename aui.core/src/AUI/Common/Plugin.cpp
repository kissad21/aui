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

#include <AUI/IO/APath.h>
#include "AUI/Common/Plugin.h"

#include "AString.h"
#include <AUI/Traits/strings.h>
#include "ASet.h"
#include "AUI/Platform/AProgramModule.h"
#include "Plugin.h"

void aui::importPlugin(const AString& name)
{
	AString filename = APath(name).filename();
	
	static ASet<AString> importedPlugins;
	if (!importedPlugins.contains(filename))
	{
		importedPlugins << filename;
		aui::importPluginPath("aui." + filename.lowercase());
	}
	
}

void aui::importPluginPath(const APath& path) {
	if (auto module = AProgramModule::load(path)) {
        if (auto proc = module->getProcAddress<void()>("aui_plugin_init")) {
            proc();
            return;
        }
        throw AException("Module {} does not contain plugin entry point"_format(path));
    }
}
