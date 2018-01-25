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
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>
#include "MeshLoader.h"
#include "Exception.h"
#include "Logger.h"
// Very, VERY simple OBJ loader.
// Here is a short list of features a real function would provide : 
// - Binary files. Reading a model should be just a few memcpy's away, not parsing a file at runtime. In short : OBJ is not very great.
// - Animations & bones (includes bones weights)
// - Multiple UVs
// - All attributes should be optional, not "forced"
// - More stable. Change a line in the OBJ file and it crashes.
// - More secure. Change another line and you can inject code.
// - Loading from memory, stream, etc

namespace BookEngine
{

	bool LoadOBJ(
		std::string path,
		std::vector<glm::vec3> & out_vertices,
		std::vector<glm::vec2> & out_uvs,
		std::vector<glm::vec3> & out_normals
	) {
		WriteLog(LogType::RUN, "Loading OBJ file " + path + " ...");

		std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
		std::vector<glm::vec3> temp_vertices;
		std::vector<glm::vec2> temp_uvs;
		std::vector<glm::vec3> temp_normals;

		try {

			std::ifstream in(path, std::ios::in);
			if (!in)
			{
				throw Exception("Error opening OBJ file: " + path);
			}

			std::string line;
			while (std::getline(in, line))
			{
				//check v for vertices
				if (line.substr(0, 2) == "v ") {
					std::istringstream v(line.substr(2));
					glm::vec3 vert;
					double x, y, z;
					v >> x;v >> y;v >> z;
					vert = glm::vec3(x, y, z);
					temp_vertices.push_back(vert);
				}
				//check for texture co-ordinate
				else if (line.substr(0, 2) == "vt") {

					std::istringstream v(line.substr(3));
					glm::vec2 uv;
					double U, V;
					v >> U;v >> V;
					uv = glm::vec2(U, V);
					uv.y = -uv.y; // Invert V coordinate since we will only use DDS texture, which are inverted. Remove if you want to use TGA or BMP loaders.
					temp_uvs.push_back(uv);
				}
				//check for normals
				else if (line.substr(0, 2) == "vn") {

					std::istringstream v(line.substr(3));
					glm::vec3 normal;
					double x, y, z;
					v >> x;v >> y;v >> z;
					normal = glm::vec3(x, y, z);
					temp_normals.push_back(normal);
				}
				//check for faces
				else if (line.substr(0, 2) == "f ")
				{
					unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
					std::stringstream ss(line.substr(2));
					char throwaway;
					for (int i = 0; i < 3; i++) {
						ss >> vertexIndex[i] >> throwaway >> uvIndex[i] >> throwaway >> normalIndex[i];
					}

					for (int i = 0; i < 3; i++)
					{
						vertexIndices.push_back(vertexIndex[i]);
						uvIndices.push_back(uvIndex[i]);
						normalIndices.push_back(normalIndex[i]);
					}
				}

			}
			//the mesh data is finally calculated here

			for (unsigned int i = 0; i < vertexIndices.size(); i++) {

				// Get the indices of its attributes
				unsigned int vertexIndex = vertexIndices[i];
				unsigned int uvIndex = uvIndices[i];
				unsigned int normalIndex = normalIndices[i];

				// Get the attributes thanks to the index
				glm::vec3 vertex = temp_vertices[vertexIndex - 1];
				glm::vec2 uv = temp_uvs[uvIndex - 1];
				glm::vec3 normal = temp_normals[normalIndex - 1];

				// Put the attributes in buffers
				out_vertices.push_back(vertex);
				out_uvs.push_back(uv);
				out_normals.push_back(normal);

			}
		}
		catch (Exception e)
		{
			WriteLog(LogType::ERROR, e.reason);
			return false;
		}

		return true;
	}
}
