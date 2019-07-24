#pragma once
#include <vector>
#include "Vector3.h"
#include "Sphere.h"

class Tracer
{
private:
	float Mix(float a, float b, float mix) const;
public:
	Vec3f Trace(const Vec3f &rayOrigin, const Vec3f &rayDirection, const std::vector<Sphere> &spheres, const int &depth) const;
};
