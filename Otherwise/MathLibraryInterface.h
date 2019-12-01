#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace Otherwise
{

struct ovec3 
{
	ovec3()
	{
		vect.x = 0.0f;
		vect.y = 0.0f;
		vect.z = 0.0f;
	}

	ovec3(float x)
	{
		vect.x = x;
		vect.y = x;
		vect.z = x;
	}

	ovec3(float x, float y, float z)
	{
		vect.x = x;
		vect.y = y;
		vect.z = z;
	}

	void operator+=(const ovec3 &operatedvec)
	{
		vect += operatedvec.vect;
	}

	ovec3 operator+(const ovec3 &operatedvec) const
	{
		ovec3 returnvect;
		returnvect.vect = vect + operatedvec.vect;
		return returnvect;
	}

	ovec3 operator*(const float &operatedFloat) const
	{
		ovec3 returnvect;
		returnvect.vect = vect * operatedFloat;
		return returnvect;
	}
	
	ovec3 operator*(const ovec3 &operatedvec) const
	{
		ovec3 returnvect;
		returnvect.vect = vect * operatedvec.vect;
		return returnvect;
	}

	ovec3 operator-(const ovec3 &operatedvec) const
	{
		ovec3 returnvect;
		returnvect.vect = vect - operatedvec.vect;
		return returnvect;
	}

	ovec3 inverse()
	{
		ovec3 returnvect;
		returnvect.vect = vect * -1.0f;
		return returnvect;
	}

		glm::vec3 vect;
};

struct ovec2
{
	ovec2()
	{
		vect.x = 0.0f;
		vect.y = 0.0f;
	}

	ovec2(float x)
	{
		vect.x = x;
		vect.y = x;
	}

	ovec2(float x, float y)
	{
		vect.x = x;
		vect.y = y;
	}

	ovec2 operator*(const ovec2 &operatedvec) const
	{
		ovec2 returnvec;
		returnvec.vect = vect * operatedvec.vect;
		return returnvec;
	}

	glm::vec2 vect;
};

struct ovec4
{
	ovec4()
	{
		vect.x = 0.0f;
		vect.y = 0.0f;
		vect.z = 0.0f;
		vect.w = 0.0f;
	}

	ovec4(float x, float y, float z, float w)
	{
		vect.x = x;
		vect.y = y;
		vect.z = z;
		vect.w = w;
	}

	ovec4(ovec2 first, ovec2 second)
	{
		vect.x = first.vect.x;
		vect.y = first.vect.y;
		vect.z = second.vect.x;
		vect.w = second.vect.y;
	}

	ovec4(float x)
	{
		vect.x = x;
		vect.y = x;
		vect.z = x;
		vect.w = x;
	}

	glm::vec4 vect;
};

struct oivec4
{
	oivec4()
	{
		vect.x = 0;
		vect.y = 0;
		vect.z = 0;
		vect.w = 0;
	}
	oivec4(int x, int y, int z, int w)
	{
		vect.x = x;
		vect.y = y;
		vect.z = z;
		vect.w = w;
	}
	oivec4(int x)
	{
		vect.x = x;
		vect.y = x;
		vect.z = x;
		vect.w = x;
	}
	glm::ivec4 vect;
};

struct omat4
{
	omat4()
	{
		matrix = glm::mat4(1.0f);
	}

	omat4(float x)
	{
		matrix = glm::mat4(x);
	}

	omat4 operator*(omat4 &operatedmatrix)
	{
		omat4 returnmatrix;
		returnmatrix.matrix = matrix * operatedmatrix.matrix;
		return returnmatrix;
	}

	void operator*=(omat4 &operatedmatrix)
	{
		matrix *= operatedmatrix.matrix;
	}

	glm::mat4 matrix;
};

omat4 scale_matrix(omat4 matrix, ovec3 scaling);

omat4 translate_matrix(omat4 matrix, ovec3 vect);

omat4 orthographic_matrix(float left, float right, float bottom, float top);

ovec3 cross_product(ovec3 first, ovec3 second);

float dot_product(ovec3 first, ovec3 second);

ovec3 normalize_vect(ovec3 non_normal_vect);

omat4 look_at_view_matrix(ovec3 position, ovec3 lookAtPosition, ovec3 upDir);

omat4 perspective_matrix(float fieldOfViewDegrees, float screenRatio,
	float nearClippingDistance, float farClippingDistance);

float degrees_to_radians(float degrees);

float tangent(float degrees);
}

