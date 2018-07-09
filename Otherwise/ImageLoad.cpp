#include "ImageLoad.h"
#include"ErrHandler.h"
#include<stdio.h>
#include"PicoPNG.h"

namespace Otherwise
{
	GLuint loadBmp(std::string filePath)
	{
		unsigned char header[54];
		unsigned int dataPos;
		unsigned int width, height;
		unsigned int imageSize;

		unsigned char* data;

		const char* filePathPtr = filePath.c_str();

		FILE* file;

		errno_t fileErr = fopen_s(&file, filePathPtr, "r");
		if (!file)
		{
			Otherwise::throwError("BMP File", "BMP File " + filePath + " could not be opened.");
		}

		if (fread(header, 1, 54, file) != 54)
		{
			Otherwise::throwError("BMP File", "BMP File " + filePath + " has an incorrect header length.");
		}

		if (header[0] != 'B' || header[1] != 'M')
		{
			Otherwise::throwError("BMP File", "BMP File " + filePath + " is not formatted correctly.");
		}

		if (*(int*)&(header[0x1E]) != 0)
		{
			Otherwise::throwError("BMP File", "BMP File " + filePath + " is not a 24 bpp file.");
		}

		if (*(int*)&(header[0x1C]) != 24)
		{
			Otherwise::throwError("BMP File", "BMP File " + filePath + " is not a 24 bpp file.");
		}

		dataPos = *(int*)&(header[0x0A]);
		imageSize = *(int*)&(header[0x22]);
		width = *(int*)&(header[0x12]);
		height = *(int*)&(header[0x16]);

		if (imageSize == 0)
		{
			imageSize = width*height * 3;
		}

		if (dataPos == 0)
		{
			dataPos = 54;
		}

		data = new unsigned char[imageSize];

		fread(data, 1, imageSize, file);

		fclose(file);

		GLuint textureID = 0;
		glGenTextures(1, &textureID);

		glBindTexture(GL_TEXTURE_2D, textureID);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

		delete[] data;

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		//not sure if this line is needed
		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);

		return textureID;
	}

	GLuint loadPng(std::string filePath)
	{
		unsigned long width, height;

		std::ifstream file(filePath, std::ios::binary);
		if (file.fail())
		{
			throwError("PNG File", "PNG " + filePath + " file could not be opened.");
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
			throwError("PNg Decoder Error", "PNG decoder threw error: " + std::to_string(errCode));
		}

		GLuint textureID = 0;
		glGenTextures(1, &textureID);

		glBindTexture(GL_TEXTURE_2D, textureID);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(pixelData[0]));

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		//not sure if this line is needed
		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);

		return textureID;
	}
}