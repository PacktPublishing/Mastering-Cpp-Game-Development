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
#include "InputManager.h"

namespace BookEngine {

InputManager::InputManager() : m_mouseCoords(0.0f)
{
}


InputManager::~InputManager()
{
}

void InputManager::Update() {
    // Loop through _keyMap using a for each loop, and copy it over to _previousKeyMap
    for (auto& it : m_keyMap) {
        m_previousKeyMap[it.first] = it.second;
    }
}

void InputManager::KeyPress(unsigned int keyID) {
    // Here we are treating _keyMap as an associative array.
    // if keyID doesn't already exist in _keyMap, it will get added
    m_keyMap[keyID] = true;
}

void InputManager::KeyRelease(unsigned int keyID) {
    m_keyMap[keyID] = false;
}

void InputManager::SetMouseCoords(float x, float y) {
    m_mouseCoords.x = x;
    m_mouseCoords.y = y;
}

bool InputManager::isKeyDown(unsigned int keyID) {
    // We dont want to use the associative array approach here
    // because we don't want to create a key if it doesnt exist.
    // So we do it manually
    auto it = m_keyMap.find(keyID);
    if (it != m_keyMap.end()) {
        // Found the key
        return it->second;
    } else {
        // Didn't find the key
        return false;
    }
}

bool InputManager::isKeyPressed(unsigned int keyID) {
    // Check if it is pressed this frame, and wasn't pressed last frame
    if (isKeyDown(keyID) == true && wasKeyDown(keyID) == false) {
        return true;
    }
    return false;
}

bool InputManager::wasKeyDown(unsigned int keyID) {
    // We dont want to use the associative array approach here
    // because we don't want to create a key if it doesnt exist.
    // So we do it manually
    auto it = m_previousKeyMap.find(keyID);
    if (it != m_previousKeyMap.end()) {
        // Found the key
        return it->second;
    } else {
        // Didn't find the key
        return false;
    }
}

}