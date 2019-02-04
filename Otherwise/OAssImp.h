#pragma once
#include<vector>
#include"glm/glm.hpp"
namespace Otherwise
{
void loadAssimp(char *filePath, std::vector<unsigned short> &indices,
	std::vector<glm::vec3> &vertices, std::vector<glm::vec2> &uvs,
	std::vector<glm::vec3> &normals);
}