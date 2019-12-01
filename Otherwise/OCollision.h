#pragma once
#include"MathLibraryInterface.h"
#include<vector>

namespace Otherwise
{

struct OCollSphr
{
	ovec3 *pos;
	float radius;
};

struct OCollAABB
{
	ovec3 *pos;
	ovec3 maxOffset;
	ovec3 minOffset;
};

struct OCollPlane
{
	OCollPlane(ovec3 newNormal, float newOffset) { normal = newNormal;
	offset = newOffset; }
	ovec3 normal;
	float offset;
};

struct OCollCnvx
{
	ovec3 *pos;
	std::vector<OCollPlane> planes;
};

bool sphrSphrColl(OCollSphr *first, OCollSphr *second);
bool aABBAABBColl(OCollAABB *first, OCollAABB *second);

/*Returns false only if the sphere is completely on the side of the plane
opposite the normal.*/
bool sphrPlaneSide(OCollSphr *sphere, OCollPlane *plane);

bool sphrCnvxColl(OCollSphr *sphere, OCollCnvx *shape);

/*Returns false only if the box is completely on the side of the plane
opposite the normal.*/
bool aABBPlaneSide(OCollAABB *box, OCollPlane *plane);
bool aABBCnvx(OCollAABB *box, OCollCnvx *shape);

}
