#include "Mesh.h"
#include <glm\glm.hpp>
#include <GL\glew.h>
#include <array>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

Mesh* LoadMesh(const std::string& filepath)
{
	std::ifstream inputFile(filepath, std::ios::in);
	if(inputFile)
	{		
		// Stores the initial vertex attribs in vectors as they were ordered in the file
		std::vector<glm::vec3> unorderedPositions;
		std::vector<glm::vec2> unorderedTextureCoords;
		std::vector<glm::vec3> unorderedNormals;

		// Stores the vertex attribs in the correct order
		std::vector<glm::vec3> finalPositions;
		std::vector<glm::vec2> finalTextureCoords;
		std::vector<glm::vec3> finalNormals;

		while(!inputFile.eof())
		{
			std::string line;
			std::getline(inputFile, line);

			// If the line starts with a v, this means the line
			// contains vertex position information
			if(line.compare(0, 2, "v ") == 0)
			{				
				// Erase the prefix
				line.erase(0, 2);

				// Use a string stream to allow converting a string to numerical values
				std::stringstream out(line);	

				float x, y, z;
				out >> x >> y >> z;
				unorderedPositions.push_back(glm::vec3(x, y, z));
			}
			else if(line.compare(0, 2, "vt") == 0)
			{			
				// Erase the prefix
				line.erase(0, 2);

				// Use a string stream to allow converting a string to numerical values
				std::stringstream out(line);

				float s, t;
				out >> s >> t;
				unorderedTextureCoords.push_back(glm::vec2(s, t));
			}
			else if(line.compare(0, 2, "vn") == 0)
			{
				// Erase the prefix
				line.erase(0, 2);

				// Use a string stream to allow converting a string to numerical values
				std::stringstream out(line);

				float x, y, z;
				out >> x >> y >> z;
				unorderedNormals.push_back(glm::vec3(x, y, z));
			}			
			else if(line.compare(0, 2, "f ") == 0)
			{
				// Erase the prefix
				line.erase(0, 2);

				// Split the line up into three vertices
				std::stringstream out(line);

				// For each vertex in the face (triangle)
				// Get the position, texture coord and normal indices.
				const int NUM_VERTICES_PER_FACE = 3;
				for(int i = 0; i < NUM_VERTICES_PER_FACE; ++i)
				{
					// Read the current vertex in the face, this should be in the form 'v/vt/vn'
					std::string s;
					out >> s;

					// Each vertex will have an index of a position, texture coordinate and normal vectors
					// in the original read in vectors.
					std::vector<int> vertexAttribs;

					// Each vertex attribute in a vertex is separated by a '/'.
					const char delim = '/';

					// The current position in the string
					int pos = 0;
					std::size_t curDelimIndex = 0;
					bool endOfDelims = false;
					while(!endOfDelims)
					{
						curDelimIndex = s.find(delim, pos);
						endOfDelims = curDelimIndex == std::string::npos;
						std::string num;

						// Extract the numerical portion of the string
						if(!endOfDelims)
						{
							num = s.substr(pos, curDelimIndex - pos);

							// Move the current position pointer along one character past the delimeter
							pos = curDelimIndex + 1;
						}
						else
						{
							num = s.substr(pos, s.length() - pos);
						}
																	
						// Convert a string to an integer.
						// We MUST subtract one, since OBJ files start indexing at 1, not 0.
						int vertexAttribValue = std::stoi(num) - 1;
						vertexAttribs.push_back(vertexAttribValue);
					}

					// Fill out the final vertex data vectors with the rearranged values
					finalPositions.push_back(unorderedPositions[vertexAttribs[0]]);
					finalTextureCoords.push_back(unorderedTextureCoords[vertexAttribs[1]]);
					finalNormals.push_back(unorderedNormals[vertexAttribs[2]]);
				}				
			}			
		}

		std::vector<GLuint> indices;
		indices.reserve(finalPositions.size());
		for(int i = 0; i < finalPositions.size(); ++i)
		{
			indices.push_back(i);
		}

		std::cout << "File '" << filepath << "' loaded successfully.\n";

		return new Mesh(finalPositions, finalTextureCoords, finalNormals, indices);
	}
	else
	{
		std::cerr << "Failed to load file: '" << filepath << "'\n";
		return nullptr;
	}
}