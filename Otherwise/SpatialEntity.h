#pragma once
#include"glm/glm.hpp"

namespace Otherwise
{

	enum FrustumCullingType { RADIUS };

	class SpatialEntity
	{
	public:
		SpatialEntity();
		~SpatialEntity();

		glm::vec3 getPosition() { return mPosition; }
		FrustumCullingType getCollisionType() { return mCollisionType; }
		virtual float getCollisionRadius();
		virtual void renderEntity();

	protected:
		glm::vec3 mPosition;
		FrustumCullingType mCollisionType = FrustumCullingType::RADIUS;
	};

}