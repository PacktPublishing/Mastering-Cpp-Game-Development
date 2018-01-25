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
#include "IOManager.h"
#include <fstream>
#include "Exception.h"

namespace BookEngine
{
	void IOManager::ReadFileToBuffer(std::vector<unsigned char>& buffer, std::string pathToFile)
	{
		std::ifstream file(pathToFile.c_str(), std::ios::in | std::ios::binary | std::ios::ate);

		//Get filesize
		std::streamsize size = 0;
		if (file.seekg(0, std::ios::end).good()) size = file.tellg();
		if (file.seekg(0, std::ios::beg).good()) size -= file.tellg();

		//Read contents of the file into the vector
		if (size > 0)
		{
			buffer.resize((size_t)size);
			file.read((char*)(&buffer[0]), size);
		}
		else
		{
			buffer.clear();
			file.close();
			throw Exception("Failure to read file to buffer");
		}
	}
}