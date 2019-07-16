#pragma once
#include <string>
#include "GL/glew.h"
#include "OMeshRenderer.h"

namespace Otherwise
{

class Skybox
{
public:
	Skybox();
	~Skybox();

	void init(char *filePath, const std::string &posXFilename,
		const std::string &negXFilename, const std::string &posYFilename,
		const std::string &negYFilename, const std::string &posZFilename,
		const std::string &negZFilename);

	void prepare();

	void render();

private:
	GLuint mTextureObject = 0;

	OMesh mMesh;

	GLuint mVao = 0;
	GLuint mVbo = 0;
	GLuint mVertexBuffer = 0;
	GLuint mUvBuffer = 0;
	GLuint mNormalBuffer = 0;
	GLuint mIndexBuffer = 0;
};

}