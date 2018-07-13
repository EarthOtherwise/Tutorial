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
		
		SDL_GLContext glContext = SDL_GL_CreateContext(mSDLWindow);

		glewInit();

		glClearDepth(1.0);

		glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		

		return 0;
	}

	void Otherwise::Window::swapBuffer()
	{
		SDL_GL_SwapWindow(mSDLWindow);
	}
}