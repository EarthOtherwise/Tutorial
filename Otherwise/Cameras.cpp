#include "Cameras.h"

namespace Otherwise
{



	Camera2D::Camera2D(int screenWidth, int screenHeight, glm::vec2 position, float scale) : mScreenWidth(screenWidth), mScreenHeight(screenHeight), mPosition(position), mScale(scale)
	{
		mOrthoMatrix = glm::ortho(0.0f, (float)mScreenWidth, 0.0f, (float)mScreenHeight);
		mOrthoProjection = glm::translate(mOrthoMatrix, glm::vec3(mPosition.x, mPosition.y, 0.0f));
		glm::vec3 scaling(mScale, mScale, 0.0f);
		mOrthoProjection = glm::scale(glm::mat4(1.0f), scaling) * mOrthoProjection;
	}

	Camera2D::~Camera2D()
	{
	}

	Camera3D::Camera3D(int screenWidth, int screenHeight, glm::vec3 position, glm::vec3 lookAt, float fieldOfView, float near, float far) 
		: mScreenWidth(screenWidth), mScreenHeight(screenHeight), mPosition(position)
	{
		mProjectionMatrix = glm::perspective(glm::radians(fieldOfView), (float)mScreenWidth / (float)mScreenHeight, near, far);
		mCameraMatrix = glm::lookAt(mPosition, lookAt, glm::vec3(0.0f, 1.0f, 0.0f));
		mModelMatrix = glm::mat4(1.0f);
		mPerspectiveProjection = mProjectionMatrix * mCameraMatrix * mModelMatrix;
	}

	Camera3D::~Camera3D()
	{
	}

}