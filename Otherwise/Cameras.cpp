#include "Cameras.h"
#include<iostream>
namespace Otherwise
{
	Camera2D::Camera2D(int screenWidth, int screenHeight, ovec2 position, float scale) : mScreenWidth(screenWidth), mScreenHeight(screenHeight), mPosition(position), mScale(scale)
	{
		mOrthoMatrix = orthographic_matrix(0.0f, (float)mScreenWidth, 0.0f, (float)mScreenHeight);
		mOrthoProjection = translate_matrix(mOrthoMatrix, ovec3(mPosition.vect.x, mPosition.vect.y, 0.0f));
		ovec3 scaling(mScale, mScale, 0.0f);
		mOrthoProjection = scale_matrix(omat4(1.0f), scaling) * mOrthoProjection;
	}

	Camera2D::~Camera2D()
	{
	}

	void Camera2D::init(int screenWidth, int screenHeight, ovec2 position, float scale)
	{
		mScreenWidth = screenWidth;
		mScreenHeight = screenHeight;
		mPosition = position;
		mScale = scale;
		mOrthoMatrix = orthographic_matrix(0.0f, (float)mScreenWidth, 0.0f, (float)mScreenHeight);
		mOrthoProjection = translate_matrix(mOrthoMatrix, ovec3(mPosition.vect.x, mPosition.vect.y, 0.0f));
		ovec3 scaling(mScale, mScale, 0.0f);
		mOrthoProjection = scale_matrix(omat4(1.0f), scaling) * mOrthoProjection;
	}

	Camera3D::Camera3D(int screenWidth, int screenHeight, ovec3 position, 
		float fieldOfView, float near, float far, ovec3 cameraRoll, 
		CorrespondentManager *corrManager, float hAngle, float vAngle)
		: mPosition(position), mScreenRatio((float)screenWidth / (float)screenHeight),
		mNearClippingDistance(near), mFarClippingDistance(far), 
		mFieldOfView(fieldOfView), mCameraRoll(cameraRoll), mHAngle(hAngle), 
		mVangle(vAngle)
	{
		mLookAtPosition = ovec3(cos(mVangle) * sin(mHAngle), sin(mVangle),
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


	void Camera3D::changePosition(ovec3 newPosition)
	{
		mPosition = newPosition;
		update();
	}

	void Camera3D::createFrustum()
	{
		mFrustum.planes.clear();
		float nearFrustumHeight = tangent(mFieldOfView/2.0f) * mNearClippingDistance;
		float nearFrustumWidth = nearFrustumHeight * mScreenRatio;
		float farFrustumHeight = tangent(mFieldOfView/2.0f) * mFarClippingDistance;
		float farFrustumWidth = farFrustumHeight * mScreenRatio;
		ovec3 farClippingCenter = mPosition + mLookAtPosition * mFarClippingDistance;
		ovec3 nearClippingCenter = mPosition + mLookAtPosition * mNearClippingDistance;

		ovec3 cameraRight = normalize_vect(cross_product(mLookAtPosition, mCameraRoll));
		ovec3 cameraUp = normalize_vect(cross_product(mLookAtPosition, cameraRight));
		
		//near clipping frustum plane
		mFrustum.planes.push_back(OCollPlane(mLookAtPosition, dot_product(mLookAtPosition.inverse(), nearClippingCenter)));
		/*std::cout << "Near plane normal: " << mFrustum.planes.back().normal.x
			<< ", " << mFrustum.planes.back().normal.y << ". "
			<< mFrustum.planes.back().normal.z << " has offset: "
			<< mFrustum.planes.back().offset << std::endl;*/
		//far clipping frustum plane
		mFrustum.planes.push_back(OCollPlane(mLookAtPosition.inverse(), dot_product(mLookAtPosition, farClippingCenter)));
		/*std::cout << "Far plane normal: " << mFrustum.planes.back().normal.x
			<< ", " << mFrustum.planes.back().normal.y << ". "
			<< mFrustum.planes.back().normal.z << " has offset: "
			<< mFrustum.planes.back().offset << std::endl;*/
		//right clipping frustum plane
		ovec3 planeNormal = normalize_vect(cross_product(cameraUp, (farClippingCenter + cameraRight * farFrustumWidth) - mPosition)).inverse();
		mFrustum.planes.push_back(OCollPlane(planeNormal, dot_product(planeNormal.inverse(), mPosition)));
		/*std::cout << "Right plane normal: " << mFrustum.planes.back().normal.x
			<< ", " << mFrustum.planes.back().normal.y << ". "
			<< mFrustum.planes.back().normal.z << " has offset: "
			<< mFrustum.planes.back().offset << std::endl;*/
		//left clipping frustum plane
		planeNormal = normalize_vect(cross_product(cameraUp, (farClippingCenter - cameraRight * farFrustumWidth) - mPosition));
		mFrustum.planes.push_back(OCollPlane(planeNormal, dot_product(planeNormal.inverse(), mPosition)));
		/*std::cout << "Left plane normal: " << mFrustum.planes.back().normal.x
			<< ", " << mFrustum.planes.back().normal.y << ". "
			<< mFrustum.planes.back().normal.z << " has offset: "
			<< mFrustum.planes.back().offset << std::endl;*/
		//top clipping frustum plane
		planeNormal = normalize_vect(cross_product(cameraRight, (farClippingCenter - cameraUp * farFrustumHeight) - mPosition)).inverse();
		mFrustum.planes.push_back(OCollPlane(planeNormal, dot_product(planeNormal.inverse(), mPosition)));
		/*std::cout << "Top plane normal: " << mFrustum.planes.back().normal.x
			<< ", " << mFrustum.planes.back().normal.y << ". "
			<< mFrustum.planes.back().normal.z << " has offset: "
			<< mFrustum.planes.back().offset << std::endl;*/
		//bottom clipping frustum plane
		planeNormal = normalize_vect(cross_product(cameraRight, (farClippingCenter + cameraUp * farFrustumHeight) - mPosition));
		mFrustum.planes.push_back(OCollPlane(planeNormal, dot_product(planeNormal.inverse(), mPosition)));
		/*std::cout << "Bottom plane normal: " << mFrustum.planes.back().normal.x
			<< ", " << mFrustum.planes.back().normal.y << ". "
			<< mFrustum.planes.back().normal.z << " has offset: "
			<< mFrustum.planes.back().offset << std::endl;*/
	}

	void Camera3D::update()
	{

		if (mLookAtReciever.getMessage())
		{
			ovec2 mouseCoordsChange = mLookAtReciever.getMouseMessage();
			mHAngle -= mouseCoordsChange.vect.x * mMouseSensitivity;
			mVangle -= mouseCoordsChange.vect.y * mMouseSensitivity;
			mLookAtPosition = ovec3(cos(mVangle) * sin(mHAngle), sin(mVangle), cos(mVangle) * cos(mHAngle));
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
			mPosition += cross_product(ovec3(0.0f, 1.0f, 0.0f), mLookAtPosition) * mLeft;
			mLeftReciever.clearMessage();
			createFrustum();
		}

		if (mRightReciever.getMessage())
		{
			mPosition += cross_product(ovec3(0.0f, 1.0f, 0.0f), mLookAtPosition) * mRight;
			mRightReciever.clearMessage();
			createFrustum();
		}

		if (mUpReciever.getMessage())
		{
			mPosition += ovec3(0.0f, mUp, 0.0f);
			mUpReciever.clearMessage();
			createFrustum();
		}

		if (mDownReciever.getMessage())
		{
			mPosition += ovec3(0.0f, mDown, 0.0f);
			mDownReciever.clearMessage();
			createFrustum();
		}

		mProjectionMatrix = perspective_matrix(mFieldOfView, mScreenRatio, mNearClippingDistance, mFarClippingDistance);
		mCameraMatrix = look_at_view_matrix(mPosition, mPosition + mLookAtPosition, ovec3(0.0f, 1.0f, 0.0f));
		mModelMatrix = omat4(1.0f);
		mPerspectiveProjection = mProjectionMatrix * mCameraMatrix * mModelMatrix;
	}

}