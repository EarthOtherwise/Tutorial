#include "Window.h"


namespace Otherwise
{
	Window::Window()
	{
	}


	Window::~Window()
	{
	}

	int Otherwise::Window::create(std::string windowName, unsigned int screenWidth, unsigned int screenHeight, int currentFlags)
	{
		Uint32 flags = SDL_WINDOW_OPENGL;
		mScreenWidth = screenWidth;
		mScreenHeight = screenHeight;

		if (currentFlags & INVISIBLE)
		{
			flags |= SDL_WINDOW_HIDDEN;
		}
		if (currentFlags & FULLSCREEN)
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}
		if (currentFlags & BORDERLESS)
		{
			flags |= SDL_WINDOW_BORDERLESS;
		}

		mSDLWindow = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, flags);

		return 0;
	}

	void Otherwise::Window::swapBuffer()
	{
		SDL_GL_SwapWindow(mSDLWindow);
	}
}