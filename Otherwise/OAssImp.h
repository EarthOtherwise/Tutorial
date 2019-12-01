#pragma once
#include<vector>
#include"MathLibraryInterface.h"
namespace Otherwise
{
void loadAssimp(char *filePath, std::vector<unsigned short> &indices,
	std::vector<ovec3> &vertices, std::vector<ovec2> &uvs,
	std::vector<ovec3> &normals);
}