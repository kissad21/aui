#pragma once


#include <AUI/Render/Render.h>
#include <AUI/Util/AWordWrappingEngine.h>
#include "AUI/Font/AFontStyle.h"

/**
 * Helper class for rendering and processing multiline texts.
 */
class AMultilineTextRender {
private:
friend class TextEntry;
    AWordWrappingEngine mEngine;
    AFontStyle mFontStyle;

    class TextEntry: public AWordWrappingEngine::Entry {
    private:
        AString::const_iterator mBegin;
        AString::const_iterator mEnd;
        AMultilineTextRender& mTextRender;
        glm::vec2 mPosition;

    public:
        TextEntry(const AString::const_iterator& begin, const AString::const_iterator& end, AMultilineTextRender& textRender)
                : mBegin(begin), mEnd(end), mTextRender(textRender) {}

        AString getText() const {
            return {mBegin, mEnd};
        }
        glm::ivec2 getSize() override;
        void setPosition(const glm::ivec2& position) override;
        Float getFloat() const override;

        ~TextEntry() override = default;

        const glm::vec2& getPosition() const {
            return mPosition;
        }
    };


public:
    Render::PrerenderedString updateText(const AString& text, const glm::ivec2& size);

    void setFontStyle(const AFontStyle& fontStyle) {
        mFontStyle = fontStyle;
    }
};


