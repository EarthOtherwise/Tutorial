#pragma once
#include "Cameras.h"
#include "MultiSprite.h"
#include "SimpleShader.h"

#include<string>

namespace Otherwise
{
	class LogoState
	{
	public:
		LogoState();
		~LogoState();

		void init(std::string vertShader, std::string fragShader);

	protected:
		Camera2D mCamera;
		MultiSprite mMultiSprite;
		GLuint mProgramID;
	};
}

