#include "OCube.h"
#include "ImageLoad.h"

namespace Otherwise
{

	Otherwise::OCube::OCube(glm::vec3 position, float sideLength, GLuint firstTextureFilePath,
		GLuint secondTextureFilePath, GLuint thirdTextureFilePath, 
		GLuint fourthTextureFilePath, GLuint fifthTextureFilePath, 
		GLuint sixthTextureFilePath, MultiSprite* multiSprite) : mSideLength(sideLength), mMultiSprite(multiSprite)
	{
		mPosition = position;
		mTextures.push_back(firstTextureFilePath);
		mTextures.push_back(secondTextureFilePath);
		mTextures.push_back(thirdTextureFilePath);
		mTextures.push_back(fourthTextureFilePath);
		mTextures.push_back(fifthTextureFilePath);
		mTextures.push_back(sixthTextureFilePath);

		mBox.pos = &mPosition;
		float halfSideLength = sideLength / 2.0f;
		mBox.maxOffset = glm::vec3(halfSideLength, halfSideLength, halfSideLength);
		mBox.minOffset = glm::vec3(-halfSideLength, -halfSideLength, -halfSideLength);
	}

	OCube::~OCube()
	{
	}

	void OCube::changeTextures(GLuint firstTextureFilePath, GLuint secondTextureFilePath, GLuint thirdTextureFilePath, GLuint fourthTextureFilePath, GLuint fifthTextureFilePath, GLuint sixthTextureFilePath)
	{
		mTextures.clear();
		mTextures.push_back(firstTextureFilePath);
		mTextures.push_back(secondTextureFilePath);
		mTextures.push_back(thirdTextureFilePath);
		mTextures.push_back(fourthTextureFilePath);
		mTextures.push_back(fifthTextureFilePath);
		mTextures.push_back(sixthTextureFilePath);
	}

	void Otherwise::OCube::renderEntity()
	{
		float halfSideLength = mSideLength / 2;

		//front
		mMultiSprite->addSprite(mTextures[0], 0.0f,
			Otherwise::Vertex(mPosition.x - halfSideLength, mPosition.y - halfSideLength, mPosition.z - halfSideLength, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, Otherwise::ColourRGBA8(255, 255, 255, 255)),
			Otherwise::Vertex(mPosition.x - halfSideLength, mPosition.y + halfSideLength, mPosition.z - halfSideLength, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, Otherwise::ColourRGBA8(255, 255, 255, 255)),
			Otherwise::Vertex(mPosition.x + halfSideLength, mPosition.y - halfSideLength, mPosition.z - halfSideLength, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, Otherwise::ColourRGBA8(255, 255, 255, 255)),
			Otherwise::Vertex(mPosition.x + halfSideLength, mPosition.y + halfSideLength, mPosition.z - halfSideLength, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, Otherwise::ColourRGBA8(255, 255, 255, 255)));
		//right
		mMultiSprite->addSprite(mTextures[1], 0.0f,
			Otherwise::Vertex(mPosition.x + halfSideLength, mPosition.y - halfSideLength, mPosition.z - halfSideLength, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, Otherwise::ColourRGBA8(255, 255, 255, 255)),
			Otherwise::Vertex(mPosition.x + halfSideLength, mPosition.y + halfSideLength, mPosition.z - halfSideLength, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, Otherwise::ColourRGBA8(255, 255, 255, 255)),
			Otherwise::Vertex(mPosition.x + halfSideLength, mPosition.y - halfSideLength, mPosition.z + halfSideLength, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, Otherwise::ColourRGBA8(255, 255, 255, 255)),
			Otherwise::Vertex(mPosition.x + halfSideLength, mPosition.y + halfSideLength, mPosition.z + halfSideLength, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, Otherwise::ColourRGBA8(255, 255, 255, 255)));
		//bottom
		mMultiSprite->addSprite(mTextures[2], 0.0f,
			Otherwise::Vertex(mPosition.x - halfSideLength, mPosition.y - halfSideLength, mPosition.z + halfSideLength, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, Otherwise::ColourRGBA8(255, 255, 255, 255)),
			Otherwise::Vertex(mPosition.x - halfSideLength, mPosition.y - halfSideLength, mPosition.z - halfSideLength, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, Otherwise::ColourRGBA8(255, 255, 255, 255)),
			Otherwise::Vertex(mPosition.x + halfSideLength, mPosition.y - halfSideLength, mPosition.z + halfSideLength, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f, Otherwise::ColourRGBA8(255, 255, 255, 255)),
			Otherwise::Vertex(mPosition.x + halfSideLength, mPosition.y - halfSideLength, mPosition.z - halfSideLength, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, Otherwise::ColourRGBA8(255, 255, 255, 255)));
		//left
		mMultiSprite->addSprite(mTextures[3], 0.0f,
			Otherwise::Vertex(mPosition.x - halfSideLength, mPosition.y - halfSideLength, mPosition.z + halfSideLength, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, Otherwise::ColourRGBA8(255, 255, 255, 255)),
			Otherwise::Vertex(mPosition.x - halfSideLength, mPosition.y + halfSideLength, mPosition.z + halfSideLength, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, Otherwise::ColourRGBA8(255, 255, 255, 255)),
			Otherwise::Vertex(mPosition.x - halfSideLength, mPosition.y - halfSideLength, mPosition.z - halfSideLength, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, Otherwise::ColourRGBA8(255, 255, 255, 255)),
			Otherwise::Vertex(mPosition.x - halfSideLength, mPosition.y + halfSideLength, mPosition.z - halfSideLength, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, Otherwise::ColourRGBA8(255, 255, 255, 255)));
		//back
		mMultiSprite->addSprite(mTextures[4], 0.0f,
			Otherwise::Vertex(mPosition.x - halfSideLength, mPosition.y - halfSideLength, mPosition.z + halfSideLength, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f, Otherwise::ColourRGBA8(255, 255, 255, 255)),
			Otherwise::Vertex(mPosition.x - halfSideLength, mPosition.y + halfSideLength, mPosition.z + halfSideLength, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, Otherwise::ColourRGBA8(255, 255, 255, 255)),
			Otherwise::Vertex(mPosition.x + halfSideLength, mPosition.y - halfSideLength, mPosition.z + halfSideLength, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, Otherwise::ColourRGBA8(255, 255, 255, 255)),
			Otherwise::Vertex(mPosition.x + halfSideLength, mPosition.y + halfSideLength, mPosition.z + halfSideLength, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, Otherwise::ColourRGBA8(255, 255, 255, 255)));
		//top
		mMultiSprite->addSprite(mTextures[5], 0.0f,
			Otherwise::Vertex(mPosition.x - halfSideLength, mPosition.y + halfSideLength, mPosition.z - halfSideLength, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, Otherwise::ColourRGBA8(255, 255, 255, 255)),
			Otherwise::Vertex(mPosition.x - halfSideLength, mPosition.y + halfSideLength, mPosition.z + halfSideLength, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, Otherwise::ColourRGBA8(255, 255, 255, 255)),
			Otherwise::Vertex(mPosition.x + halfSideLength, mPosition.y + halfSideLength, mPosition.z - halfSideLength, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, Otherwise::ColourRGBA8(255, 255, 255, 255)),
			Otherwise::Vertex(mPosition.x + halfSideLength, mPosition.y + halfSideLength, mPosition.z + halfSideLength, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, Otherwise::ColourRGBA8(255, 255, 255, 255)));
	}
}