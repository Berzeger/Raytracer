#pragma once
#include "Renderer.h"

#if defined __linux__  || defined __APPLE__
#else
#define M_PI 3.141592653589793
#endif

class PpmRenderer : Renderer
{
public:
	PpmRenderer(unsigned width, unsigned height, float fov);
	void Render(const std::vector<Sphere> &spheres) const;
};
