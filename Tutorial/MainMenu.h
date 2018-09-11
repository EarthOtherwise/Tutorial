#pragma once
#include "GraphicsResourceManager.h"
#include "GUI.h"
#include "CEGUI/CEGUI.h"
#include "Window.h"
#include "Cameras.h"

class MainMenu
{
public:
	MainMenu();
	~MainMenu();

	void init(Otherwise::GraphicsResourceManager* GRM, std::string vertShader, std::string fragShader, 
		int screenWidth, int screenHeight, glm::vec2 cameraPosition, float cameraZoom, 
		Otherwise::Window* windowptr, Otherwise::GUI* gui);

	void mainMenuLoop();
	void render();
	bool exitGUI(const CEGUI::EventArgs &e);
	bool beginGame(const CEGUI::EventArgs &e);
private:
	Otherwise::GraphicsResourceManager* mGRM;
	Otherwise::GUI* mGUI;
	bool mStartGame = false;
	Otherwise::Camera2D mCamera;
	GLuint mProgramID;
	GLint mPerspectiveUniformID;
	glm::mat4 mOrthographicMatrix;
	Otherwise::Window* mWindow;
	float mScreenWidth, mScreenHeight;
};

