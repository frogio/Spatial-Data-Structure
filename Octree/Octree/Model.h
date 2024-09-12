#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include "glut.h"

#define INF	0x7f7f7f7f

#pragma warning(disable:4996) 

using namespace std;

class Vector {
public:
	double x;
	double y;
	double z;

	Vector() : x(0), y(0), z(0) {}
	Vector(double _x, double _y, double  _z) : x(_x), y(_y), z(_z) {}

	Vector operator+(const Vector& v) {
		return Vector(x + v.x, y + v.y, z + v.z);
	}
	Vector operator-(const Vector& v) {
		return Vector(x - v.x, y - v.y, z - v.z);
	}
	Vector operator*(const double scalar) {
		return Vector(x * scalar, y * scalar, z * scalar);
	}
	Vector operator/(const double scalar) {
		return Vector(x / scalar, y / scalar, z / scalar);
	}

	double GetSize() {
		return sqrt(x * x + y * y + z * z);
	}

	Vector GetNormalizedVector() {
		return *this / GetSize();
	}

	Vector Cross(const Vector& v) {
		return Vector(y * v.z - z * v.y,
			z * v.x - x * v.z,
			x * v.y - y * v.x);
	}
};

class Face {
public:
	Vector* vertex[3];

	Face(Vector* v1, Vector* v2, Vector* v3) {
		vertex[0] = v1;
		vertex[1] = v2;
		vertex[2] = v3;
	}

};

class Model {
public:
	vector<Face> faces;
	vector<Vector*> vertices;
	Vector max;
	Vector min;

	Model(const char* obj_path);
	void DrawPoint();
	void DrawModel();

	vector<Vector*>& GetVertices() {
		return vertices;
	}

};