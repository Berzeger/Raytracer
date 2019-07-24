#include <algorithm>
#include "Tracer.h"
#include "Sphere.h"

#define MAX_RAY_DEPTH 5

float Tracer::Mix(float a, float b, float mix) const
{
	return b * mix + a * (1 - mix);
}

Vec3f Tracer::Trace(const Vec3f &rayOrigin, const Vec3f &rayDirection, const std::vector<Sphere> &spheres, const int &depth) const
{
	float tNear = INFINITY;
	const Sphere* sphere = nullptr;

	// find intersection
	for (unsigned i = 0; i < spheres.size(); ++i)
	{
		float t0 = INFINITY;
		float t1 = INFINITY;

		if (spheres[i].Intersect(rayOrigin, rayDirection, t0, t1))
		{
			if (t0 < 0)
			{
				t0 = t1;
			}
			if (t0 < tNear)
			{
				tNear = t0;
				sphere = &spheres[i];
			}
		}
	}

	if (!sphere)
	{
		// no intersection, return black or background color
		return Vec3f(2);
	}

	Vec3f surfaceColor = 0;
	Vec3f hit = rayOrigin + rayDirection * tNear;
	Vec3f normal = hit - sphere->Center;
	normal.Normalize();

	// If the normal and the view direction are not opposite to each other
	// reverse the normal direction. That also means we are inside the sphere so set
	// the inside bool to true. Finally reverse the sign of IdotN which we want
	// positive.
	float bias = (float)1e-4; // add some bias to the point from which we will be tracing
	bool inside = false;

	if (rayDirection.Dot(normal) > 0)
	{
		normal = -normal;
		inside = true;
	}

	if ((sphere->Transparency > 0 || sphere->Reflection > 0) && depth < MAX_RAY_DEPTH)
	{
		float facingRation = -rayDirection.Dot(normal);
		// change the mix value to tweak the effect
		float fresnelEffect = Mix(pow(1 - facingRation, 3), 1, 0.1f);
		// reflection direction
		Vec3f reflectionDir = rayDirection - normal * 2 * rayDirection.Dot(normal);
		reflectionDir.Normalize();

		Vec3f reflection = Trace(hit + normal * bias, reflectionDir, spheres, depth + 1);
		Vec3f refraction = 0;

		// if the sphere is transparent, compute refraction ray (transmission)
		if (sphere->Transparency)
		{
			float ior = 1.1f;
			float eta = inside ? ior : 1 / ior;
			float cosi = -normal.Dot(rayDirection);
			float k = 1 - eta * eta * (1 - cosi * cosi);
			Vec3f refractionDir = rayDirection * eta + normal * (eta * cosi - sqrt(k));
			refractionDir.Normalize();
			refraction = Trace(hit - normal * bias, refractionDir, spheres, depth + 1);
		}

		// the result is a mix of reflection and refraction (if the sphere is transparent)
		surfaceColor = (reflection * fresnelEffect + refraction * (1 - fresnelEffect) * sphere->Transparency) * sphere->SurfaceColor;
	}
	else
	{
		// diffuse object, no need to raytrace any further
		for (unsigned i = 0; i < spheres.size(); ++i)
		{
			if (spheres[i].EmissionColor.X > 0)
			{
				// light
				Vec3f transmission = 1;
				Vec3f lightDirection = spheres[i].Center - hit;
				lightDirection.Normalize();

				for (unsigned j = 0; j < spheres.size(); ++j)
				{
					if (i != j)
					{
						float t0, t1;
						if (spheres[j].Intersect(hit + normal * bias, lightDirection, t0, t1))
						{
							transmission = 0;
							break;
						}
					}
				}

				surfaceColor += sphere->SurfaceColor * transmission *
					std::max(float(0), normal.Dot(lightDirection)) * spheres[i].EmissionColor;
			}
		}
	}

	return surfaceColor + sphere->EmissionColor;
}
