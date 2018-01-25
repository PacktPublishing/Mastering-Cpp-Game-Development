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
#include <unordered_map>
#include <glm/glm.hpp>

namespace BookEngine {
	
	// Input manager stores a key map that maps SDL_Keys to booleans.
	// If the value in the key map is true, then the key is pressed.
	// Otherwise, it is released.
	class InputManager
	{
	public:
		InputManager();
		~InputManager();

		void Update();

		void KeyPress(unsigned int keyID); //TODO: wrap in SDL Keys in engine local enum
		void KeyRelease(unsigned int keyID);

		bool isKeyDown(unsigned int keyID); //Returns true if key is held
		bool isKeyPressed(unsigned int keyID); //Returns true if key was pressed this update

		void SetMouseCoords(float x, float y);
		glm::vec2 GetMouseCoords() const { return m_mouseCoords; };

	private:

		bool wasKeyDown(unsigned int keyID);

		std::unordered_map<unsigned int, bool> m_keyMap;
		std::unordered_map<unsigned int, bool> m_previousKeyMap;
		glm::vec2 m_mouseCoords;
	};
}

