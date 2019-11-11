#include <iostream>
#include <fstream>

#include "sphere.h"
#include "vec3.h"
#include "ray.h"
#include "hittable_list.h"
#include "camera.h"
#include "util.h"

using namespace std;
const char* filename = "outputimg.ppm";

vec3 white(1.0f, 1.0f, 1.0f);
vec3 blue(0.5f, 0.7f, 1.0f);
vec3 green(0.0f, 1.0f, 0.0f);

vec3 color(const ray& r, hittable* world, int depth)
{
	
	hit_record rec;
	if (world->hit(r, 0.001, FLT_MAX, rec)) {
		ray scattered;
		vec3 attenuation;
		if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
			return attenuation * color(scattered, world, depth+1);
		}
		else {
			return vec3(0, 0, 0);
		}
	}
	else {
		vec3 unit_direction = unit_vector(r.direction());
		float t = 0.5f * (unit_direction.y() + 1.0f);
		return (1.0f - t) * white + t * blue;
	}                                                                             
}

hittable* random_scene() {
	int n = 500;
	hittable** list = new hittable*[n+1];
	list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(vec3(0.5, 0.5, 0.5)));
	int i = 1;
	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			float choose_mat = random_double();
			vec3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());
			if ((center - vec3(4, 0.2, 0)).length() > 0.9) {
				if (choose_mat < 0.8) {
					// diffuse
					list[i++] = new sphere(center, 0.2, new lambertian(vec3(random_double() * random_double(),
																			random_double() * random_double(), 
																			random_double() * random_double())));
				}
				else if (choose_mat < 0.95) {
					// metal
					list[i++] = new sphere(center, 0.2, new metal(vec3(	0.5 * (1 + random_double()),
																		0.5 * (1 + random_double()),
																		0.5 * (1 + random_double())),
																0.5 * random_double()));
				}
				else {
					list[i++] = new sphere(center, 0.2, new dielectric(1.5));
				}
			}
		}
	}

	list[i++] = new sphere(vec3(0, 1, 0), 1.0, new dielectric(1.5));
	list[i++] = new sphere(vec3(-4, 1, 0), 1.0, new lambertian(vec3(0.4, 0.2, 0.1)));
	list[i++] = new sphere(vec3(4, 1, 0), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0));

	return new hittable_list(list, i);
}

int main()
{
	fstream outstream;
	outstream.open(filename, ios::out);

	if (outstream.is_open()) {
		int nx = 1920;
		int ny = 1080;
		int ns = 10;

		outstream << "P3\n" << nx << " " << ny << "\n255\n";

		hittable* world = random_scene();
		vec3 lookfrom(13,2,3);
		vec3 lookat(0,0,0);
		float dist_to_focus = 10.0f;
		float aperture = 0.1f;
		camera cam(lookfrom, lookat, vec3(0,1,0), 20, float(nx) / float(ny), aperture, dist_to_focus);
		for (int j = ny - 1; j >= 0; j--) {
			for (int i = 0; i < nx; i++) {
				vec3 col(0, 0, 0);
				for (int s = 0; s < ns;  s++) {
					float u = float(i + random_double()) / float(nx);
					float v = float(j + random_double()) / float(ny);
					ray r = cam.get_ray(u, v);
					col += color(r, world, 0);
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