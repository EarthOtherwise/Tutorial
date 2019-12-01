#include "OCollision.h"

#include<iostream>

namespace Otherwise
{
bool sphrSphrColl(OCollSphr *first, OCollSphr *second)
{
	if ((*first->pos - *second->pos).vect.length() < 
		first->radius + second->radius)
	{
		return true;
	}
	return false;
}
bool aABBAABBColl(OCollAABB *first, OCollAABB *second)
{
	if (first->pos->vect.x + first->maxOffset.vect.x <=
		second->pos->vect.x + second->minOffset.vect.x)
	{
		return false;
	}
	if (first->pos->vect.y + first->maxOffset.vect.y <=
		second->pos->vect.y + second->minOffset.vect.y)
	{
		return false;
	}
	if (first->pos->vect.z + first->maxOffset.vect.z <=
		second->pos->vect.z + second->minOffset.vect.z)
	{
		return false;
	}
	if (second->pos->vect.x + second->maxOffset.vect.x <=
		first->pos->vect.x + first->minOffset.vect.x)
	{
		return false;
	}
	if (second->pos->vect.y + second->maxOffset.vect.y <=
		first->pos->vect.y + first->minOffset.vect.y)
	{
		return false;
	}
	if (second->pos->vect.z + second->maxOffset.vect.z <=
		first->pos->vect.z + first->minOffset.vect.z)
	{
		return false;
	}
	return true;
}
bool sphrPlaneSide(OCollSphr *sphere, OCollPlane *plane)
{
	if (dot_product(plane->normal, *sphere->pos) + plane->offset <
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
	ovec3 minRelToPlane;
	ovec3 maxRelToPlane;
	if (plane->normal.vect.x >= 0.0f)
	{
		minRelToPlane.vect.x = box->pos->vect.x + box->minOffset.vect.x;
		maxRelToPlane.vect.x = box->pos->vect.x + box->maxOffset.vect.x;
	}
	else
	{
		minRelToPlane.vect.x = box->pos->vect.x + box->maxOffset.vect.x;
		maxRelToPlane.vect.x = box->pos->vect.x + box->minOffset.vect.x;
	}
	if (plane->normal.vect.y >= 0.0f)
	{
		minRelToPlane.vect.y = box->pos->vect.y + box->minOffset.vect.y;
		maxRelToPlane.vect.y = box->pos->vect.y + box->maxOffset.vect.y;
	}
	else
	{
		minRelToPlane.vect.y = box->pos->vect.y + box->maxOffset.vect.y;
		maxRelToPlane.vect.y = box->pos->vect.y + box->minOffset.vect.y;
	}
	if (plane->normal.vect.z >= 0.0f)
	{
		minRelToPlane.vect.z = box->pos->vect.z + box->minOffset.vect.z;
		maxRelToPlane.vect.z = box->pos->vect.z + box->maxOffset.vect.z;
	}
	else
	{
		minRelToPlane.vect.z = box->pos->vect.z + box->maxOffset.vect.z;
		maxRelToPlane.vect.z = box->pos->vect.z + box->minOffset.vect.z;
	}

	if (dot_product(plane->normal, minRelToPlane) + plane->offset > 0)
	{
		return true;
	}
	if (dot_product(plane->normal, maxRelToPlane) + plane->offset > 0)
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