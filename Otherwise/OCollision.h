#pragma once
#include"glm/glm.hpp"
#include<vector>

namespace Otherwise
{

struct OCollSphr
{
	glm::vec3 *pos;
	float radius;
};

struct OCollAABB
{
	glm::vec3 *pos;
	glm::vec3 maxOffset;
	glm::vec3 minOffset;
};

struct OCollPlane
{
	OCollPlane(glm::vec3 newNormal, float newOffset) { normal = newNormal;
	offset = newOffset; }
	glm::vec3 normal;
	float offset;
};

struct OCollCnvx
{
	glm::vec3 *pos;
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
