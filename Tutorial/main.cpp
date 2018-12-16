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
#include "OAudio.h"
#include "MessagingSystem.h"
#include "OCollision.h"

/*temporarily include iostream*/
#include<iostream>

int main(int argc, char** argv)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	
	Otherwise::Window newWindow;
	newWindow.create("Window", 512, 512, 0);

	Otherwise::GraphicsResourceManager graphics;

	Otherwise::CorrespondentManager manager;
	manager.init();

	GameLogo gameLogo;
	gameLogo.init("Logo.vert", "Logo.frag", 512, 512, glm::vec2(0.0f, 0.0f), 1.0f, &newWindow, &graphics);
	gameLogo.logoUpdateRenderLoop();

	Otherwise::GUI gui;
	gui.init("GUI", &manager);

	Otherwise::InputHandler newInput;
	newInput.init(&manager);

	MainMenu mainMenu;
	mainMenu.init(&graphics, "Logo.vert", "Logo.frag", 512, 512, glm::vec2(0.0f, 0.0f), 1.0f, &newWindow, &gui, &newInput);
	mainMenu.mainMenuLoop();

	SDL_SetRelativeMouseMode(SDL_TRUE);

	Otherwise::OAudio audio;
	audio.init((std::string)"SoundMap1.txt", &manager);

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

	Otherwise::Camera3D camera3D(512, 512, glm::vec3(53.0f, 53.0f, 53.0f),
		45.0f, 1.0f, 100.0f, glm::vec3(0.0f, 1.0f, 0.0f), &manager, -1.57f, 0.0f);

	Otherwise::Camera3D cameraCheck(512, 512, glm::vec3(53.0f, 53.0f, 53.0f),
		45.0f, 1.0f, 20.0f, glm::vec3(0.0f, 1.0f, 0.0f), &manager, -1.57f, 0.0f);

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
	float minSpace = -1000.0f;
	float maxSpace = 1000.0f;
	Otherwise::SpatialSceneGraphOct spatialGraph(minSpace, maxSpace, minSpace, maxSpace, minSpace, maxSpace);
	spatialGraph.createChildren();
	sceneGraphOctStack.push_back(&spatialGraph);

	std::vector<Otherwise::OCube> cubes;

	GLuint frontSprite = Otherwise::loadPng("Textures/red.png");
	GLuint backSprite = Otherwise::loadPng("Textures/green.png");
	GLuint leftSprite = Otherwise::loadPng("Textures/blue.png");
	GLuint rightSprite = Otherwise::loadPng("Textures/purple.png");
	GLuint topSprite = Otherwise::loadPng("Textures/orange.png");
	GLuint bottomSprite = Otherwise::loadPng("Textures/yellow.png");

	for (float i = -100; i <= 100; i += 1)
	{
		for (float j = -100; j < 100; j += 1)
		{
			for (float k = -100; k < 100; k += 1)
			{
				spatialGraph.addEntityToGraph(new Otherwise::OCube(glm::vec3(i, j, k),
					0.3f, frontSprite, frontSprite, frontSprite, frontSprite, frontSprite,
					frontSprite, &multiSprite2));
			}
		}
	}

	std::cout << "camera at " << cameraCheck.getPosition().x << ", " << cameraCheck.getPosition().y << ", " << cameraCheck.getPosition().z << std::endl;

	for (unsigned int i = 0; i < sceneGraphOctStack.size(); i++)
	{
		if (Otherwise::aABBCnvx(sceneGraphOctStack[i]->getBox(), cameraCheck.getFrustum()))
		{
			sceneGraphOctStack[i]->addToStack(&spatialEntityStack, &sceneGraphOctStack);
		}
	}

	for (unsigned int i = 0; i < spatialEntityStack.size(); i++)
	{
		if (Otherwise::aABBCnvx(spatialEntityStack[i]->getBox(), cameraCheck.getFrustum()))
		{
			spatialEntityStack[i]->changeTextures(backSprite, backSprite, backSprite, backSprite, backSprite, backSprite);
			spatialEntityStack[i]->renderEntity();
			//std::cout << "Box at " << spatialEntityStack[i]->getPosition().x << ", " << spatialEntityStack[i]->getPosition().y << ", " << spatialEntityStack[i]->getPosition().z << std::endl;
		}
		
	}

	multiSprite2.prepareBatches();
	multiSprite3.prepareBatches();



	while (!doQuit)
	{
		audio.update();
		camera3D.update();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(programID);
		//triangle.textureDraw();
		glUniformMatrix4fv(shaderPerspectiveID, 1, GL_FALSE, &ortho[0][0]);
		//multiSprite3.renderBatches();
		//multiSprite.renderBatches();
		glUniformMatrix4fv(shaderPerspectiveID, 1, GL_FALSE, &camera3D.getPerspectiveMatrix()[0][0]);
		glUniformMatrix4fv(shaderModelMatrixID, 1, GL_FALSE, &camera3D.getModelMatrix()[0][0]);
		glUniformMatrix4fv(shaderCameraMatrixID, 1, GL_FALSE, &camera3D.getCameraMatrix()[0][0]);
		glUniformMatrix4fv(shaderModelCameraMatrixID, 1, GL_FALSE, &camera3D.getModelCameraMatrix()[0][0]);
		glUniform3f(shaderLightPositionID, 2.0f, 0.5f, -1.5f);
		glUniform3f(shaderLightColourID, 1.0f, 1.0f, 1.0f);
		glUniform1f(shaderLightIntensityID, 5.0f);
		//multiSprite3.renderBatches();
		multiSprite2.renderBatches();
		glUseProgram(secondProgramID);
		//square.draw();
		glUseProgram(0);
		newInput.inputQueue();
		newWindow.swapBuffer();
	}

	return 0;
}