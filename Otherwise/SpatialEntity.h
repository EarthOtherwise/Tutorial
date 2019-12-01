#pragma once
#include"OCollision.h"
#include"MathLibraryInterface.h"
#include<GL\glew.h>

namespace Otherwise
{

	enum FrustumCullingType { RADIUS };

	class SpatialEntity
	{
	public:
		SpatialEntity();
		~SpatialEntity();

		ovec3 getPosition() { return mPosition; }
		FrustumCullingType getCollisionType() { return mCollisionType; }
		virtual float getCollisionRadius();
		virtual void renderEntity();
		virtual OCollAABB* getBox() { OCollAABB *blank = nullptr;  return blank; }
		virtual void changeTextures(GLuint firstTextureFilePath, GLuint secondTextureFilePath,
			GLuint thirdTextureFilePath, GLuint fourthTextureFilePath, GLuint fifthTextureFilePath,
			GLuint sixthTextureFilePath) { return; }

	protected:
		ovec3 mPosition;
		FrustumCullingType mCollisionType = FrustumCullingType::RADIUS;
	};

}