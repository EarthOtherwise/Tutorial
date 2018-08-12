#pragma once

#include"MultiSprite.h"
#include<vector>
#include"glm/glm.hpp"
#include"SpatialEntity.h"

namespace Otherwise
{

	class OCube : public SpatialEntity
	{
	public:
		OCube(glm::vec3 position, float sideLength, std::string firstTextureFilePath, std::string secondTextureFilePath,
			std::string thirdTextureFilePath, std::string fourthTextureFilePath, std::string fifthTextureFilePath, 
			std::string sixthTextureFilePath, MultiSprite* multiSprite);
		~OCube();

		float getCollisionRadius() { return mSideLength; }

		void renderEntity();
	private:
		float mSideLength;
		std::vector<std::string> mTextures;
		MultiSprite* mMultiSprite;
	};

}