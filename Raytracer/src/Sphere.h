#pragma once
#include "Vector3.h"

typedef Vector3<float> Vec3f;

class Sphere
{
private:
public:
	Vec3f Center;
	float Radius, RadiusSqr;
	Vec3f SurfaceColor, EmissionColor;
	float Transparency, Reflection;

	Sphere(const Vec3f &center, float radius, const Vec3f &surfaceColor, float reflection = 0, float transparency = 0, const Vec3f &emissionColor = 0) :
		Center(center),
		Radius(radius),
		RadiusSqr(radius * radius),
		SurfaceColor(surfaceColor),
		Reflection(reflection),
		Transparency(transparency),
		EmissionColor(emissionColor)
	{ }

	bool Intersect(const Vec3f &rayOrigin, const Vec3f &rayDirection, float &t0, float &t1) const
	{
		Vec3f l = Center - rayOrigin;
		float tca = l.Dot(rayDirection);
		if (tca < 0)
		{
			return false;
		}

		float d2 = l.Dot(l) - tca * tca;
		if (d2 > RadiusSqr)
		{
			return false;
		}

		float thc = sqrt(RadiusSqr - d2);
		t0 = tca - thc;
		t1 = tca + thc;

		return true;
	}
};
