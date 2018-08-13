#include "LogoState.h"

namespace Otherwise
{

	LogoState::LogoState()
	{
	}


	LogoState::~LogoState()
	{
	}

	void LogoState::init(std::string vertShader, std::string fragShader)
	{
		mProgramID = Otherwise::compileLinkSimpleShaders(vertShader, fragShader);
	}

}