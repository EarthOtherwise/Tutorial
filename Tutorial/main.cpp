#include<InputHandler.h>
#include<Window.h>
#include<InitFileReadWrite.h>
#include<SDL/SDL.h>

/*temporarily include iostream*/
#include<iostream>

int main(int argc, char** argv)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	Otherwise::Window newWindow;
	newWindow.create("Window", 500, 500, 0);

	Otherwise::InputHandler newInput;

	bool doQuit = false;

	while (!doQuit)
	{
		newInput.inputQueue();
		if (newInput.isKeyDown(SDLK_w))
		{
			doQuit = true;
		}
		newWindow.swapBuffer();
	}

	return 0;
}