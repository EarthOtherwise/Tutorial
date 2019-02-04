#include "OAssImp.h"
#include"ErrHandler.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Otherwise
{
void loadAssimp(char * filePath, std::vector<unsigned short> &indices, 
	std::vector<glm::vec3> &vertices, std::vector<glm::vec2> &uvs, 
	std::vector<glm::vec3> &normals)
{
	Assimp::Importer importer;

	const aiScene *scene = importer.ReadFile(filePath,
		aiProcess_JoinIdenticalVertices | aiProcess_Triangulate);
	if (!scene)
	{
		throwError("MeshImport", importer.GetErrorString());
	}

	vertices.reserve(scene->mMeshes[0]->mNumVertices);
	uvs.reserve(scene->mMeshes[0]->mNumVertices);
	normals.reserve(scene->mMeshes[0]->mNumVertices);
	indices.reserve(3 * scene->mMeshes[0]->mNumFaces);

	for (unsigned int i = 0; i < scene->mMeshes[0]->mNumVertices; i++)
	{
		vertices.push_back(glm::vec3(scene->mMeshes[0]->mVertices[i].x,
			scene->mMeshes[0]->mVertices[i].y, scene->mMeshes[0]->mVertices[i].z));
		uvs.push_back(glm::vec2(scene->mMeshes[0]->mTextureCoords[0][i].x,
			scene->mMeshes[0]->mTextureCoords[0][i].y));
		normals.push_back(glm::vec3(scene->mMeshes[0]->mNormals[i].x,
			scene->mMeshes[0]->mNormals[i].y, scene->mMeshes[0]->mNormals[i].z));
	}

	for (unsigned int i = 0; i < scene->mMeshes[0]->mNumFaces; i++)
	{
		indices.push_back(scene->mMeshes[0]->mFaces[i].mIndices[0]);
		indices.push_back(scene->mMeshes[0]->mFaces[i].mIndices[1]);
		indices.push_back(scene->mMeshes[0]->mFaces[i].mIndices[2]);
	}
}
}
