#pragma once
#include"glm/glm.hpp"
#include"glm/gtc/matrix_transform.hpp"
#include <vector>
#include "MessagingSystem.h"

namespace Otherwise
{
	class Plane
	{
	public:
		Plane() {}
		Plane(glm::vec3 normal, float originDistance) : mNormal(normal), mOriginDistance(originDistance) {}
		~Plane() {}

		float getDistance(glm::vec3 position)
		{
			return glm::dot(mNormal, position) + mOriginDistance;
		}
	private:
		glm::vec3 mNormal;
		float mOriginDistance;
	};
	class Camera2D
	{
	public:
		Camera2D() {}
		Camera2D(int screenWidth, int screenHeight, glm::vec2 position, float scale);
		~Camera2D();

		void init(int screenWidth, int screenHeight, glm::vec2 position, float scale);

		glm::mat4 getMatrix() { return mOrthoProjection; }

	private:
		int mScreenWidth, mScreenHeight;
		glm::vec2 mPosition;
		glm::mat4 mOrthoProjection;
		float mScale;
		glm::mat4 mOrthoMatrix;
	};

	class Camera3D
	{
	public:
		Camera3D(int screenWidth, int screenHeight, glm::vec3 position, float fieldOfView, float near, float far, glm::vec3 cameraRoll, CorrespondentManager *corrManager, float hAngle, float vAngle);
		~Camera3D();
		glm::mat4 getPerspectiveMatrix() { return mPerspectiveProjection; }
		glm::mat4 getModelMatrix() { return mModelMatrix; }
		glm::mat4 getCameraMatrix() { return mCameraMatrix; }
		glm::mat4 getModelCameraMatrix() { return mModelMatrix * mCameraMatrix; }

		glm::vec3 getPosition() { return mPosition; }
		void changePosition(glm::vec3 newPosition);
		void createFrustum();
		bool isSphereInView(glm::vec3 centerPoint, float radius);
		bool isBoxInView(std::vector<glm::vec3> points);
	
		void update();
	private:
		bool mFrustum = false;

		std::vector<Plane> mFrustumPlanes;

		float mScreenRatio;
		float mNearClippingDistance;
		float mFarClippingDistance;
		float mFieldOfView;

		float mForward = 0.5f;
		float mBack = -0.5f;
		float mLeft = 0.5f;
		float mRight = -0.5f;
		float mUp = 0.5f;
		float mDown = -0.5;

		float mVangle = 0.0f;
		float mHAngle = 0.0f;

		float mMouseSensitivity = 0.005;

		Correspondent mUpReciever;
		Correspondent mDownReciever;
		Correspondent mLeftReciever;
		Correspondent mRightReciever;
		Correspondent mForwardReciever;
		Correspondent mBackReciever;
		Correspondent mLookAtReciever;

		glm::vec3 mCameraRoll; 
		glm::vec3 mLookAtPosition;
		glm::vec3 mPosition;
		glm::mat4 mCameraMatrix;
		glm::mat4 mModelMatrix;
		glm::mat4 mProjectionMatrix;
		glm::mat4 mPerspectiveProjection;
	};
}