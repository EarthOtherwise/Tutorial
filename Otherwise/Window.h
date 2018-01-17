#pragma once
#include<SDL\SDL.h>
#include<GL\glew.h>
#include<string>

namespace Otherwise
{
	/*Enumerator that gives us the ability to change the style of window; used in the create function.*/
	enum WindowFlags { INVISIBLE = 0x1, FULLSCREEN = 0x2, BORDERLESS = 0x4 };

	class Window
	{
	public:
		Window();
		~Window();

		int create(std::string windowName, unsigned int screenWidth, unsigned int screenHeight, int currentFlags);

		void swapBuffer();

		unsigned int getScreenWidth() { return mScreenWidth;  }
		unsigned int getScreenHeight() { return mScreenHeight; }
	private:
		SDL_Window* mSDLWindow;
		unsigned int mScreenWidth, mScreenHeight;
	};

}