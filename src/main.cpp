#include <iostream>
#include <fstream>

#include "vec3.h"
#include "ray.h"

using namespace std;
const char* filename = "outputimg.ppm";

vec3 color(const ray& r)
{
	vec3 unit_direction = unit_vector(r.direction());
	float t = 0.5f * (unit_direction.y() + 1.0f);
	return (1.0f - t) * vec3(1.0f, 1.0f, 1.0f) + t * vec3(0.5f, 0.7f, 1.0f);
}

int main()
{
	vec3 a(2.0f, 2.0f, 2.0f);
	vec3 b(4.0f, 4.0f, 2.0f);
	vec3 c = a + b;
	//cout << c;
	fstream outstream;
	outstream.open(filename, ios::out);

	if (outstream.is_open()) {
		int nx = 200;
		int ny = 100;

		outstream << "P3\n" << nx << " " << ny << "\n255\n";

		vec3 lower_left_corner(-2.0f, -1.0f, -1.0f);
		vec3 horizontal(4.0f, 0.0f, 0.0f);
		vec3 vertical(0.0f, 2.0f, 0.0f);
		vec3 origin(0.0f, 0.0f, 0.0f);
		vec3 dir;
		for (int j = ny - 1; j >= 0; --j) {
			for (int i = 0; i < nx; ++i) {
				float u = float(i) / float(nx);
				float v = float(j) / float(ny);
				dir = lower_left_corner + u*horizontal + v*vertical;
				ray r(origin, dir);
				vec3 col = color(r);
				int ir = int(255.99 * col[0]);
				int ig = int(255.99 * col[1]);
				int ib = int(255.99 * col[2]);
				//cout << ir << " " << ig << " " << ib << "\n";
				outstream << ir << " " << ig << " " << ib << "\n";
			}
		}
	}

	//outstream.close();
	cout << "done";
	cin.get();
	return 0;
}