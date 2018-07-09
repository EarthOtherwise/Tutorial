#pragma once
#include<string>
#include"GL/glew.h"
#include<fstream>
#include<vector>
#include"ErrHandler.h"

namespace Otherwise
{
	GLuint compileLinkSimpleShaders(const std::string &vertShaderFilePath, const std::string &fragShaderFilePath);
}

