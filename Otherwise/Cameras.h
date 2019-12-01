#pragma once
#include"MathLibraryInterface.h"
#include <vector>
#include "MessagingSystem.h"
#include "OCollision.h"

namespace Otherwise
{
	class Camera2D
	{
	public:
		Camera2D() {}
		Camera2D(int screenWidth, int screenHeight, ovec2 position, float scale);
		~Camera2D();

		void init(int screenWidth, int screenHeight, ovec2 position, float scale);

		omat4 getMatrix() { return mOrthoProjection; }

	private:
		int mScreenWidth, mScreenHeight;
		ovec2 mPosition;
		omat4 mOrthoProjection;
		float mScale;
		omat4 mOrthoMatrix;
	};

	class Camera3D
	{
	public:
		Camera3D(int screenWidth, int screenHeight, ovec3 position, float fieldOfView, float near, float far, ovec3 cameraRoll, CorrespondentManager *corrManager, float hAngle, float vAngle);
		~Camera3D();
		omat4 getPerspectiveMatrix() { return mPerspectiveProjection; }
		omat4 getModelMatrix() { return mModelMatrix; }
		omat4 getCameraMatrix() { return mCameraMatrix; }
		omat4 getProjectionMatrix() { return mProjectionMatrix; }
		omat4 getModelCameraMatrix() { return mModelMatrix * mCameraMatrix; }

		ovec3 getPosition() { return mPosition; }
		void changePosition(ovec3 newPosition);
		void createFrustum();
	
		void update();

		OCollCnvx* getFrustum() { return &mFrustum; }
	private:
		OCollCnvx mFrustum;

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

		float mMouseSensitivity = 0.005f;

		Correspondent mUpReciever;
		Correspondent mDownReciever;
		Correspondent mLeftReciever;
		Correspondent mRightReciever;
		Correspondent mForwardReciever;
		Correspondent mBackReciever;
		Correspondent mLookAtReciever;

		ovec3 mCameraRoll; 
		ovec3 mLookAtPosition;
		ovec3 mPosition;
		omat4 mCameraMatrix;
		omat4 mModelMatrix;
		omat4 mProjectionMatrix;
		omat4 mPerspectiveProjection;
	};
}