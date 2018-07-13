#pragma once
#include"glm/glm.hpp"
#include"glm/gtc/matrix_transform.hpp"

namespace Otherwise
{
	class Camera2D
	{
	public:
		Camera2D(int screenWidth, int screenHeight, glm::vec2 position, float scale);
		~Camera2D();

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
		Camera3D(int screenWidth, int screenHeight, glm::vec3 position, glm::vec3 lookAt, float fieldOfView, float near, float far);
		~Camera3D();

		glm::mat4 getMatrix() { return mPerspectiveProjection; }

	private:
		int mScreenWidth, mScreenHeight;
		glm::vec3 mPosition;
		glm::mat4 mCameraMatrix;
		glm::mat4 mModelMatrix;
		glm::mat4 mProjectionMatrix;
		glm::mat4 mPerspectiveProjection;
	};
}