#include "OCube.h"
#include "ImageLoad.h"

namespace Otherwise
{

	Otherwise::OCube::OCube(glm::vec3 position, float sideLength, std::string firstTextureFilePath,
		std::string secondTextureFilePath, std::string thirdTextureFilePath, 
		std::string fourthTextureFilePath, std::string fifthTextureFilePath, 
		std::string sixthTextureFilePath, MultiSprite* multiSprite) : mSideLength(sideLength), mMultiSprite(multiSprite)
	{
		mPosition = position;
		mTextures.push_back(firstTextureFilePath);
		mTextures.push_back(secondTextureFilePath);
		mTextures.push_back(thirdTextureFilePath);
		mTextures.push_back(fourthTextureFilePath);
		mTextures.push_back(fifthTextureFilePath);
		mTextures.push_back(sixthTextureFilePath);
	}

	OCube::~OCube()
	{
	}

	void Otherwise::OCube::renderEntity()
	{
		float halfSideLength = mSideLength / 2;

		//front
		mMultiSprite->addSprite(Otherwise::loadPng(mTextures[0]), 0.0f,
			Otherwise::Vertex(mPosition.x - halfSideLength, mPosition.y - halfSideLength, mPosition.z - halfSideLength, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, Otherwise::ColourRGBA8(255, 255, 255, 255)),
			Otherwise::Vertex(mPosition.x - halfSideLength, mPosition.y + halfSideLength, mPosition.z - halfSideLength, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, Otherwise::ColourRGBA8(255, 255, 255, 255)),
			Otherwise::Vertex(mPosition.x + halfSideLength, mPosition.y - halfSideLength, mPosition.z - halfSideLength, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, Otherwise::ColourRGBA8(255, 255, 255, 255)),
			Otherwise::Vertex(mPosition.x + halfSideLength, mPosition.y + halfSideLength, mPosition.z - halfSideLength, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, Otherwise::ColourRGBA8(255, 255, 255, 255)));
		//right
		mMultiSprite->addSprite(Otherwise::loadPng(mTextures[1]), 0.0f,
			Otherwise::Vertex(mPosition.x + halfSideLength, mPosition.y - halfSideLength, mPosition.z - halfSideLength, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, Otherwise::ColourRGBA8(255, 255, 255, 255)),
			Otherwise::Vertex(mPosition.x + halfSideLength, mPosition.y + halfSideLength, mPosition.z - halfSideLength, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, Otherwise::ColourRGBA8(255, 255, 255, 255)),
			Otherwise::Vertex(mPosition.x + halfSideLength, mPosition.y - halfSideLength, mPosition.z + halfSideLength, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, Otherwise::ColourRGBA8(255, 255, 255, 255)),
			Otherwise::Vertex(mPosition.x + halfSideLength, mPosition.y + halfSideLength, mPosition.z + halfSideLength, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, Otherwise::ColourRGBA8(255, 255, 255, 255)));
		//bottom
		mMultiSprite->addSprite(Otherwise::loadPng(mTextures[2]), 0.0f,
			Otherwise::Vertex(mPosition.x - halfSideLength, mPosition.y - halfSideLength, mPosition.z + halfSideLength, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, Otherwise::ColourRGBA8(255, 255, 255, 255)),
			Otherwise::Vertex(mPosition.x - halfSideLength, mPosition.y - halfSideLength, mPosition.z - halfSideLength, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, Otherwise::ColourRGBA8(255, 255, 255, 255)),
			Otherwise::Vertex(mPosition.x + halfSideLength, mPosition.y - halfSideLength, mPosition.z + halfSideLength, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f, Otherwise::ColourRGBA8(255, 255, 255, 255)),
			Otherwise::Vertex(mPosition.x + halfSideLength, mPosition.y - halfSideLength, mPosition.z - halfSideLength, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, Otherwise::ColourRGBA8(255, 255, 255, 255)));
		//left
		mMultiSprite->addSprite(Otherwise::loadPng(mTextures[3]), 0.0f,
			Otherwise::Vertex(mPosition.x - halfSideLength, mPosition.y - halfSideLength, mPosition.z + halfSideLength, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, Otherwise::ColourRGBA8(255, 255, 255, 255)),
			Otherwise::Vertex(mPosition.x - halfSideLength, mPosition.y + halfSideLength, mPosition.z + halfSideLength, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, Otherwise::ColourRGBA8(255, 255, 255, 255)),
			Otherwise::Vertex(mPosition.x - halfSideLength, mPosition.y - halfSideLength, mPosition.z - halfSideLength, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, Otherwise::ColourRGBA8(255, 255, 255, 255)),
			Otherwise::Vertex(mPosition.x - halfSideLength, mPosition.y + halfSideLength, mPosition.z - halfSideLength, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, Otherwise::ColourRGBA8(255, 255, 255, 255)));
		//back
		mMultiSprite->addSprite(Otherwise::loadPng(mTextures[4]), 0.0f,
			Otherwise::Vertex(mPosition.x - halfSideLength, mPosition.y - halfSideLength, mPosition.z + halfSideLength, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f, Otherwise::ColourRGBA8(255, 255, 255, 255)),
			Otherwise::Vertex(mPosition.x - halfSideLength, mPosition.y + halfSideLength, mPosition.z + halfSideLength, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, Otherwise::ColourRGBA8(255, 255, 255, 255)),
			Otherwise::Vertex(mPosition.x + halfSideLength, mPosition.y - halfSideLength, mPosition.z + halfSideLength, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, Otherwise::ColourRGBA8(255, 255, 255, 255)),
			Otherwise::Vertex(mPosition.x + halfSideLength, mPosition.y + halfSideLength, mPosition.z + halfSideLength, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, Otherwise::ColourRGBA8(255, 255, 255, 255)));
		//top
		mMultiSprite->addSprite(Otherwise::loadPng(mTextures[5]), 0.0f,
			Otherwise::Vertex(mPosition.x - halfSideLength, mPosition.y + halfSideLength, mPosition.z - halfSideLength, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, Otherwise::ColourRGBA8(255, 255, 255, 255)),
			Otherwise::Vertex(mPosition.x - halfSideLength, mPosition.y + halfSideLength, mPosition.z + halfSideLength, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, Otherwise::ColourRGBA8(255, 255, 255, 255)),
			Otherwise::Vertex(mPosition.x + halfSideLength, mPosition.y + halfSideLength, mPosition.z - halfSideLength, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, Otherwise::ColourRGBA8(255, 255, 255, 255)),
			Otherwise::Vertex(mPosition.x + halfSideLength, mPosition.y + halfSideLength, mPosition.z + halfSideLength, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, Otherwise::ColourRGBA8(255, 255, 255, 255)));
	}
}