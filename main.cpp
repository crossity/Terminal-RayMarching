#include <iostream>
#include <cstdio>
#include <cmath>

#define Width  80
#define Height 23

using namespace std;

const char gradient[] = "@+:-. ";

struct vec2 {
	float x, y;
	vec2() {
		this->x = 0;
		this->y = 0;
	}
	vec2(float a, float b) {
		this->x = a;
		this->y = b;
	}
};

struct vec3 {
	float x, y, z;
	vec3() {
		this->x = 0;
		this->y = 0;
		this->z = 0;
	}
	vec3(float a, float b, float c) {
		this->x = a;
		this->y = b;
		this->z = c;
	}
	void print() {
		cout << this->x << ", " << this->y << ", " << this->z << endl;
	}
	vec3 operator/(float a) {
        return vec3(this->x / a, this->y / a, this->z / a);
    }
    vec3 operator*(float a) {
        return vec3(this->x * a, this->y * a, this->z * a);
    }
    vec3 operator+(float a) {
        return vec3(this->x + a, this->y + a, this->z + a);
    }
    vec3 operator+(vec3 a) {
        return vec3(this->x + a.x, this->y + a.y, this->z + a.z);
    }
    vec3 operator-(vec3 a) {
        return vec3(this->x - a.x, this->y - a.y, this->z - a.z);
    }
};

float length(vec3 p1, vec3 p2);
vec3 norm(vec3 v);
vec3 vec_dir(vec3 p1, vec3 p2);

struct sphere {
	vec3 center;
	float r;
	int color;
	sphere() {
		this->center = vec3();
		this->r = 1.f;
		this->color = 0;
	}
	sphere(vec3 a, float b, int c) {
		this->center = a;
		this->r = b;
		this->color = c;
	}
	float dist(vec3 p) {
		return length(p, this->center) - this->r;
	}
};

struct ground {
	float y;
	int color;
	ground() {
		this->y = 0;
		this->color = '#';
	}
	ground(float a, int b) {
		this->y = a;
		this->color = b;
	}
	float dist(vec3 p) {
		return p.y - this->y;
	}
};

void ClearScreen(char *Screen, char bg);

void putpix(char *Screen, vec2 p, char color);

int main() {
	char Screen[Height * Width + 1];
	Screen[Height * Width] = 0;
	ClearScreen(Screen, '.');

	sphere s = sphere(vec3(0, 0, 3.f), 1.f, 0);
	ground g = ground(-1.f, 2);
	vec3 light = vec3(2, 2, 0);

	for (int i = 0; i < Height; i++) {
		for (int j = 0; j < Width; j++) {
			vec2 uv = vec2((float)j / Width * 2.f - 1.f, (float)i / Height * 2.f - 1.f);
			uv.x *= Width / Height;
			uv.x *= 8.f / 13.f;
			uv.y *= -1.f;
			vec3 dir = norm(vec3(uv.x, uv.y, 1));
			int color = 5;
			float step = 0.1f;
			for (; step < 8.f; ) {
				float dist = min(s.dist(dir * step), g.dist(dir * step));
				if (dist < 0.000001f) {
					vec3 ref_place = dir * step;
					if (s.dist(dir * step) < g.dist(dir * step))
						color = s.color;
					else
						color = g.color;

					dir = vec_dir(ref_place, light);
					step = 0.1f;
					while (length(light, ref_place + dir * step) > 1.f && step < 8.f) {
						dist = min(s.dist(ref_place + dir * step), g.dist(ref_place + dir * step));
						if (dist < 0.0001f) {
							color++;
							break;
						}
						step += dist;
					}
					break;	
				}
				step += dist;
			}
			putpix(Screen, vec2(j, i), gradient[color]);
		}
	}
	
	printf(Screen);
	return 0;
}

void ClearScreen(char *Screen, char bg) {
	for (int i = 0; i < Height * Width; i++)
		Screen[i] = bg;
}

void putpix(char *Screen, vec2 p, char color) {
	if (p.x < Width && p.x >= 0 && p.y < Height && p.y >= 0)
		Screen[(int)p.y * Width + (int)p.x] = color;
}

float length(vec3 p1, vec3 p2) {
	return sqrt((p1.x - p2.x)*(p1.x - p2.x) + 
		(p1.y - p2.y)*(p1.y - p2.y) +
		(p1.z - p2.z)*(p1.z - p2.z));
}

vec3 norm(vec3 v) {
	return v / length(vec3(), v);
}

vec3 vec_dir(vec3 p1, vec3 p2) {
	return norm(vec3(p2.x - p1.x, p2.y - p1.y, p2.z - p1.z));
}