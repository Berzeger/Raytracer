#include "MultiThreadedPpmRenderer.h"
#include <algorithm>
#include <fstream>
#include <thread>
#include <future>

#define NUM_THREADS 8

MultiThreadedPpmRenderer::MultiThreadedPpmRenderer(unsigned width, unsigned height, float fov)
{
	_width = width;
	_height = height;
	_fov = fov;
	_aspectRatio = _width / float(_height);
	_angle = (float)tan(M_PI * 0.5 * _fov / 180.);
}

void MultiThreadedPpmRenderer::Render(const std::vector<Sphere> &spheres) const
{
	Vec3f * image = new Vec3f[_width * _width];
	float invWidth = 1 / float(_width);
	float invHeight = 1 / float(_height);

	unsigned cores = std::thread::hardware_concurrency();
	unsigned max = _height * _width;
	volatile std::atomic<unsigned> count(0);
	std::vector<std::future<void>> futures;

	while (cores--)
	{
		futures.emplace_back(std::async([=, &count]() 
		{
			while (true)
			{
				unsigned index = count++;
				if (index >= max)
				{
					break;
				}

				unsigned x = index % _width;
				unsigned y = index / _width;

				float xx = (float)(2 * ((x + 0.5) * invWidth) - 1) * _angle * _aspectRatio;
				float yy = (float)(1 - 2 * ((y + 0.5) * invHeight)) * _angle;
				Vec3f rayDirection(xx, yy, -1);
				rayDirection.Normalize();
				image[index] = _tracer.Trace(Vec3f(0), rayDirection, spheres, 0);
			}
		}));
	}

	for (std::future<void> &future : futures)
	{
		future.wait();
	}

	// Save to PPM
	std::ofstream ofs("./multithreaded_render.ppm", std::ios::out | std::ios::binary);
	ofs << "P6\n" << _width << " " << _height << "\n255\n";

	unsigned upperBound = _width * _height;
	for (unsigned i = 0; i < upperBound; ++i)
	{
		ofs << (unsigned char)(std::min(float(1), image[i].X) * 255) <<
			(unsigned char)(std::min(float(1), image[i].Y) * 255) <<
			(unsigned char)(std::min(float(1), image[i].Z) * 255);
	}

	ofs.close();
	delete[] image;
}
