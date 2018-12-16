#include "OCollision.h"
#include"glm/gtc/matrix_transform.hpp"

#include<iostream>

namespace Otherwise
{
bool sphrSphrColl(OCollSphr *first, OCollSphr *second)
{
	if ((*first->pos - *second->pos).length() < 
		first->radius + second->radius)
	{
		return true;
	}
	return false;
}
bool aABBAABBColl(OCollAABB *first, OCollAABB *second)
{
	if (first->pos->x + first->maxOffset.x <= 
		second->pos->x + second->minOffset.x)
	{
		return false;
	}
	if (first->pos->y + first->maxOffset.y <= 
		second->pos->y + second->minOffset.y)
	{
		return false;
	}
	if (first->pos->z + first->maxOffset.z <=
		second->pos->z + second->minOffset.z)
	{
		return false;
	}
	if (second->pos->x + second->maxOffset.x <=
		first->pos->x + first->minOffset.x)
	{
		return false;
	}
	if (second->pos->y + second->maxOffset.y <=
		first->pos->y + first->minOffset.y)
	{
		return false;
	}
	if (second->pos->z + second->maxOffset.z <=
		first->pos->z + first->minOffset.z)
	{
		return false;
	}
	return true;
}
bool sphrPlaneSide(OCollSphr *sphere, OCollPlane *plane)
{
	if (glm::dot(plane->normal, *sphere->pos) + plane->offset < 
		-sphere->radius)
	{
		return false;
	}
	return true;
}
bool sphrCnvxColl(OCollSphr *sphere, OCollCnvx *shape)
{
	for (unsigned int i = 0; i < shape->planes.size(); i++)
	{
		if (!sphrPlaneSide(sphere, &shape->planes[i]))
		{
			return false;
		}
	}
	return true;
}
bool aABBPlaneSide(OCollAABB *box, OCollPlane *plane)
{
	/*find which verticies are closes and furthest from the plane*/
	glm::vec3 minRelToPlane;
	glm::vec3 maxRelToPlane;
	if (plane->normal.x >= 0.0f)
	{
		minRelToPlane.x = box->pos->x + box->minOffset.x;
		maxRelToPlane.x = box->pos->x + box->maxOffset.x;
	}
	else
	{
		minRelToPlane.x = box->pos->x + box->maxOffset.x;
		maxRelToPlane.x = box->pos->x + box->minOffset.x;
	}
	if (plane->normal.y >= 0.0f)
	{
		minRelToPlane.y = box->pos->y + box->minOffset.y;
		maxRelToPlane.y = box->pos->y + box->maxOffset.y;
	}
	else
	{
		minRelToPlane.y = box->pos->y + box->maxOffset.y;
		maxRelToPlane.y = box->pos->y + box->minOffset.y;
	}
	if (plane->normal.z >= 0.0f)
	{
		minRelToPlane.z = box->pos->z + box->minOffset.z;
		maxRelToPlane.z = box->pos->z + box->maxOffset.z;
	}
	else
	{
		minRelToPlane.z = box->pos->z + box->maxOffset.z;
		maxRelToPlane.z = box->pos->z + box->minOffset.z;
	}

	if (glm::dot(plane->normal, minRelToPlane) + plane->offset > 0)
	{
		return true;
	}
	if (glm::dot(plane->normal, maxRelToPlane) + plane->offset > 0)
	{
		return true;
	}
	return false;
}
bool aABBCnvx(OCollAABB *box, OCollCnvx *shape)
{
	for (unsigned int i = 0; i < shape->planes.size(); i++)
	{
		if (!aABBPlaneSide(box, &shape->planes[i]))
		{
			return false;
		}
	}
	return true;
}

}