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

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

#include "Vertex2D.h"

namespace BookEngine{

// Determines how we should sort the glyphs
enum class GlyphSortType {
    NONE,
    FRONT_TO_BACK,
    BACK_TO_FRONT,
    TEXTURE
};

// A glyph is a single quad. These are added via SpriteBatch::Draw
class Glyph {
public:
    Glyph() {};
    Glyph(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint Texture, float Depth, const ColorRGBA8& color);
    Glyph(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint Texture, float Depth, const ColorRGBA8& color, float angle);

    GLuint texture;
    float depth;
    
    Vertex2D topLeft;
    Vertex2D bottomLeft;
    Vertex2D topRight;
    Vertex2D bottomRight;
private:
    // Rotates a point about (0,0) by angle
    glm::vec2 RotatePoint(const glm::vec2& pos, float angle);
};

// Each render batch is used for a single draw call
class BatchRenderr {
public:
    BatchRenderr(GLuint Offset, GLuint NumVertices, GLuint Texture) : offset(Offset),
        numVertices(NumVertices), texture(Texture) {
    }
    GLuint offset;
    GLuint numVertices;
    GLuint texture;
};

// The SpriteBatch class is a more efficient way of drawing sprites
class SpriteBatch
{
public:
    SpriteBatch();
    ~SpriteBatch();

    // Initializes the spritebatch
    void Init();

    // Begins the spritebatch
    void Begin(GlyphSortType sortType = GlyphSortType::TEXTURE);

    // Ends the spritebatch
    void End();

    // Adds a glyph to the spritebatch
    void Draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color);
    // Adds a glyph to the spritebatch with rotation
    void Draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color, float angle);
    // Adds a glyph to the spritebatch with rotation
    void Draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color, const glm::vec2& dir);

    // Renders the entire SpriteBatch to the screen
    void BatchRender();
	
private:
    // Creates all the needed RenderBatches
    void CreateRenderBatches();

    // Generates our VAO and VBO
    void CreateVertexArray();

    // Sorts glyphs according to _sortType
    void SortGlyphs();

    // Comparators used by sortGlyphs()
    static bool CompareFrontToBack(Glyph* a, Glyph* b);
    static bool CompareBackToFront(Glyph* a, Glyph* b);
    static bool CompareTexture(Glyph* a, Glyph* b);

    GLuint m_vbo;
    GLuint m_vao;

    GlyphSortType m_sortType;
	
    std::vector<Glyph*> m_glyphPointers; ///< This is for sorting
    std::vector<Glyph> m_glyphs; ///< These are the actual glyphs
	std::vector<BatchRenderr> m_renderBatches;
};

}