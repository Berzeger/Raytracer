#pragma once
#include <vector>
#include "Vector3.h"
#include "Sphere.h"

class Tracer
{
public:
	Vec3f Trace(const Vec3f &rayOrigin, const Vec3f &rayDirection, const std::vector<Sphere> &spheres, const int &depth) const;
};
