#include <iostream>
#include <fstream>

#include "sphere.h"
#include "vec3.h"
#include "ray.h"
#include "hittable_list.h"
#include "camera.h"
#include <cstdlib>

using namespace std;
const char* filename = "outputimg.ppm";

vec3 white(1.0f, 1.0f, 1.0f);
vec3 blue(0.5f, 0.7f, 1.0f);
vec3 green(0.0f, 1.0f, 0.0f);

inline double random_double() {
	return rand() / (RAND_MAX + 1.0);
}

vec3 random_in_unit_sphere() {
	vec3 p;
	do {
		p = 2.0 * vec3(random_double(), random_double(), random_double()) - vec3(1, 1, 1);
	} while (p.squared_length() >= 1.0);
	return p;
}

vec3 color(const ray& r, hittable* world)
{
	hit_record rec;
	if (world->hit(r, 0.001, FLT_MAX, rec)) {
		vec3 target = rec.p + rec.normal + random_in_unit_sphere();
		return 0.5 * color(ray(rec.p, target - rec.p), world);
	}
	else {
		vec3 unit_direction = unit_vector(r.direction());
		float t = 0.5f * (unit_direction.y() + 1.0f);
		return (1.0f - t) * white + t * blue;
	}                                                                             
}

int main()
{
	fstream outstream;
	outstream.open(filename, ios::out);

	if (outstream.is_open()) {
		int nx = 600;
		int ny = 300;
		int ns = 100;

		outstream << "P3\n" << nx << " " << ny << "\n255\n";

		hittable* list[2];
		list[0] = new sphere(vec3(0, 0, -1), 0.5);
		list[1] = new sphere(vec3(0, -100.5, -1), 100);
		hittable* world = new hittable_list(list, 2);
		camera cam;
		for (int j = ny - 1; j >= 0; j--) {
			for (int i = 0; i < nx; i++) {
				vec3 col(0, 0, 0);
				for (int s = 0; s < ns;  s++) {
					float u = float(i + random_double()) / float(nx);
					float v = float(j + random_double()) / float(ny);
					ray r = cam.get_ray(u, v);
					col += color(r, world);
				}
				col /= float(ns);
				col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
				int ir = int(255.0 *col[0]);
				int ig = int(255.0 * col[1]);
				int ib = int(255.0 * col[2]);

				//cout << ir << " " << ig << " " << ib << "\n";
				outstream << ir << " " << ig << " " << ib << "\n";
			}
		}
	}

	outstream.close();
	//cout << "done";
	//cin.get();
	return 0;
}