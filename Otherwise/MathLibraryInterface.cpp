#include "MathLibraryInterface.h"

namespace Otherwise
{
omat4 scale_matrix(omat4 matrix, ovec3 scaling)
{
	omat4 scaled_matrix(1.0f);
	scaled_matrix.matrix = glm::scale(matrix.matrix, scaling.vect);
	return scaled_matrix;
}
omat4 translate_matrix(omat4 matrix, ovec3 vect)
{
	omat4 translated_matrix(1.0f);
	translated_matrix.matrix = glm::translate(matrix.matrix, vect.vect);
	return translated_matrix;
}
omat4 orthographic_matrix(float left, float right, float bottom, float top)
{
	omat4 ortho_matrix(1.0f);
	ortho_matrix.matrix = glm::ortho(left, right, bottom, top);
	return ortho_matrix;
}
ovec3 cross_product(ovec3 first, ovec3 second)
{
	ovec3 return_vec;
	return_vec.vect = glm::cross(first.vect, second.vect);
	return return_vec;
}
float dot_product(ovec3 first, ovec3 second)
{
	return glm::dot(first.vect, second.vect);
}
ovec3 normalize_vect(ovec3 non_normal_vect)
{
	ovec3 return_vec;
	return_vec.vect = glm::normalize(non_normal_vect.vect);
	return return_vec;
}
omat4 look_at_view_matrix(ovec3 position, ovec3 lookAtPosition, ovec3 upDir)
{
	omat4 return_mat;
	return_mat.matrix = glm::lookAt(position.vect, lookAtPosition.vect, upDir.vect);
	return return_mat;
}
omat4 perspective_matrix(float fieldOfViewDegrees, float screenRatio, float nearClippingDistance, float farClippingDistance)
{
	omat4 return_mat;
	return_mat.matrix = glm::perspective(glm::radians(fieldOfViewDegrees),
		screenRatio, nearClippingDistance, farClippingDistance);
	return return_mat;
}
float degrees_to_radians(float degrees)
{
	return glm::radians(degrees);
}
float tangent(float degrees)
{
	return glm::tan(degrees);
}
}