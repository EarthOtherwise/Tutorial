#pragma once
#include <GL/glew.h>
#include <string>
#include "MathLibraryInterface.h"
#include <vector>

namespace Otherwise
{

void initGraphicsInterface();

void skyboxMode(bool on);

void depthTest(bool on);

void clearBackScreenBuffer();

class Texture2D
{
public:
	void load(const std::string& file_path);
	void use() const;
	GLuint mTexture;
};

/*
Primarily used for skyboxes.
*/
class TextureCubeMap
{
public:
	void load(std::string m_file_names[6]);
	void use();
	GLuint mTexture;
};

class IndexedMesh
{
public:
	void load(char* file_path);
	std::vector<unsigned short> indices;
	std::vector<ovec3> vertices;
	std::vector<ovec2> uvs;
	std::vector<ovec3> normals;
};

class VertexArray
{
public:
	void init();
	void update(const IndexedMesh& object);
	void render();
protected:
	GLuint mVao = 0;
	GLuint mVbo = 0;
	GLuint mVertexBuffer = 0;
	GLuint mUvBuffer = 0;
	GLuint mNormalBuffer = 0;
	GLuint mIndexBuffer = 0;
	unsigned int index_count = 0; 
};

class Shader
{
public:
	void load(const std::string &vertShaderFilePath, const std::string &fragShaderFilePath);
	void use() const;
	GLuint mShaderID = 0;
};

class ShaderUniform
{
public:
	void get(Shader& shader, std::string& name);

	void fill(omat4& fill);
	void fill(ovec3& fill);
	void fill(float& fill);

protected:
	GLint mShaderUniformID = 0;
};

}