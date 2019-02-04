#include "OMeshRenderer.h"
#include<iostream>
namespace Otherwise
{

OMeshRenderer::OMeshRenderer()
{
}

OMeshRenderer::~OMeshRenderer()
{
}

void OMeshRenderer::init()
{
	glGenVertexArrays(1, &mVao);
	glBindVertexArray(mVao);

	glGenBuffers(1, &mVertexBuffer);
	glGenBuffers(1, &mUvBuffer);
	glGenBuffers(1, &mNormalBuffer);
	glGenBuffers(1, &mIndexBuffer);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, mUvBuffer);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, mNormalBuffer);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void OMeshRenderer::addMesh(char * filePath, GLuint texture)
{
	OMesh tempMesh;
	tempMesh.texture = texture;
	loadAssimp(filePath, tempMesh.indices, tempMesh.vertices, tempMesh.uvs, 
		tempMesh.normals);
	mMeshes.push_back(tempMesh);
}

void OMeshRenderer::prepareMesh()
{
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, mMeshes[0].vertices.size() * sizeof(glm::vec3), 
		&mMeshes[0].vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, mUvBuffer);
	glBufferData(GL_ARRAY_BUFFER, mMeshes[0].uvs.size() * sizeof(glm::vec2),
		&mMeshes[0].uvs[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, mNormalBuffer);
	glBufferData(GL_ARRAY_BUFFER, mMeshes[0].normals.size() * sizeof(glm::vec3),
		&mMeshes[0].normals[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
		mMeshes[0].indices.size() * sizeof(unsigned short),&mMeshes[0].indices[0], 
		GL_STATIC_DRAW);

	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void OMeshRenderer::renderMesh()
{
	glBindVertexArray(mVao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);

	std::cout << mMeshes[0].vertices.size() << std::endl;

	glBindTexture(GL_TEXTURE_2D, mMeshes[0].texture);

	glDrawElements(GL_TRIANGLES, mMeshes[0].indices.size(), GL_UNSIGNED_SHORT,
		(void *)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

}