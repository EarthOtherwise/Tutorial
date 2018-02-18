#pragma once
#include "IAppState.h"

namespace Otherwise
{
	class LogoState :
		public IAppState
	{
	public:
		LogoState();
		~LogoState();

		void update();
		void render();
	};
}

