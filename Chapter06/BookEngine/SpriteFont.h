/*
    Copyright (c) 2016 Michael "Mickey" MacDonald.

    Permission is hereby granted, free of charge, to any person obtaining
    a copy of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation
    the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom the Software
    is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
    IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
    CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
    TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
    OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#pragma once

#ifndef SpriteFont_h__
#define SpriteFont_h__

#include <SDL/SDL_ttf.h>
#include <glm/glm.hpp>
#include <map>
#include <vector>

#include "Vertex2D.h"

namespace BookEngine {

    struct GLTexture;
    class SpriteBatch;

    struct CharGlyph {
    public:
        char character;
        glm::vec4 uvRect;
        glm::vec2 size;
    };

#define FIRST_PRINTABLE_CHAR ((char)32)
#define LAST_PRINTABLE_CHAR ((char)126)

    /// For text justification
    enum class Justification {
        LEFT, MIDDLE, RIGHT
    };

    class SpriteFont {
    public:
        SpriteFont(const char* font, int size, char cs, char ce);
        SpriteFont(const char* font, int size) :
            SpriteFont(font, size, FIRST_PRINTABLE_CHAR, LAST_PRINTABLE_CHAR) {
        }
        /// Destroys the font resources
        void dispose();

        int getFontHeight() const {
            return _fontHeight;
        }

        /// Measures the dimensions of the text
        glm::vec2 measure(const char* s);

        /// Draws using a spritebatch
        void Draw(SpriteBatch& batch, const char* s, glm::vec2 position, glm::vec2 scaling, 
                  float depth, ColorRGBA8 tint, Justification just = Justification::LEFT);
    private:
        static std::vector<int>* createRows(glm::ivec4* rects, int rectsLength, int r, int padding, int& w);

        int _regStart, _regLength;
        CharGlyph* m_glyphs;
        int _fontHeight;

        unsigned int _texID;
    };

}

#endif // SpriteFont_h__