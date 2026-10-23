#include "Mesh.h"
#include <iostream>
#include <sstream>
#include <fstream>

std::vector<std::string> split(std::string s, std::string t) {
	std::vector<std::string> res;
	while (1) {
		int pos = s.find(t);
		if (pos == -1) {
			res.push_back(s);
			break;
		}
		res.push_back(s.substr(0, pos));
		s = s.substr(pos + 1, s.size() - pos - 1);
	}
	return res;
}

Mesh::Mesh() 
	:mLoaded(false)
{ 
}

Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &mVAO);
    glDeleteBuffers(1, &mVBO);
}

bool Mesh::loadOBJ(const std::string& filename)
{
	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> tempVertices;
	std::vector<glm::vec2> tempUVs;
	std::vector<glm::vec3> tempNormals;


	if (filename.find(".obj") != std::string::npos)
	{
		std::ifstream fin(filename, std::ios::in);
		if (!fin)
		{
			std::cerr << "Cannot open " << filename << std::endl;
			return false;
		}

		std::cout << "Loading OBJ file " << filename << " ..." << std::endl;

		std::string lineBuffer;
		while (std::getline(fin, lineBuffer))
		{
			std::stringstream ss(lineBuffer);
			std::string cmd;
			ss >> cmd;

			if (cmd == "v")
			{
				glm::vec3 vertex;
				int dim = 0;
				while (dim < 3 && ss >> vertex[dim])
				{
					dim++;
				}
				tempVertices.push_back(vertex);
			}
			else if (cmd == "vt")
			{
				glm::vec2 uv;
				int dim = 0;
				while (dim < 2 && ss >> uv[dim])
				{
					dim++;
				}
				tempUVs.push_back(uv);
			}
			else if (cmd == "vn")
			{
				glm::vec3 normal;
				int dim = 0;
				while (dim < 3 && ss >> normal[dim])
				{
					dim++;
				}
				normal = glm::normalize(normal);
				tempNormals.push_back(normal);
			}
			else if (cmd == "f")
			{
				std::string faceData;
				std::vector<std::string> faceTokens;
				while (ss >> faceData) {
					faceTokens.push_back(faceData);
				}

				// Triangulation automatique (gère les triangles, quads et polygones)
				for (size_t j = 1; j + 1 < faceTokens.size(); ++j) {
					std::string tokens[3] = { faceTokens[0], faceTokens[j], faceTokens[j + 1] };

					for (int k = 0; k < 3; ++k) {
						std::vector<std::string> data = split(tokens[k], "/");

						unsigned int vIdx = 0, uvIdx = 0, nIdx = 0;
						if (data.size() > 0 && !data[0].empty()) {
							vIdx = std::stoi(data[0]);
						}
						if (data.size() > 1 && !data[1].empty()) {
							uvIdx = std::stoi(data[1]);
						}
						if (data.size() > 2 && !data[2].empty()) {
							nIdx = std::stoi(data[2]);
						}

						vertexIndices.push_back(vIdx);
						uvIndices.push_back(uvIdx);
						normalIndices.push_back(nIdx);
					}
				}
			}
		}

		fin.close();

		// Construction des sommets du mesh
		for (unsigned int i = 0; i < vertexIndices.size(); i++)
		{
			Vertex meshVertex;

			if (vertexIndices[i] > 0 && vertexIndices[i] <= tempVertices.size()) {
				meshVertex.position = tempVertices[vertexIndices[i] - 1];
			}

			if (normalIndices[i] > 0 && normalIndices[i] <= tempNormals.size()) {
				meshVertex.normal = tempNormals[normalIndices[i] - 1];
			}
			else {
				meshVertex.normal = glm::vec3(0.0f, 0.0f, 1.0f); // Sécurité si normale manquante
			}

			if (uvIndices[i] > 0 && uvIndices[i] <= tempUVs.size()) {
				meshVertex.texCoords = tempUVs[uvIndices[i] - 1];
			}
			else {
				meshVertex.texCoords = glm::vec2(0.0f, 0.0f);
			}

			mVertices.push_back(meshVertex);
		}

		// Create and initialize the buffers
		initBuffers();

		return (mLoaded = true);
	}

	// We shouldn't get here so return failure
	return false;
}

void Mesh::draw() {
    if (!mLoaded) return;

    glBindVertexArray(mVAO);
    glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
    glBindVertexArray(0);
}

void Mesh::initBuffers() {
    glGenBuffers(1, &mVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(Vertex), &mVertices[0], GL_STATIC_DRAW);

    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);

    //Positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL);
    glEnableVertexAttribArray(0);

	//Normals
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

    //Textures Coords
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}