#ifndef  RAYH
#define RAYH

#include "vec3.h"

class ray
{
public:
	ray() {};
	ray(const vec3& origin, const vec3& dir) { A = origin; B = dir; }
	vec3 origin() const { return A; }
	vec3 direction() const { return B; }
	vec3 point_at_parameter(float t) const { A + t*B; }

	vec3 A;
	vec3 B;
};
#endif // ! RAYH

