//
// Created by Alex2772 on 1/24/2022.
//

#include "AGLEmbedAuiWrap.h"
#include "AUI/GL/State.h"
#include "AUI/GL/OpenGLRenderer.h"

void AGLEmbedAuiWrap::render() {
    windowMakeCurrent();
    AThread::current()->processMessages();

    glEnable(GL_STENCIL_TEST);
    glDisable(GL_DEPTH_TEST);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
    GL::State::activeTexture(0);
    GL::State::useProgram(0);
    GL::State::bindVertexArray(0);
    GL::State::bindTexture(GL_TEXTURE_2D, 0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Render::setColorForced(1.f);
    Render::setTransformForced(glm::ortho(0.f,
                                          static_cast<float>(mSize.x),
                                          static_cast<float>(mSize.y),
                                          0.f));
    windowRender();

    resetGLState();
}

void AGLEmbedAuiWrap::resetGLState() {
    glBindTexture(GL_TEXTURE_2D, 0);
    if (glBindVertexArray) {
        glBindVertexArray(0);
    }
}

AGLEmbedAuiWrap::AGLEmbedAuiWrap() {
#if !(AUI_PLATFORM_ANDROID || AUI_PLATFORM_IOS)
    glewExperimental = true;
    auto r = glewInit();
    assert(r == 0);
#endif
    Render::setRenderer(std::make_unique<OpenGLRenderer>());
}
