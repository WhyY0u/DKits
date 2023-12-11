#pragma once
#include <cmath>
class vec2 {
public:
	float x, y;
	vec2() {};
	vec2(float x, float y) : x(x), y(y) {}
	vec2(float i) : x(i), y(i){}

};
class vec3 {
public:
	float x, y, z;
	vec3(){};
	vec3(float x, float y, float z) : x(x), y(y), z(z) {}
	vec3(float i) : x(i), y(i), z(i) {}

};
class vec4 {
public:
	float x, y, z, w;
	vec4() {};
	vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {};
	vec4(float i) : x(i), y(i), z(i), w(i) {}

};