#include "Triangle.h"

namespace Otherwise
{

	Triangle::Triangle()
	{
		glGenBuffers(1, &mVboID);
		static const GLfloat vboData[] =
		{
			-1.0f, -1.0f, 1.0f, -1.0f, 0.0f, 1.0f
		};
		glBindBuffer(GL_ARRAY_BUFFER, mVboID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vboData), vboData, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}


	Triangle::~Triangle()
	{
	}
	void Triangle::draw()
	{
		glBindBuffer(GL_ARRAY_BUFFER, mVboID);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glDisableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}