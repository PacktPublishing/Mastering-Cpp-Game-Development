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
#include "SpriteBatch.h"

#include <algorithm>

namespace BookEngine {


    Glyph::Glyph(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint Texture, float Depth, const ColorRGBA8& color) :
        texture(Texture),
        depth(Depth) {

        topLeft.color = color;
        topLeft.SetPosition(destRect.x, destRect.y + destRect.w);
        topLeft.SetUV(uvRect.x, uvRect.y + uvRect.w);

        bottomLeft.color = color;
        bottomLeft.SetPosition(destRect.x, destRect.y);
        bottomLeft.SetUV(uvRect.x, uvRect.y);

        bottomRight.color = color;
        bottomRight.SetPosition(destRect.x + destRect.z, destRect.y);
        bottomRight.SetUV(uvRect.x + uvRect.z, uvRect.y);

        topRight.color = color;
        topRight.SetPosition(destRect.x + destRect.z, destRect.y + destRect.w);
        topRight.SetUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);
    }

    Glyph::Glyph(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint Texture, float Depth, const ColorRGBA8& color, float angle) :
        texture(Texture),
        depth(Depth) {

        glm::vec2 halfDims(destRect.z / 2.0f, destRect.w / 2.0f);

        // Get points centered at origin
        glm::vec2 tl(-halfDims.x, halfDims.y);
        glm::vec2 bl(-halfDims.x, -halfDims.y);
        glm::vec2 br(halfDims.x, -halfDims.y);
        glm::vec2 tr(halfDims.x, halfDims.y);

        // Rotate the points
        tl = RotatePoint(tl, angle) + halfDims;
        bl = RotatePoint(bl, angle) + halfDims;
        br = RotatePoint(br, angle) + halfDims;
        tr = RotatePoint(tr, angle) + halfDims;

        topLeft.color = color;
        topLeft.SetPosition(destRect.x + tl.x, destRect.y + tl.y);
        topLeft.SetUV(uvRect.x, uvRect.y + uvRect.w);

        bottomLeft.color = color;
        bottomLeft.SetPosition(destRect.x + bl.x, destRect.y + bl.y);
        bottomLeft.SetUV(uvRect.x, uvRect.y);

        bottomRight.color = color;
        bottomRight.SetPosition(destRect.x + br.x, destRect.y + br.y);
        bottomRight.SetUV(uvRect.x + uvRect.z, uvRect.y);

        topRight.color = color;
        topRight.SetPosition(destRect.x + tr.x, destRect.y + tr.y);
        topRight.SetUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);
    }

    glm::vec2 Glyph::RotatePoint(const glm::vec2& pos, float angle) {
        glm::vec2 newv;
        newv.x = pos.x * cos(angle) - pos.y * sin(angle);
        newv.y = pos.x * sin(angle) + pos.y * cos(angle);
        return newv;
    }

SpriteBatch::SpriteBatch() : m_vbo(0), m_vao(0)
{
}

SpriteBatch::~SpriteBatch()
{
}

void SpriteBatch::Init() {
    CreateVertexArray();
}

void SpriteBatch::Begin(GlyphSortType sortType /* GlyphSortType::TEXTURE */) {
    m_sortType = sortType;
    m_renderBatches.clear();
   
    // Makes _glpyhs.size() == 0, however it does not free internal memory.
    // So when we later call emplace_back it doesn't need to internally call new.
    m_glyphs.clear();
}

void SpriteBatch::End() {
    // Set up all pointers for fast sorting
    m_glyphPointers.resize(m_glyphs.size());
    for (size_t i = 0; i < m_glyphs.size(); i++) {
        m_glyphPointers[i] = &m_glyphs[i];
    }

    SortGlyphs();
    CreateRenderBatches();
}

void SpriteBatch::Draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color) {
    m_glyphs.emplace_back(destRect, uvRect, texture, depth, color);
}

void SpriteBatch::Draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color, float angle) {
    m_glyphs.emplace_back(destRect, uvRect, texture, depth, color, angle);
}

void SpriteBatch::Draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color, const glm::vec2& dir) {
    const glm::vec2 right(1.0f, 0.0f);
    float angle = acos(glm::dot(right, dir));
    if (dir.y < 0.0f) angle = -angle;

    m_glyphs.emplace_back(destRect, uvRect, texture, depth, color, angle);
}

void SpriteBatch::BatchRender() {

    // Bind our VAO. This sets up the opengl state we need, including the 
    // vertex attribute pointers and it binds the VBO
    glBindVertexArray(m_vao);

    for (size_t i = 0; i < m_renderBatches.size(); i++) {
        glBindTexture(GL_TEXTURE_2D, m_renderBatches[i].texture);

        glDrawArrays(GL_TRIANGLES, m_renderBatches[i].offset, m_renderBatches[i].numVertices);
    }

    glBindVertexArray(0);
}

void SpriteBatch::CreateRenderBatches() {
    // This will store all the vertices that we need to upload
    std::vector <Vertex2D> vertices;
    // Resize the buffer to the exact size we need so we can treat
    // it like an array
    vertices.resize(m_glyphPointers.size() * 6);

    if (m_glyphPointers.empty()) {
        return;
    }

    int offset = 0; // current offset
    int cv = 0; // current vertex

    //Add the first batch
    m_renderBatches.emplace_back(offset, 6, m_glyphPointers[0]->texture);
    vertices[cv++] = m_glyphPointers[0]->topLeft;
    vertices[cv++] = m_glyphPointers[0]->bottomLeft;
    vertices[cv++] = m_glyphPointers[0]->bottomRight;
    vertices[cv++] = m_glyphPointers[0]->bottomRight;
    vertices[cv++] = m_glyphPointers[0]->topRight;
    vertices[cv++] = m_glyphPointers[0]->topLeft;
    offset += 6;

    //Add all the rest of the glyphs
    for (size_t cg = 1; cg < m_glyphPointers.size(); cg++) {

        // Check if this glyph can be part of the current batch
        if (m_glyphPointers[cg]->texture != m_glyphPointers[cg - 1]->texture) {
            // Make a new batch
            m_renderBatches.emplace_back(offset, 6, m_glyphPointers[cg]->texture);
        } else {
            // If its part of the current batch, just increase numVertices
            m_renderBatches.back().numVertices += 6;
        }
        vertices[cv++] = m_glyphPointers[cg]->topLeft;
        vertices[cv++] = m_glyphPointers[cg]->bottomLeft;
        vertices[cv++] = m_glyphPointers[cg]->bottomRight;
        vertices[cv++] = m_glyphPointers[cg]->bottomRight;
        vertices[cv++] = m_glyphPointers[cg]->topRight;
        vertices[cv++] = m_glyphPointers[cg]->topLeft;
        offset += 6;
    }

    // Bind our VBO
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    // Orphan the buffer (for speed)
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex2D), nullptr, GL_DYNAMIC_DRAW);
    // Upload the data
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex2D), vertices.data());

    // Unbind the VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void SpriteBatch::CreateVertexArray() {

    // Generate the VAO if it isn't already generated
    if (m_vao == 0) {
        glGenVertexArrays(1, &m_vao);
    }
    
    // Bind the VAO. All subsequent opengl calls will modify it's state.
    glBindVertexArray(m_vao);

    //G enerate the VBO if it isn't already generated
    if (m_vbo == 0) {
        glGenBuffers(1, &m_vbo);
    }
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    //Tell opengl what attribute arrays we need
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    //This is the position attribute pointer
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void*)offsetof(Vertex2D, position));
    //This is the color attribute pointer
    glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex2D), (void*)offsetof(Vertex2D, color));
    //This is the UV attribute pointer
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void*)offsetof(Vertex2D, uv));

    glBindVertexArray(0);

}

void SpriteBatch::SortGlyphs() {
   
    switch (m_sortType) {
        case GlyphSortType::BACK_TO_FRONT:
            std::stable_sort(m_glyphPointers.begin(), m_glyphPointers.end(), CompareBackToFront);
            break;
        case GlyphSortType::FRONT_TO_BACK:
            std::stable_sort(m_glyphPointers.begin(), m_glyphPointers.end(), CompareFrontToBack);
            break;
        case GlyphSortType::TEXTURE:
            std::stable_sort(m_glyphPointers.begin(), m_glyphPointers.end(), CompareTexture);
            break;
    }
}

bool SpriteBatch::CompareFrontToBack(Glyph* a, Glyph* b) {
    return (a->depth < b->depth);
}

bool SpriteBatch::CompareBackToFront(Glyph* a, Glyph* b) {
    return (a->depth > b->depth);
}

bool SpriteBatch::CompareTexture(Glyph* a, Glyph* b) {
    return (a->texture < b->texture);
}

}