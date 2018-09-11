#include"InputHandler.h"
#include"Window.h"
#include"InitFileReadWrite.h"
#include<SDL/SDL.h>
#include"Triangle.h"
#include"SimpleShader.h"
#include"Square.h"
#include"MultiSprite.h"
#include"imageLoad.h"
#include"Cameras.h"
#include"SpriteFont.h"
#include"OCube.h"
#include"SpatialSceneGraphOct.h"
#include "GameLogo.h"
#include "GraphicsResourceManager.h"
#include "GUI.h"
#include "MainMenu.h"

/*temporarily include iostream*/
#include<iostream>

int main(int argc, char** argv)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	Otherwise::Window newWindow;
	newWindow.create("Window", 512, 512, 0);

	Otherwise::GraphicsResourceManager graphics;

	GameLogo gameLogo;
	gameLogo.init("Logo.vert", "Logo.frag", 512, 512, glm::vec2(0.0f, 0.0f), 1.0f, &newWindow, &graphics);
	gameLogo.logoUpdateRenderLoop();

	Otherwise::GUI gui;
	gui.init("GUI");

	MainMenu mainMenu;
	mainMenu.init(&graphics, "Logo.vert", "Logo.frag", 512, 512, glm::vec2(0.0f, 0.0f), 1.0f, &newWindow, &gui);
	mainMenu.mainMenuLoop();

	Otherwise::InputHandler newInput;

	bool doQuit = false;

	Otherwise::Square triangle("Textures/apple.png");
	Otherwise::Triangle square;

	GLuint programID = Otherwise::compileLinkSimpleShaders("TextureVert.vert", "TextureFrag.frag");
	GLuint secondProgramID = Otherwise::compileLinkSimpleShaders("SimpleVert.vert", "SimpleFrag.frag");

	GLint shaderPerspectiveID = glGetUniformLocation(programID, "Perspective");
	GLint shaderCameraMatrixID = glGetUniformLocation(programID, "CameraMatrix");
	GLint shaderModelMatrixID = glGetUniformLocation(programID, "ModelMatrix");
	GLint shaderLightPositionID = glGetUniformLocation(programID, "LightPosition");
	GLint shaderModelCameraMatrixID = glGetUniformLocation(programID, "ModelCameraMatrix");
	GLint shaderLightColourID = glGetUniformLocation(programID, "LightColour");
	GLint shaderLightIntensityID = glGetUniformLocation(programID, "LightIntensity");

	Otherwise::MultiSprite multiSprite3;
	multiSprite3.init();

	Otherwise::SpriteFont spriteFont("Fonts/chintzy.ttf", 64);
	char buffer[256];
	sprintf_s(buffer, "Test String");
	spriteFont.draw(multiSprite3, buffer, glm::vec2(0.0f, 0.0f), glm::vec2(0.01f), 0.0f, Otherwise::ColourRGBA8(255, 255, 255, 255));

	Otherwise::Camera2D camera2D (512, 512, glm::vec2(0.0f, 0.0f), 1.0f);
	glm::mat4 ortho = camera2D.getMatrix();
	Otherwise::Camera3D camera3D (512, 512, glm::vec3(53.0f, 53.0f, 53.0f), glm::vec3(50.0f, 50.0f, 50.0f), 40.0f, 2.0f, 30.0f, glm::vec3 (0.0f, 1.0f, 0.0f));
	glm::mat4 perspective = camera3D.getPerspectiveMatrix();
	glm::mat4 modelMatrix = camera3D.getModelMatrix();
	glm::mat4 cameraMatrix = camera3D.getCameraMatrix();
	glm::mat4 modelCameraMatrix = modelMatrix * cameraMatrix;

	Otherwise::MultiSprite multiSprite;
	multiSprite.init();

	multiSprite.addSprite(Otherwise::loadPng("Textures/apple.png"), 0.0f,
		Otherwise::Vertex (0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, Otherwise::ColourRGBA8(255, 100, 100, 255)), 
		Otherwise::Vertex(0.0f, 100.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, Otherwise::ColourRGBA8(255, 100, 100, 255)),
		Otherwise::Vertex(100.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, Otherwise::ColourRGBA8(255, 100, 100, 255)), 
		Otherwise::Vertex(100.0f, 100.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, Otherwise::ColourRGBA8(255, 100, 100, 255)));

	multiSprite.prepareBatches();


	Otherwise::MultiSprite multiSprite2;
	multiSprite2.init();

	std::vector<Otherwise::SpatialSceneGraphOct*> sceneGraphOctStack;
	std::vector<Otherwise::SpatialEntity*> spatialEntityStack;
	float minSpace = -100.0f;
	float maxSpace = 100.0f;
	Otherwise::SpatialSceneGraphOct spatialGraph(minSpace, maxSpace, minSpace, maxSpace, minSpace, maxSpace);
	spatialGraph.createChildren();
	sceneGraphOctStack.push_back(&spatialGraph);

	std::vector<Otherwise::OCube> cubes;

	for (float i = 10; i < 50; i += 5)
	{
		for (float j = 10; j < 50; j += 5)
		{
			for (float k = 10; k < 50; k += 5)
			{
				spatialGraph.addEntityToGraph(new Otherwise::OCube(glm::vec3(i, j, k), 1.0f, "Textures/red.png", "Textures/green.png", "Textures/blue.png",
					"Textures/purple.png", "Textures/orange.png", "Textures/yellow.png", &multiSprite2));
			}
		}
	}
	for (unsigned int i = 0; i < sceneGraphOctStack.size(); i++)
	{
		if (camera3D.isBoxInView(sceneGraphOctStack[i]->getPoints()))
		{
			sceneGraphOctStack[i]->addToStack(&spatialEntityStack, &sceneGraphOctStack);
		}
	}

	for (unsigned int i = 0; i < spatialEntityStack.size(); i++)
	{
		if (camera3D.isSphereInView(spatialEntityStack[i]->getPosition(), spatialEntityStack[i]->getCollisionRadius()))
		{
			spatialEntityStack[i]->renderEntity();
		}
	}

	multiSprite2.prepareBatches();
	multiSprite3.prepareBatches();

	while (!doQuit)
	{
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(programID);
		//triangle.textureDraw();
		glUniformMatrix4fv(shaderPerspectiveID, 1, GL_FALSE, &ortho[0][0]);
		//multiSprite3.renderBatches();
		//multiSprite.renderBatches();
		glUniformMatrix4fv(shaderPerspectiveID, 1, GL_FALSE, &perspective[0][0]);
		glUniformMatrix4fv(shaderModelMatrixID, 1, GL_FALSE, &modelMatrix[0][0]);
		glUniformMatrix4fv(shaderCameraMatrixID, 1, GL_FALSE, &cameraMatrix[0][0]);
		glUniformMatrix4fv(shaderModelCameraMatrixID, 1, GL_FALSE, &modelCameraMatrix[0][0]);
		glUniform3f(shaderLightPositionID, 2.0f, 0.5f, -1.5f);
		glUniform3f(shaderLightColourID, 1.0f, 1.0f, 1.0f);
		glUniform1f(shaderLightIntensityID, 5.0f);
		//multiSprite3.renderBatches();
		multiSprite2.renderBatches();
		glUseProgram(secondProgramID);
		//square.draw();
		glUseProgram(0);
		newInput.inputQueue();
		if (newInput.isKeyDown(SDLK_w))
		{
			doQuit = true;
		}
		if (newInput.isKeyDown(SDLK_k))
		{
			camera3D.changePosition(camera3D.getPosition() + glm::vec3(0.0f, 1.0f, 0.0f));
			perspective = camera3D.getPerspectiveMatrix();
			modelMatrix = camera3D.getModelMatrix();
			cameraMatrix = camera3D.getCameraMatrix();
			modelCameraMatrix = modelMatrix * cameraMatrix;
			newInput.unPressKey(SDLK_k);
		}
		newWindow.swapBuffer();

	}

	return 0;
}