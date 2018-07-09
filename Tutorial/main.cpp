#include"InputHandler.h"
#include"Window.h"
#include"InitFileReadWrite.h"
#include<SDL/SDL.h>
#include"Triangle.h"
#include"SimpleShader.h"
#include"Square.h"
#include"MultiSprite.h"
#include"imageLoad.h"

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

	Otherwise::MultiSprite multiSprite;
	multiSprite.init();

	multiSprite.addSprite(Otherwise::loadPng("Textures/apple.png"), 0.0f,
		Otherwise::Vertex (0.0f, 0.0f, 0.0f, 1.0f, Otherwise::ColourRGBA8(255, 100, 100, 255)), Otherwise::Vertex(0.0f, 1.0f, 0.0f, 0.0f, Otherwise::ColourRGBA8(255, 100, 100, 255)),
		Otherwise::Vertex(1.0f, 0.0f, 1.0f, 1.0f, Otherwise::ColourRGBA8(255, 100, 100, 255)), Otherwise::Vertex(1.0f, 1.0f, 1.0f, 0.0f, Otherwise::ColourRGBA8(255, 100, 100, 255)));

	multiSprite.prepareBatches();

	while (!doQuit)
	{
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(programID);
		//triangle.textureDraw();
		multiSprite.renderBatches();
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