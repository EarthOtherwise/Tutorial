#pragma once
#include<vector>
#include"OAssImp.h"
#include"glm/glm.hpp"
#include"GL/glew.h"
namespace Otherwise
{

struct OMesh
{
	std::vector<unsigned short> indices;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	GLuint texture;
};

class OMeshRenderer
{
public:
	OMeshRenderer();
	~OMeshRenderer();

	void init();
	void addMesh(char *filePath, GLuint texture);
	void prepareMesh();
	void renderMesh();

private:
	GLuint mVao = 0;
	GLuint mVbo = 0;
	GLuint mVertexBuffer = 0;
	GLuint mUvBuffer = 0;
	GLuint mNormalBuffer = 0;
	GLuint mIndexBuffer = 0;
	std::vector<OMesh> mMeshes;
};

}