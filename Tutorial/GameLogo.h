#pragma once

#include "Cameras.h"
#include "MultiSprite.h"
#include "SimpleShader.h"
#include "ImageLoad.h"
#include "Window.h"

#include <math.h>

class GameLogo
{
public:
	GameLogo();
	~GameLogo();

	void init(std::string vertShader, std::string fragShader, int screenWidth, int screenHeight, glm::vec2 cameraPosition, float cameraZoom, Otherwise::Window* windowptr);

	void logoUpdateRenderLoop();

	void render();
private:
	Otherwise::Camera2D mCamera;
	Otherwise::MultiSprite mMultiSprite;
	GLuint mProgramID;
	GLint mPerspectiveUniformID;
	glm::mat4 mOrthographicMatrix;
	Otherwise::Window* mWindow;
	float mScreenWidth, mScreenHeight;
};
