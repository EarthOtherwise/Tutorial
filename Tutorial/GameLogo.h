#pragma once

#include "Cameras.h"
#include "SimpleShader.h"
#include "OSInterface.h"

#include "GraphicsInterface.h"

#include <math.h>

/*class GameLogo
{
public:
	GameLogo();
	~GameLogo();

	void init(std::string vertShader, std::string fragShader, int screenWidth, int screenHeight, glm::vec2 cameraPosition, float cameraZoom, Otherwise::OSInterface* windowptr, Otherwise::GraphicsResourceManager* graphics);

	void logoUpdateRenderLoop();

	void render();
private:
	Otherwise::Camera2D mCamera;
	Otherwise::GraphicsResourceManager* mGraphics;
	Otherwise::Shader simple_shader;
	std::string perspective = "Perspective";
	Otherwise::ShaderUniform texture_perspective;
	glm::mat4 mOrthographicMatrix;
	Otherwise::OSInterface* mWindow;
	float mScreenWidth, mScreenHeight;
};
*/