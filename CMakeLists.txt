# ======================================================================================================================
# Copyright (c) 2021 Alex2772
#
# Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
# documentation files (the "Software"), to deal in the Software without restriction, including without limitation the
# rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
# permit persons to whom the Software is furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all copies or substantial portions of the
# Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
# WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
# COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
# OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#
# Original code located at https://github.com/aui-framework/aui
# ======================================================================================================================

cmake_minimum_required(VERSION 3.16)

project(AUI)

if (NOT AUI_ROOT)
    set(AUI_ROOT ${CMAKE_CURRENT_SOURCE_DIR} CACHE PATH "AUI src location")
endif()


if(AUI_BOOT_COMPONENTS)
    # adding required dependencies to the components
    list(APPEND AUI_BOOT_COMPONENTS core)
    if (views IN_LIST AUI_BOOT_COMPONENTS)
        list(APPEND AUI_BOOT_COMPONENTS xml)
        if (NOT CMAKE_CROSSCOMPILING)
            list(APPEND AUI_BOOT_COMPONENTS crypt toolbox)
        endif()
    elseif (toolbox IN_LIST AUI_BOOT_COMPONENTS AND NOT CMAKE_CROSSCOMPILING)
        list(APPEND AUI_BOOT_COMPONENTS crypt)
    endif()
endif()
list(REMOVE_DUPLICATES AUI_BOOT_COMPONENTS)

include(aui.boot.cmake)

# use msvc dll
cmake_policy(SET CMP0091 NEW)
include(cmake/aui.build.cmake)

set(AUI_BUILD_EXAMPLES false CACHE BOOL "Build AUI examples")

function(define_aui_component AUI_COMPONENT_NAME)
    if (AUI_BOOT_COMPONENTS)
        if (NOT ${AUI_COMPONENT_NAME} IN_LIST AUI_BOOT_COMPONENTS)
            return()
        endif()
        message(STATUS "[+] ${AUI_COMPONENT_NAME}")
    endif()
    add_subdirectory("aui.${AUI_COMPONENT_NAME}")
endfunction()


define_aui_component(core)
if (NOT ANDROID AND NOT IOS)
    define_aui_component(toolbox)
    define_aui_component(data)
    define_aui_component(mysql)
    define_aui_component(sqlite)
    define_aui_component(network)
endif()
define_aui_component(crypt)
define_aui_component(curl)
define_aui_component(views)
define_aui_component(xml)
define_aui_component(json)

if (NOT ANDROID AND NOT IOS)
    define_aui_component(uitests)
endif()

if(NOT ANDROID AND AUI_BUILD_EXAMPLES)
    add_subdirectory(examples/AUI.Example.Views)
    #add_subdirectory(examples/AUI.Example.Fractal)
    #add_subdirectory(examples/AUI.Example.Minesweeper)
endif()

if (AUI_BOOT)
    # cleanup tests data when using aui thru aui.boot with add_subdirectory
    set_property(GLOBAL PROPERTY TESTS_INCLUDE_DIRS "")
    set_property(GLOBAL PROPERTY TESTS_SRCS "")
    set_property(GLOBAL PROPERTY TESTS_DEPS "")
else()
    aui_enable_tests()
endif()

#install(EXPORT AUI DESTINATION "cmake/")
set(AUI_ALL_COMPONENTS core
        crypt
        curl
        data
        image
        json
        mysql
        network
        sqlite
        views
        xml
        uitests
        toolbox
        )


if(NOT AUI_BOOT_COMPONENTS)
    # adding all components
    set(AUI_BOOT_COMPONENTS ${AUI_ALL_COMPONENTS})

    # remove unexisting modules from module list (i.e. toolbox)
    foreach(_module ${AUI_BOOT_COMPONENTS})
        if (NOT TARGET "aui.${_module}")
            list(REMOVE_ITEM AUI_BOOT_COMPONENTS ${_module})
        endif()
    endforeach()
else()
    # remove executables from module list (i.e. toolbox)
    foreach(_module ${AUI_BOOT_COMPONENTS})
        get_target_property(_type "aui.${_module}" TYPE)
        if (_type STREQUAL "EXECUTABLE")
            list(REMOVE_ITEM AUI_BOOT_COMPONENTS ${_module})
        endif()
    endforeach()
endif()

set(AUI_CONFIG_VARS "set(AUI_ALL_COMPONENTS ${AUI_ALL_COMPONENTS})\nset(AUI_AVAILABLE_COMPONENTS ${AUI_BOOT_COMPONENTS})\n")
get_property(AUI_BOOT_ROOT_ENTRIES GLOBAL PROPERTY AUI_BOOT_ROOT_ENTRIES)
# forward AUI's dependencies
set(AUI_CONFIG_VARS "${AUI_CONFIG_VARS}\nset(AUI_BOOT_ROOT_ENTRIES ${AUI_BOOT_ROOT_ENTRIES})\n")
foreach(_module ${AUI_BOOT_COMPONENTS})
    set(_target "aui.${_module}")
    get_target_property(_compile_definitions ${_target} INTERFACE_COMPILE_DEFINITIONS)
    get_target_property(_link_libs ${_target} INTERFACE_LINK_LIBRARIES)
    get_target_property(_wholearchive ${_target} INTERFACE_AUI_WHOLEARCHIVE)
    unset(_link_libs2)
    foreach(_lib ${_link_libs})
        # skip NOTFOUNDs
        if (_lib)
            string(REPLACE "aui." "aui::" _lib ${_lib})
            list(APPEND _link_libs2 ${_lib})
        endif()
    endforeach()
    set(AUI_CONFIG_VARS "${AUI_CONFIG_VARS}\

set(AUI_COMPONENT_${_module}_COMPILE_DEFINITIONS ${_compile_definitions})
set(AUI_COMPONENT_${_module}_LINK_LIBS ${_link_libs2})
")
    if (_wholearchive)
        set(AUI_CONFIG_VARS "${AUI_CONFIG_VARS}\
set(AUI_COMPONENT_${_module}_WHOLEARCHIVE ON)
")
    endif()
endforeach()


# test aui.boot on ci/cd
if (DEFINED ENV{GITHUB_SHA})
    add_subdirectory(test/aui.boot)
endif()

configure_file(cmake/aui-config.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/aui-config.cmake @ONLY)
#install(EXPORT AUI DESTINATION cmake/)
install(
        FILES
        ${CMAKE_CURRENT_BINARY_DIR}/aui-config.cmake
        DESTINATION ".")

install(
        FILES
        cmake/aui.build.cmake
        DESTINATION "cmake/")
