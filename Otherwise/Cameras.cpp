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

	Camera3D::Camera3D(int screenWidth, int screenHeight, glm::vec3 position, glm::vec3 lookAt, float fieldOfView, float near, float far, glm::vec3 cameraRoll) 
		:  mPosition(position), mLookAtPosition(lookAt), mScreenRatio((float)screenWidth / (float)screenHeight), mNearClippingDistance(near), 
		mFarClippingDistance(far), mFieldOfView(fieldOfView), mCameraRoll(cameraRoll)
	{
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
		mFrustumPlanes.clear();
		float nearFrustumHeight = glm::tan(mFieldOfView/2) * mNearClippingDistance;
		float nearFrustumWidth = nearFrustumHeight * mScreenRatio;
		float farFrustumHeight = glm::tan(mFieldOfView/2) * mFarClippingDistance;
		float farFrustumWidth = farFrustumHeight * mScreenRatio;

		glm::vec3 viewDirection = glm::normalize(mLookAtPosition - mPosition);
		glm::vec3 farClippingCenter = mPosition + viewDirection * mFarClippingDistance;
		glm::vec3 nearClippingCenter = mPosition + viewDirection * mNearClippingDistance;

		glm::vec3 cameraRight = glm::normalize(glm::cross(viewDirection, mCameraRoll));
		glm::vec3 cameraUp = glm::normalize(glm::cross(viewDirection, cameraRight));
		
		//near clipping frustum plane
		mFrustumPlanes.push_back(Plane(viewDirection, glm::dot(-viewDirection, nearClippingCenter)));

		//far clipping frustum plane
		mFrustumPlanes.push_back(Plane(-viewDirection, glm::dot(viewDirection, farClippingCenter)));

		//right clipping frustum plane
		glm::vec3 planeNormal = -glm::normalize(glm::cross(cameraUp, (farClippingCenter + cameraRight * farFrustumWidth) - mPosition));
		mFrustumPlanes.push_back(Plane(planeNormal, glm::dot(-planeNormal, mPosition)));

		//left clipping frustum plane
		planeNormal = glm::normalize(glm::cross(cameraUp, (farClippingCenter - cameraRight * farFrustumWidth) - mPosition));
		mFrustumPlanes.push_back(Plane(planeNormal, glm::dot(-planeNormal, mPosition)));

		//top clipping frustum plane
		planeNormal = -glm::normalize(glm::cross(cameraRight, (farClippingCenter - cameraUp * farFrustumHeight) - mPosition));
		mFrustumPlanes.push_back(Plane(planeNormal, glm::dot(-planeNormal, mPosition)));

		//bottom clipping frustum plane
		planeNormal = glm::normalize(glm::cross(cameraRight, (farClippingCenter + cameraUp * farFrustumHeight) - mPosition));
		mFrustumPlanes.push_back(Plane(planeNormal, glm::dot(-planeNormal, mPosition)));
		mFrustum = true;
	}

	bool Camera3D::isSphereInView(glm::vec3 centerPoint, float radius)
	{
		if (!mFrustum)
		{
			createFrustum();
		}
		if (mFrustumPlanes[0].getDistance(centerPoint) < -radius)
		{
			return false;
		}
		if (mFrustumPlanes[1].getDistance(centerPoint) < -radius)
		{
			return false;
		}
		if (mFrustumPlanes[2].getDistance(centerPoint) < -radius)
		{
			return false;
		}
		if (mFrustumPlanes[3].getDistance(centerPoint) < -radius)
		{
			return false;
		}
		if (mFrustumPlanes[4].getDistance(centerPoint) < -radius)
		{
			return false;
		}
		if (mFrustumPlanes[5].getDistance(centerPoint) < -radius)
		{
			return false;
		}
		return true;
	}

	bool Camera3D::isBoxInView(std::vector<glm::vec3> points)
	{
		bool isThereAGoodPoint = false;
		unsigned int iterator = 0;

		if (!mFrustum)
		{
			createFrustum();
		}

		for (size_t i = 0; i < mFrustumPlanes.size(); i++)
		{
			while (!isThereAGoodPoint && iterator < 8)
			{
				if (mFrustumPlanes[i].getDistance(points[iterator]) > 0)
				{
					isThereAGoodPoint = true;
				}
				iterator++;
				std::cout << "Point " << iterator << " in relation to plane " << i << '\n';
			}
			std::cout << '\n';
			if (!isThereAGoodPoint)
			{
				std::cout << "false" << '\n';
				return false;
			}

			isThereAGoodPoint = false;
			iterator = 0;
		}
		std::cout << "true" << '\n';
		return true;
	}

	void Camera3D::update()
	{
		mProjectionMatrix = glm::perspective(glm::radians(mFieldOfView), mScreenRatio, mNearClippingDistance, mFarClippingDistance);
		mCameraMatrix = glm::lookAt(mPosition, mLookAtPosition, glm::vec3(0.0f, 1.0f, 0.0f));
		mModelMatrix = glm::mat4(1.0f);
		mPerspectiveProjection = mProjectionMatrix * mCameraMatrix * mModelMatrix;
	}

}