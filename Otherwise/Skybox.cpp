#include "Skybox.h"
#include "PicoPNG.h"
#include "ErrHandler.h"
#include "OAssImp.h"
#include "glm/glm.hpp"

namespace Otherwise
{

Skybox::Skybox()
{
}


Skybox::~Skybox()
{
}

void Skybox::init(char * filePath, const std::string & posXFilename, const std::string & negXFilename, const std::string & posYFilename, const std::string & negYFilename, const std::string & posZFilename, const std::string & negZFilename)
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

	loadAssimp(filePath, mMesh.indices, mMesh.vertices, mMesh.uvs,
		mMesh.normals);

	glGenTextures(1, &mTextureObject);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mTextureObject);

	std::string m_fileNames[6];

	m_fileNames[0] = posXFilename;
	m_fileNames[1] = negXFilename;
	m_fileNames[2] = posYFilename;
	m_fileNames[3] = negYFilename;
	m_fileNames[4] = posZFilename;
	m_fileNames[5] = negZFilename;

	unsigned long width, height;

	for (unsigned int i = 0; i < 6; i++)
	{
		width = 0;
		height = 0;

		std::ifstream file(m_fileNames[i], std::ios::binary);
		if (file.fail())
		{
			throwError("PNG File", "PNG " + m_fileNames[i] + " file could not be opened.");
		}

		file.seekg(0, std::ios::end);

		unsigned int fileSize = file.tellg();

		file.seekg(0, std::ios::beg);

		fileSize -= file.tellg();

		std::vector<char> imageData;

		imageData.resize(fileSize);

		file.read(&(imageData[0]), fileSize);

		std::vector<unsigned char> tempImageData;
		std::vector<unsigned char> pixelData;

		tempImageData.resize(fileSize);
		for (size_t i = 0; i < fileSize; i++)
		{
			tempImageData[i] = imageData[i];
		}

		int errCode = decodePNG(pixelData, width, height, &(tempImageData[0]), tempImageData.size());

		if (errCode != 0)
		{
			throwError("PNG Decoder Error", "PNG decoder threw error: " + std::to_string(errCode));
		}

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(pixelData[0]));

	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void Skybox::prepare()
{
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, mMesh.vertices.size() * sizeof(glm::vec3),
		&mMesh.vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, mUvBuffer);
	glBufferData(GL_ARRAY_BUFFER, mMesh.uvs.size() * sizeof(glm::vec2),
		&mMesh.uvs[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, mNormalBuffer);
	glBufferData(GL_ARRAY_BUFFER, mMesh.normals.size() * sizeof(glm::vec3),
		&mMesh.normals[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		mMesh.indices.size() * sizeof(unsigned short), &mMesh.indices[0],
		GL_STATIC_DRAW);

	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Skybox::render()
{
	glCullFace(GL_FRONT);
	glDepthFunc(GL_LEQUAL);

	glBindVertexArray(mVao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);

	glBindTexture(GL_TEXTURE_CUBE_MAP, mTextureObject);

	glDrawElements(GL_TRIANGLES, mMesh.indices.size(), GL_UNSIGNED_SHORT,
		(void *)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glCullFace(GL_BACK);
	glDepthFunc(GL_LESS);
}

}