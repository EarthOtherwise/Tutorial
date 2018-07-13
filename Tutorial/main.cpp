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

/*temporarily include iostream*/
#include<iostream>

int main(int argc, char** argv)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	Otherwise::Window newWindow;
	newWindow.create("Window", 512, 512, 0);

	Otherwise::InputHandler newInput;

	bool doQuit = false;

	Otherwise::Square triangle("Textures/apple.png");
	Otherwise::Triangle square;

	GLuint programID = Otherwise::compileLinkSimpleShaders("TextureVert.vert", "TextureFrag.frag");
	GLuint secondProgramID = Otherwise::compileLinkSimpleShaders("SimpleVert.vert", "SimpleFrag.frag");

	GLint location = glGetUniformLocation(programID, "Projection");

	Otherwise::MultiSprite multiSprite3;
	multiSprite3.init();

	Otherwise::SpriteFont spriteFont("Fonts/chintzy.ttf", 64);
	char buffer[256];
	sprintf_s(buffer, "Test String");
	spriteFont.draw(multiSprite3, buffer, glm::vec2(0.0f, 0.0f), glm::vec2(0.01f), 0.0f, Otherwise::ColourRGBA8(255, 255, 255, 255));

	Otherwise::Camera2D camera2D (512, 512, glm::vec2(0.0f, 0.0f), 1.0f);
	glm::mat4 camera2DMatrix = camera2D.getMatrix();
	Otherwise::Camera3D camera3D (512, 512, glm::vec3(4.0f, 4.0f, 3.0f), glm::vec3(0.5f, 0.5f, 0.5f), 45.0f, 0.01f, 100.0f);
	glm::mat4 camera3DMatrix = camera3D.getMatrix();

	Otherwise::MultiSprite multiSprite;
	multiSprite.init();

	multiSprite.addSprite(Otherwise::loadPng("Textures/apple.png"), 0.0f,
		Otherwise::Vertex (0.0f, 0.0f, 0.0f, 0.0f, 1.0f, Otherwise::ColourRGBA8(255, 100, 100, 255)), Otherwise::Vertex(0.0f, 100.0f, 0.0f, 0.0f, 0.0f, Otherwise::ColourRGBA8(255, 100, 100, 255)),
		Otherwise::Vertex(100.0f, 0.0f, 0.0f, 1.0f, 1.0f, Otherwise::ColourRGBA8(255, 100, 100, 255)), Otherwise::Vertex(100.0f, 100.0f, 0.0f, 1.0f, 0.0f, Otherwise::ColourRGBA8(255, 100, 100, 255)));

	multiSprite.prepareBatches();


	Otherwise::MultiSprite multiSprite2;
	multiSprite2.init();

	//front
	multiSprite2.addSprite(Otherwise::loadPng("Textures/red.png"), 0.0f,
		Otherwise::Vertex(0.0f, 0.0f, 0.0f, 0.0f, 1.0f, Otherwise::ColourRGBA8(255, 255, 255, 255)), Otherwise::Vertex(0.0f, 1.0f, 0.0f, 0.0f, 0.0f, Otherwise::ColourRGBA8(255, 255, 255, 255)),
		Otherwise::Vertex(1.0f, 0.0f, 0.0f, 1.0f, 1.0f, Otherwise::ColourRGBA8(255, 255, 255, 255)), Otherwise::Vertex(1.0f, 1.0f, 0.0f, 1.0f, 0.0f, Otherwise::ColourRGBA8(255, 255, 255, 255)));
	//right
	multiSprite2.addSprite(Otherwise::loadPng("Textures/green.png"), 0.0f,
		Otherwise::Vertex(1.0f, 0.0f, 0.0f, 0.0f, 1.0f, Otherwise::ColourRGBA8(255, 255, 255, 255)), Otherwise::Vertex(1.0f, 1.0f, 0.0f, 0.0f, 0.0f, Otherwise::ColourRGBA8(255, 255, 255, 255)),
		Otherwise::Vertex(1.0f, 0.0f, 1.0f, 1.0f, 1.0f, Otherwise::ColourRGBA8(255, 255, 255, 255)), Otherwise::Vertex(1.0f, 1.0f, 1.0f, 1.0f, 0.0f, Otherwise::ColourRGBA8(255, 255, 255, 255)));
	//bottom
	multiSprite2.addSprite(Otherwise::loadPng("Textures/blue.png"), 0.0f,
		Otherwise::Vertex(0.0f, 0.0f, 1.0f, 0.0f, 1.0f, Otherwise::ColourRGBA8(255, 255, 255, 255)), Otherwise::Vertex(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, Otherwise::ColourRGBA8(255, 255, 255, 255)),
		Otherwise::Vertex(1.0f, 0.0f, 1.0f, 1.0f, 1.0f, Otherwise::ColourRGBA8(255, 255, 255, 255)), Otherwise::Vertex(1.0f, 0.0f, 0.0f, 1.0f, 0.0f, Otherwise::ColourRGBA8(255, 255, 255, 255)));
	//left
	multiSprite2.addSprite(Otherwise::loadPng("Textures/purple.png"), 0.0f,
		Otherwise::Vertex(0.0f, 0.0f, 1.0f, 0.0f, 1.0f, Otherwise::ColourRGBA8(255, 255, 255, 255)), Otherwise::Vertex(0.0f, 1.0f, 1.0f, 0.0f, 0.0f, Otherwise::ColourRGBA8(255, 255, 255, 255)),
		Otherwise::Vertex(0.0f, 0.0f, 0.0f, 1.0f, 1.0f, Otherwise::ColourRGBA8(255, 255, 255, 255)), Otherwise::Vertex(0.0f, 1.0f, 0.0f, 1.0f, 0.0f, Otherwise::ColourRGBA8(255, 255, 255, 255)));
	//back
	multiSprite2.addSprite(Otherwise::loadPng("Textures/orange.png"), 0.0f,
		Otherwise::Vertex(0.0f, 0.0f, 1.0f, 0.0f, 1.0f, Otherwise::ColourRGBA8(255, 255, 255, 255)), Otherwise::Vertex(0.0f, 1.0f, 1.0f, 0.0f, 0.0f, Otherwise::ColourRGBA8(255, 255, 255, 255)),
		Otherwise::Vertex(1.0f, 0.0f, 1.0f, 1.0f, 1.0f, Otherwise::ColourRGBA8(255, 255, 255, 255)), Otherwise::Vertex(1.0f, 1.0f, 1.0f, 1.0f, 0.0f, Otherwise::ColourRGBA8(255, 255, 255, 255)));
	//top
	multiSprite2.addSprite(Otherwise::loadPng("Textures/yellow.png"), 0.0f,
		Otherwise::Vertex(0.0f, 1.0f, 0.0f, 0.0f, 1.0f, Otherwise::ColourRGBA8(255, 255, 255, 255)), Otherwise::Vertex(0.0f, 1.0f, 1.0f, 0.0f, 0.0f, Otherwise::ColourRGBA8(255, 255, 255, 255)),
		Otherwise::Vertex(1.0f, 1.0f, 0.0f, 1.0f, 1.0f, Otherwise::ColourRGBA8(255, 255, 255, 255)), Otherwise::Vertex(1.0f, 1.0f, 1.0f, 1.0f, 0.0f, Otherwise::ColourRGBA8(255, 255, 255, 255)));

	multiSprite2.prepareBatches();
	multiSprite3.prepareBatches();

	while (!doQuit)
	{
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(programID);
		//triangle.textureDraw();
		glUniformMatrix4fv(location, 1, GL_FALSE, &camera2DMatrix[0][0]);
		//multiSprite3.renderBatches();
		//multiSprite.renderBatches();
		glUniformMatrix4fv(location, 1, GL_FALSE, &camera3DMatrix[0][0]);
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
		newWindow.swapBuffer();

	}

	return 0;
}