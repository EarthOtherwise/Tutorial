#include "Cameras.h"
#include<iostream>
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

	void Camera2D::init(int screenWidth, int screenHeight, glm::vec2 position, float scale)
	{
		mScreenWidth = screenWidth;
		mScreenHeight = screenHeight;
		mPosition = position;
		mScale = scale;
		mOrthoMatrix = glm::ortho(0.0f, (float)mScreenWidth, 0.0f, (float)mScreenHeight);
		mOrthoProjection = glm::translate(mOrthoMatrix, glm::vec3(mPosition.x, mPosition.y, 0.0f));
		glm::vec3 scaling(mScale, mScale, 0.0f);
		mOrthoProjection = glm::scale(glm::mat4(1.0f), scaling) * mOrthoProjection;
	}

	Camera3D::Camera3D(int screenWidth, int screenHeight, glm::vec3 position, 
		float fieldOfView, float near, float far, glm::vec3 cameraRoll, 
		CorrespondentManager *corrManager, float hAngle, float vAngle)
		: mPosition(position), mScreenRatio((float)screenWidth / (float)screenHeight),
		mNearClippingDistance(near), mFarClippingDistance(far), 
		mFieldOfView(fieldOfView), mCameraRoll(cameraRoll), mHAngle(hAngle), 
		mVangle(vAngle)
	{
		mLookAtPosition = glm::vec3(cos(mVangle) * sin(mHAngle), sin(mVangle),
			cos(mVangle) * cos(mHAngle));

		mUpReciever.init(corrManager, (std::string)"CameraMoveUpReciever");
		mDownReciever.init(corrManager, (std::string)"CameraMoveDownReciever");
		mLeftReciever.init(corrManager, (std::string)"CameraMoveLeftReciever");
		mRightReciever.init(corrManager, (std::string)"CameraMoveRightReciever");
		mForwardReciever.init(corrManager, (std::string)"CameraMoveForwardReciever");
		mBackReciever.init(corrManager, (std::string)"CameraMoveBackReciever");
		mLookAtReciever.init(corrManager, (std::string)"CameraLookAtReciever");

		mFrustum.pos = &mPosition;
		createFrustum();

		update();
	}

	Camera3D::~Camera3D()
	{
	}


	void Camera3D::changePosition(glm::vec3 newPosition)
	{
		mPosition = newPosition;
		update();
	}

	void Camera3D::createFrustum()
	{
		mFrustum.planes.clear();
		float nearFrustumHeight = glm::tan(mFieldOfView/2.0f) * mNearClippingDistance;
		float nearFrustumWidth = nearFrustumHeight * mScreenRatio;
		float farFrustumHeight = glm::tan(mFieldOfView/2.0f) * mFarClippingDistance;
		float farFrustumWidth = farFrustumHeight * mScreenRatio;
		glm::vec3 farClippingCenter = mPosition + mLookAtPosition * mFarClippingDistance;
		glm::vec3 nearClippingCenter = mPosition + mLookAtPosition * mNearClippingDistance;

		glm::vec3 cameraRight = glm::normalize(glm::cross(mLookAtPosition, mCameraRoll));
		glm::vec3 cameraUp = glm::normalize(glm::cross(mLookAtPosition, cameraRight));
		
		//near clipping frustum plane
		mFrustum.planes.push_back(OCollPlane(mLookAtPosition, glm::dot(-mLookAtPosition, nearClippingCenter)));
		/*std::cout << "Near plane normal: " << mFrustum.planes.back().normal.x
			<< ", " << mFrustum.planes.back().normal.y << ". "
			<< mFrustum.planes.back().normal.z << " has offset: "
			<< mFrustum.planes.back().offset << std::endl;*/
		//far clipping frustum plane
		mFrustum.planes.push_back(OCollPlane(-mLookAtPosition, glm::dot(mLookAtPosition, farClippingCenter)));
		/*std::cout << "Far plane normal: " << mFrustum.planes.back().normal.x
			<< ", " << mFrustum.planes.back().normal.y << ". "
			<< mFrustum.planes.back().normal.z << " has offset: "
			<< mFrustum.planes.back().offset << std::endl;*/
		//right clipping frustum plane
		glm::vec3 planeNormal = -glm::normalize(glm::cross(cameraUp, (farClippingCenter + cameraRight * farFrustumWidth) - mPosition));
		mFrustum.planes.push_back(OCollPlane(planeNormal, glm::dot(-planeNormal, mPosition)));
		/*std::cout << "Right plane normal: " << mFrustum.planes.back().normal.x
			<< ", " << mFrustum.planes.back().normal.y << ". "
			<< mFrustum.planes.back().normal.z << " has offset: "
			<< mFrustum.planes.back().offset << std::endl;*/
		//left clipping frustum plane
		planeNormal = glm::normalize(glm::cross(cameraUp, (farClippingCenter - cameraRight * farFrustumWidth) - mPosition));
		mFrustum.planes.push_back(OCollPlane(planeNormal, glm::dot(-planeNormal, mPosition)));
		/*std::cout << "Left plane normal: " << mFrustum.planes.back().normal.x
			<< ", " << mFrustum.planes.back().normal.y << ". "
			<< mFrustum.planes.back().normal.z << " has offset: "
			<< mFrustum.planes.back().offset << std::endl;*/
		//top clipping frustum plane
		planeNormal = -glm::normalize(glm::cross(cameraRight, (farClippingCenter - cameraUp * farFrustumHeight) - mPosition));
		mFrustum.planes.push_back(OCollPlane(planeNormal, glm::dot(-planeNormal, mPosition)));
		/*std::cout << "Top plane normal: " << mFrustum.planes.back().normal.x
			<< ", " << mFrustum.planes.back().normal.y << ". "
			<< mFrustum.planes.back().normal.z << " has offset: "
			<< mFrustum.planes.back().offset << std::endl;*/
		//bottom clipping frustum plane
		planeNormal = glm::normalize(glm::cross(cameraRight, (farClippingCenter + cameraUp * farFrustumHeight) - mPosition));
		mFrustum.planes.push_back(OCollPlane(planeNormal, glm::dot(-planeNormal, mPosition)));
		/*std::cout << "Bottom plane normal: " << mFrustum.planes.back().normal.x
			<< ", " << mFrustum.planes.back().normal.y << ". "
			<< mFrustum.planes.back().normal.z << " has offset: "
			<< mFrustum.planes.back().offset << std::endl;*/
	}

	void Camera3D::update()
	{

		if (mLookAtReciever.getMessage())
		{
			glm::vec2 mouseCoordsChange = mLookAtReciever.getMouseMessage();
			mHAngle -= mouseCoordsChange.x * mMouseSensitivity;
			mVangle -= mouseCoordsChange.y * mMouseSensitivity;
			mLookAtPosition = glm::vec3(cos(mVangle) * sin(mHAngle), sin(mVangle), cos(mVangle) * cos(mHAngle));
			mLookAtReciever.clearMessage();
			createFrustum();
		}

		if (mForwardReciever.getMessage())
		{
			mPosition += mLookAtPosition * mForward;
			mForwardReciever.clearMessage();
			createFrustum();
		}

		if (mBackReciever.getMessage())
		{
			mPosition += mLookAtPosition * mBack;
			mBackReciever.clearMessage();
			createFrustum();
		}

		if (mLeftReciever.getMessage())
		{
			mPosition += mLeft * glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), mLookAtPosition);
			mLeftReciever.clearMessage();
			createFrustum();
		}

		if (mRightReciever.getMessage())
		{
			mPosition += mRight * glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), mLookAtPosition);
			mRightReciever.clearMessage();
			createFrustum();
		}

		if (mUpReciever.getMessage())
		{
			mPosition += glm::vec3(0.0f, mUp, 0.0f);
			mUpReciever.clearMessage();
			createFrustum();
		}

		if (mDownReciever.getMessage())
		{
			mPosition += glm::vec3(0.0f, mDown, 0.0f);
			mDownReciever.clearMessage();
			createFrustum();
		}

		mProjectionMatrix = glm::perspective(glm::radians(mFieldOfView), mScreenRatio, mNearClippingDistance, mFarClippingDistance);
		mCameraMatrix = glm::lookAt(mPosition, mPosition + mLookAtPosition, glm::vec3(0.0f, 1.0f, 0.0f));
		mModelMatrix = glm::mat4(1.0f);
		mPerspectiveProjection = mProjectionMatrix * mCameraMatrix * mModelMatrix;
	}

}