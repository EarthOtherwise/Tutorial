#pragma once

#include"MultiSprite.h"
#include<vector>
#include"glm/glm.hpp"
#include "OCollision.h"
#include"SpatialEntity.h"


namespace Otherwise
{

	class OCube : public SpatialEntity
	{
	public:
		OCube(glm::vec3 position, float sideLength, GLuint firstTextureFilePath, GLuint secondTextureFilePath,
			GLuint thirdTextureFilePath, GLuint fourthTextureFilePath, GLuint fifthTextureFilePath, 
			GLuint sixthTextureFilePath, MultiSprite* multiSprite);
		~OCube();

		float getCollisionRadius() { return mSideLength; }

		void changeTextures(GLuint firstTextureFilePath, GLuint secondTextureFilePath,
			GLuint thirdTextureFilePath, GLuint fourthTextureFilePath, GLuint fifthTextureFilePath,
			GLuint sixthTextureFilePath);

		void renderEntity();
		OCollAABB* getBox() { return &mBox; }

		void move(glm::vec3 delta) 
		{ mPosition += delta; mBox.minOffset += delta; mBox.maxOffset += delta; }

	private:
		OCollAABB mBox;
		float mSideLength;
		std::vector<GLuint> mTextures;
		MultiSprite* mMultiSprite;
	};

}