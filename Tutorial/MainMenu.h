#pragma once
#include "GUI.h"
#include "CEGUI/CEGUI.h"
#include "OSInterface.h"
#include "Cameras.h"
#include "InputHandler.h"

#include "GraphicsInterface.h"

/*class MainMenu
{
public:
	MainMenu();
	~MainMenu();

	void init(Otherwise::GraphicsResourceManager* GRM, std::string vertShader, std::string fragShader, 
		int screenWidth, int screenHeight, glm::vec2 cameraPosition, float cameraZoom, 
		Otherwise::OSInterface* windowptr, Otherwise::GUI* gui, Otherwise::InputHandler* input);

	void mainMenuLoop();
	void render();
	bool exitGUI(const CEGUI::EventArgs &e);
	bool beginGame(const CEGUI::EventArgs &e);
private:
	Otherwise::GraphicsResourceManager* mGRM;
	Otherwise::GUI* mGUI;
	bool mStartGame = false;
	Otherwise::Camera2D mCamera;
	Otherwise::Shader simple_shader;
	std::string perspective = "Perspective";
	Otherwise::ShaderUniform texture_perspective;
	glm::mat4 mOrthographicMatrix;
	Otherwise::OSInterface* mWindow;
	float mScreenWidth, mScreenHeight;
	Otherwise::InputHandler* mInput;
};

*/