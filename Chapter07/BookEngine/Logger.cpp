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
#include "Logger.h"
#include <fstream>
#include <iostream>

namespace BookEngine
{

	extern void WriteLog(LogType type, const std::string &logMessage)
	{
		std::ofstream errorFile("ErrorLog.txt", std::ios::app);
		std::ofstream runtimeLogFile("RuntimeLog.txt", std::ios::app);
		switch (type)
		{
		case LogType::ERROR:
			//const char* ERRORFILEPATH = "\\Logs\\Error.txt";

			errorFile << logMessage.c_str() << std::endl;
			errorFile.close();
			//Print to screen
			std::cout << logMessage.c_str() << std::endl;
			break;
		case LogType::RUN:
			//const char* ERRORFILEPATH = "\\Logs\\Error.txt";

			runtimeLogFile << logMessage.c_str() << std::endl;
			runtimeLogFile.close();
			//Print to screen
			std::cout << logMessage.c_str() << std::endl;
			break;
		}
	}
}