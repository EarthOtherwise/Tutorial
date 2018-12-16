#include "SpatialSceneGraphOct.h"
#include"ErrHandler.h"

namespace Otherwise
{

	SpatialSceneGraphOct::SpatialSceneGraphOct()
	{
	}

	SpatialSceneGraphOct::SpatialSceneGraphOct(float minX, float maxX, float minY, float maxY, float minZ, float maxZ) : mMinX(minX), mMaxX(maxX), mMinY(minY), mMaxY(maxY), mMinZ(minZ), mMaxZ(maxZ)
	{
		if (mMaxX <= mMinX)
		{
			throwError("SpatialGraph", "Tried to create a spatial graph where the minimun X is greater thatn or equal to our Maximunm X.");
		}
		if (mMaxY <= mMinY)
		{
			throwError("SpatialGraph", "Tried to create a spatial graph where the minimun Y is greater thatn or equal to our Maximunm Y.");
		}
		if (mMaxZ <= mMinZ)
		{
			throwError("SpatialGraph", "Tried to create a spatial graph where the minimun Z is greater thatn or equal to our Maximunm Z.");
		}

		
		
		mCenter = getCenter();
		mBox.pos = &mCenter;
		mBox.maxOffset = glm::vec3(maxX, maxY, maxZ) - mCenter;
		mBox.minOffset = glm::vec3(minX, minY, minZ) - mCenter;
	}


	SpatialSceneGraphOct::~SpatialSceneGraphOct()
	{
	}

	void SpatialSceneGraphOct::addToStack(std::vector<SpatialEntity*>* entityStack, std::vector<SpatialSceneGraphOct*>* graphStack)
	{
		if (mOctsWithin.empty())
		{
			entityStack->insert(entityStack->end(), mEntitiesWithin.begin(), mEntitiesWithin.end());
		}
		else
		{
			graphStack->insert(graphStack->end(), mOctsWithin.begin(), mOctsWithin.end());
		}
	}

	bool SpatialSceneGraphOct::doesEntityBelongHere(SpatialEntity * entity)
	{
		if (entity->getPosition().x < mMinX)
		{
			return false;
		}
		if (entity->getPosition().x >= mMaxX)
		{
			return false;
		}

		if (entity->getPosition().y < mMinY)
		{
			return false;
		}
		if (entity->getPosition().y >= mMaxY)
		{
			return false;
		}

		if (entity->getPosition().z < mMinZ)
		{
			return false;
		}
		if (entity->getPosition().z >= mMaxZ)
		{
			return false;
		}

		return true;
	}

	void SpatialSceneGraphOct::addEntityToGraph(SpatialEntity * entity)
	{
		if (mOctsWithin.empty())
		{
			mEntitiesWithin.push_back(entity);
		}
		else
		{
			bool placedEntity = false;
			unsigned int iterator = 0;
			while (!placedEntity)
			{
				placedEntity = mOctsWithin[iterator]->doesEntityBelongHere(entity);
				if (!placedEntity)
				{
					iterator++;
					if (iterator > 7)
					{
						throwError("EntityPlacement", "Attempted to place an entity in an oct where it cannot exist.");
					}
				}
			}
			mOctsWithin[iterator]->addEntityToGraph(entity);
		}
	}

	glm::vec3 SpatialSceneGraphOct::getCenter()
	{
		float middleX = mMinX + (std::abs(mMaxX - mMinX) / 2);
		float middleY = mMinY + (std::abs(mMaxY - mMinY) / 2);
		float middleZ = mMinZ + (std::abs(mMaxZ - mMinZ) / 2);
		return glm::vec3(middleX, middleY, middleZ);
	}

	void SpatialSceneGraphOct::createChildren()
	{
		float middleX = mMinX + (std::abs(mMaxX - mMinX) / 2);
		float middleY = mMinY + (std::abs(mMaxY - mMinY) / 2);
		float middleZ = mMinZ + (std::abs(mMaxZ - mMinZ) / 2);

		mOctsWithin.push_back(new SpatialSceneGraphOct(mMinX, middleX, mMinY, middleY, mMinZ, middleZ));
		mOctsWithin.push_back(new SpatialSceneGraphOct(middleX, mMaxX, mMinY, middleY, mMinZ, middleZ));
		mOctsWithin.push_back(new SpatialSceneGraphOct(mMinX, middleX, middleY, mMaxY, mMinZ, middleZ));
		mOctsWithin.push_back(new SpatialSceneGraphOct(middleX, mMaxX, middleY, mMaxY, mMinZ, middleZ));
		mOctsWithin.push_back(new SpatialSceneGraphOct(mMinX, middleX, mMinY, middleY, middleZ, mMaxZ));
		mOctsWithin.push_back(new SpatialSceneGraphOct(middleX, mMaxX, mMinY, middleY, middleZ, mMaxZ));
		mOctsWithin.push_back(new SpatialSceneGraphOct(mMinX, middleX, middleY, mMaxY, middleZ, mMaxZ));
		mOctsWithin.push_back(new SpatialSceneGraphOct(middleX, mMaxX, middleY, mMaxY, middleZ, mMaxZ));
	}

	std::vector<glm::vec3> SpatialSceneGraphOct::getPoints()
	{
		std::vector<glm::vec3> points;

		points.push_back(glm::vec3(mMinX, mMinY, mMinZ));
		points.push_back(glm::vec3(mMaxX, mMinY, mMinZ));
		points.push_back(glm::vec3(mMinX, mMaxY, mMinZ));
		points.push_back(glm::vec3(mMaxX, mMaxY, mMinZ));
		points.push_back(glm::vec3(mMinX, mMinY, mMaxZ));
		points.push_back(glm::vec3(mMaxX, mMinY, mMaxZ));
		points.push_back(glm::vec3(mMinX, mMaxY, mMaxZ));
		points.push_back(glm::vec3(mMaxX, mMaxY, mMaxZ));

		return points;
	}
}