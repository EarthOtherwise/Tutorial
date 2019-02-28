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
#include "ErrHandler.h"
#include "ONetwork.h"
#include "OMeshRenderer.h"

/*temporarily include iostream*/
#include<iostream>

//#define GLM_ENABLE_EXPERIMENTAL
//#include "glm/gtc/quaternion.hpp"
//#include "glm/gtx/quaternion.hpp"
//#include "glm/gtx/euler_angles.hpp"

int main(int argc, char** argv)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	
	Otherwise::Window newWindow;
	newWindow.create("Window", 512, 512, 0);

	Otherwise::GraphicsResourceManager graphics;

	Otherwise::CorrespondentManager manager;
	manager.init();

	Otherwise::Correspondent quitCorrespondent;
	quitCorrespondent.init(&manager, (std::string)"QuitGame");

	Otherwise::ONetwork network(&manager);
	std::string ip;
	ip = "0";
	if (ip == "0")
	{
		network.initHosting(1598);
	}
	else
	{
		network.connectToHost(ip.c_str(), 1598);
	}

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

	GLuint meshProgramID = Otherwise::compileLinkSimpleShaders("Mesh.vert", "Mesh.frag");
	GLint shaderPerspectiveIDm = glGetUniformLocation(meshProgramID, "Perspective");
	GLint shaderCameraMatrixIDm = glGetUniformLocation(meshProgramID, "CameraMatrix");
	GLint shaderModelMatrixIDm = glGetUniformLocation(meshProgramID, "ModelMatrix");
	GLint shaderLightPositionIDm = glGetUniformLocation(meshProgramID, "LightPosition");
	GLint shaderModelCameraMatrixIDm = glGetUniformLocation(meshProgramID, "ModelCameraMatrix");
	GLint shaderLightColourIDm = glGetUniformLocation(meshProgramID, "LightColour");
	GLint shaderLightIntensityIDm = glGetUniformLocation(meshProgramID, "LightIntensity");

	GLuint billboardProgramID = Otherwise::compileLinkSimpleShaders("Billboard.vert", "Billboard.frag");
	GLint shaderPerspectiveIDma = glGetUniformLocation(billboardProgramID, "Perspective");
	GLint shaderCameraMatrixIDma = glGetUniformLocation(billboardProgramID, "CameraMatrix");
	GLint shaderModelMatrixIDma = glGetUniformLocation(billboardProgramID, "ModelMatrix");

	Otherwise::MultiSprite multiSprite3;
	multiSprite3.init();

	Otherwise::SpriteFont spriteFont("Fonts/chintzy.ttf", 64);
	char buffer[256];
	sprintf_s(buffer, "Test String");
	spriteFont.draw(multiSprite3, buffer, glm::vec2(0.0f, 0.0f), glm::vec2(0.01f), 0.0f, Otherwise::ColourRGBA8(255, 255, 255, 255));

	Otherwise::Camera2D camera2D (512, 512, glm::vec2(0.0f, 0.0f), 1.0f);
	glm::mat4 ortho = camera2D.getMatrix();

	Otherwise::Camera3D camera3D(512, 512, glm::vec3(0.0f, 0.0f, 5.0f),
		45.0f, 1.0f, 100.0f, glm::vec3(0.0f, 1.0f, 0.0f), &manager, 1.57f, 0.0f);

	Otherwise::Camera3D cameraCheck(512, 512, glm::vec3(0.0f, 0.0f, 0.0f),
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

	GLuint frontSprite = Otherwise::loadPng("Textures/rainbow.png");
	GLuint backSprite = Otherwise::loadPng("Textures/green.png");
	GLuint leftSprite = Otherwise::loadPng("Textures/blue.png");
	GLuint rightSprite = Otherwise::loadPng("Textures/purple.png");
	GLuint topSprite = Otherwise::loadPng("Textures/orange.png");
	GLuint bottomSprite = Otherwise::loadPng("Textures/yellow.png");

	for (float i = -1; i <= 1; i += 1)
	{
		for (float j = -1; j < 1; j += 1)
		{
			for (float k = -1; k < 1; k += 1)
			{
				spatialGraph.addEntityToGraph(new Otherwise::OCube(glm::vec3(i, j, k),
					0.3f, frontSprite, frontSprite, frontSprite, frontSprite, frontSprite,
					frontSprite, &multiSprite2));
			}
		}
	}

	for (unsigned int i = 0; i < sceneGraphOctStack.size(); i++)
	{
		if (Otherwise::aABBCnvx(sceneGraphOctStack[i]->getBox(), cameraCheck.getFrustum()))
		{
			sceneGraphOctStack[i]->addToStack(&spatialEntityStack, &sceneGraphOctStack);
		}
	}

	for (unsigned int i = 0; i < spatialEntityStack.size(); i++)
	{
		if (1/*Otherwise::aABBCnvx(spatialEntityStack[i]->getBox(), cameraCheck.getFrustum())*/)
		{
			spatialEntityStack[i]->changeTextures(backSprite, backSprite, backSprite, backSprite, backSprite, backSprite);
			spatialEntityStack[i]->renderEntity();
			std::cout << "Box at " << spatialEntityStack[i]->getPosition().x << ", " << spatialEntityStack[i]->getPosition().y << ", " << spatialEntityStack[i]->getPosition().z << std::endl;
		}
		
	}

	multiSprite2.prepareBatches();
	multiSprite3.prepareBatches();

	gui.mChatBox.setVisible(true);

	Otherwise::OMeshRenderer meshRenderer;
	meshRenderer.init();
	meshRenderer.addMesh("Meshes/plane.obj", frontSprite);
	meshRenderer.prepareMesh();

	std::vector<glm::mat4> meshModelMatrices;

	for (float i = -10.0f; i < 10.0f; i += 5.0f)
	{
		for (float j = -10.0f; j < 10.0f; j += 5.0f)
		{
			for (float k = -10.0f; k < 10.0f; k += 5.0f)
			{
				glm::mat4 tempMatrix(1.0f);// = glm::scale(glm::mat4(1.0f), glm::vec3(k, k, k));
				//tempMatrix *= glm::eulerAngleXYZ(i, j, k);
				tempMatrix *= glm::translate(glm::mat4(1.0f), glm::vec3(i, j, k));
				meshModelMatrices.push_back(tempMatrix);
			}
		}
	}

	while (!quitCorrespondent.getMessage())
	{
		network.update();
		gui.update();
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

		glUseProgram(meshProgramID);

		glUniformMatrix4fv(shaderPerspectiveIDm, 1, GL_FALSE, &camera3D.getPerspectiveMatrix()[0][0]);
		
		glUniformMatrix4fv(shaderCameraMatrixIDm, 1, GL_FALSE, &camera3D.getCameraMatrix()[0][0]);
		glUniformMatrix4fv(shaderModelCameraMatrixIDm, 1, GL_FALSE, &camera3D.getModelCameraMatrix()[0][0]);
		glUniform3f(shaderLightPositionIDm, 2.0f, 0.5f, -1.5f);
		glUniform3f(shaderLightColourIDm, 1.0f, 1.0f, 1.0f);
		glUniform1f(shaderLightIntensityIDm, 5.0f);


		glUseProgram(billboardProgramID);
		glUniformMatrix4fv(shaderPerspectiveIDma, 1, GL_FALSE, &camera3D.getProjectionMatrix()[0][0]);
		glUniformMatrix4fv(shaderCameraMatrixIDma, 1, GL_FALSE, &camera3D.getCameraMatrix()[0][0]);

		for (unsigned int i = 0; i < meshModelMatrices.size(); i++)
		{
			glUniformMatrix4fv(shaderModelMatrixIDma, 1, GL_FALSE, &meshModelMatrices[i][0][0]);
			meshRenderer.renderMesh();
		}
		
		//square.draw();
		glUseProgram(0);
		newInput.inputQueue();

		gui.render();

		newWindow.swapBuffer();
	}

	return 0;
}