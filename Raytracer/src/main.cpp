#include <vector>
#include <iostream>
#include "Timer.h"
#include "Sphere.h"
#include "PpmRenderer.h"
#include "MultiThreadedPpmRenderer.h"

int main(int argc, char ** argv) 
{
	std::vector<Sphere> spheres;
	// position, radius, surface color, reflectivity, transparency, emission color
	spheres.push_back(Sphere(Vec3f( 0.0, -10004, -20), 10000, Vec3f(0.20, 0.20, 0.20), 0, 0.0));
	spheres.push_back(Sphere(Vec3f( 0.0,      0, -20),     4, Vec3f(1.00, 0.32, 0.36), 1, 0.5));
	spheres.push_back(Sphere(Vec3f( 5.0,     -1, -15),     2, Vec3f(0.90, 0.76, 0.46), 1, 0.0));
	spheres.push_back(Sphere(Vec3f( 5.0,      0, -25),     3, Vec3f(0.65, 0.77, 0.97), 1, 0.0));
	spheres.push_back(Sphere(Vec3f(-5.5,      0, -15),     3, Vec3f(0.90, 0.90, 0.90), 1, 0.0));

	// light
	spheres.push_back(Sphere(Vec3f( 0.0,     20, -30),     3, Vec3f(0.00, 0.00, 0.00), 0, 0.0, Vec3f(3)));

	Timer timer;
	timer.start();
	PpmRenderer renderer(1920, 1080, 30);
	renderer.Render(spheres);
	timer.end();

	std::cout << "Singlethreaded rendered in ";
	timer.printMs();

	timer.start();
	MultiThreadedPpmRenderer multiThreadedRenderer(1920, 1080, 30);
	multiThreadedRenderer.Render(spheres);
	timer.end();

	std::cout << "Multithreaded rendered in ";
	timer.printMs();
	std::cin.get();
	return 0;
}