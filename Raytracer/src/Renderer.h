#pragma once
#include <vector>
#include "Sphere.h"
#include "Tracer.h"

class Renderer
{
private:
protected:
	unsigned _width, _height;
	float _fov;
	float _aspectRatio;
	float _angle;
	Tracer _tracer;
public:
	virtual void Render(const std::vector<Sphere> &spheres) const = 0;
};
