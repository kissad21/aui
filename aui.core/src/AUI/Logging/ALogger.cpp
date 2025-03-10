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

#include "ALogger.h"
#if AUI_PLATFORM_ANDROID
#include <android/log.h>
#else
#include <ctime>
#endif

ALogger::ALogger()
{

}

ALogger& ALogger::instance()
{
	static ALogger l;
	return l;
}

void ALogger::log(Level level, const AString& str)
{
#if AUI_PLATFORM_ANDROID

    int prio;
    switch (level) {
        case INFO:
            prio = ANDROID_LOG_INFO;
            break;
        case WARN:
            prio = ANDROID_LOG_WARN;
            break;
        case ERR:
            prio = ANDROID_LOG_ERROR;
            break;
        case DEBUG:
            prio = ANDROID_LOG_DEBUG;
            break;
        default:
            assert(0);
    }
    __android_log_print(prio, "AUI", "%ls", str.c_str());
#else
    if (!mLogFile) {
        try {
            mLogFile = _new<AFileOutputStream>("latest.log");
        } catch (const AException& e) {
            //log(WARN, e.getMessage());
        }
    }

    const char* levelName = "UNKNOWN";

    switch (level)
    {
    case INFO:
        levelName = "INFO";
        break;
    case WARN:
        levelName = "WARN";
        break;
    case ERR:
        levelName = "ERR";
        break;
    case DEBUG:
        levelName = "DEBUG";
        break;
    }

    std::time_t t = std::time(nullptr);
    std::tm* tm;
    tm = localtime(&t);
    char timebuf[64];
    std::strftime(timebuf, sizeof(timebuf), "%H:%M:%S", tm);

    std::unique_lock lock(mSync);

    std::cout << '[' << timebuf << "] " << levelName << ": " << str << std::endl;
    if (auto l = mLogFile) {
        *l << '[';
        l->write(timebuf, strlen(timebuf));
        *l << "] ";
        l->write(levelName, strlen(levelName));
        *l << ": " << str << "\n";
    }
#endif
}

void ALogger::setLogFile(const AString& str) {
#if !(AUI_PLATFORM_ANDROID)
    try {
        if (str.empty()) {
            instance().mLogFile = nullptr;
        } else {
            instance().mLogFile = _new<AFileOutputStream>(str);
        }
    } catch (const AException& e) {
        //log(WARN, e.getMessage());
    }
#endif

}

void ALogger::rawWrite(const char *data, size_t length) {
    std::cout.write(data, length);
    if (mLogFile) {
        mLogFile->write(data, length);
    }
}
