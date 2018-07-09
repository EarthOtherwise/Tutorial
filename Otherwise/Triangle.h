#pragma once
#include"GL/glew.h"
namespace Otherwise
{
	class Triangle
	{
	public:
		Triangle();
		~Triangle();
		void draw();
	private:
		GLuint mVboID = 0;
	};
}

