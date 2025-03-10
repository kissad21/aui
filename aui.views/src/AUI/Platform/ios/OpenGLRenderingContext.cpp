//
// Created by Alex2772 on 12/7/2021.
//

#include <AUI/GL/gl.h>
#include <AUI/Platform/OpenGLRenderingContext.h>
#include <AUI/Util/ARandom.h>
#include <AUI/Logging/ALogger.h>
#include <AUI/Platform/AMessageBox.h>
#include <AUI/GL/GLDebug.h>

#include <AUI/GL/OpenGLRenderer.h>
#include <AUI/GL/State.h>
#include "AUI/Traits/arrays.h"

void OpenGLRenderingContext::init(const Init& init) {
    CommonRenderingContext::init(init);
    Render::setRenderer(std::make_unique<OpenGLRenderer>());
}

void OpenGLRenderingContext::destroyNativeWindow(ABaseWindow& window) {
    CommonRenderingContext::destroyNativeWindow(window);
}

void OpenGLRenderingContext::beginPaint(ABaseWindow& window) {
    CommonRenderingContext::beginPaint(window);
    
    GL::State::activeTexture(0);
    GL::State::bindTexture(GL_TEXTURE_2D, 0);
    GL::State::bindVertexArray(0);
    GL::State::useProgram(0);

    glViewport(0, 0, window.getWidth(), window.getHeight());

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // stencil
    glClear(GL_STENCIL_BUFFER_BIT);
    glClearStencil(0);
    glStencilMask(0xff);
    glDisable(GL_SCISSOR_TEST);
    glClear(GL_STENCIL_BUFFER_BIT);
    glEnable(GL_STENCIL_TEST);
    glStencilMask(0x00);
    glStencilFunc(GL_EQUAL, 0, 0xff);
}

void OpenGLRenderingContext::beginResize(ABaseWindow& window) {
}

void OpenGLRenderingContext::endResize(ABaseWindow& window) {

}

void OpenGLRenderingContext::endPaint(ABaseWindow& window) {
    CommonRenderingContext::endPaint(window);
}

OpenGLRenderingContext::~OpenGLRenderingContext() {
}

AImage OpenGLRenderingContext::makeScreenshot() {
    // stub
    return AImage();
}
