#pragma once
#include"GL/glew.h"
#include<string>
namespace Otherwise
{
	class Square
	{
	public:
		Square(std::string textureFilePath);
		~Square();
		void draw();
		void textureDraw();
	private:
		GLuint mVboID = 0;
		GLuint mUvBuffer = 0;
		GLuint mTexture = 0;
	};
}

