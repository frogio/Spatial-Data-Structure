#pragma once
#include "glut.h"

class Vector2D {
public:
	double x;
	double y;

	Vector2D() : x(0), y(0) {}

	Vector2D(double _x, double _y) : x(_x), y(_y) {}

	Vector2D operator+(const Vector2D & v) {
		Vector2D res(x + v.x, y + v.y);
		return res;
	}

	Vector2D operator-(const Vector2D& v) {
		Vector2D res(x - v.x, y - v.y);
		return res;
	}

	Vector2D operator*(const double scalar) {
		Vector2D res(x * scalar, y * scalar);
		return res;
	}

	Vector2D operator/(const double scalar) {
		Vector2D res(x / scalar, y / scalar);
		return res;
	}

	double GetSize() {
		return x * x + y * y;
	}
	
	Vector2D GetUnitVector() {	
		Vector2D unit(x / GetSize(), y / GetSize());
		return unit;
	}
};

class Particle {
public:
	Vector2D Pos;
	bool isActive;
	bool isInRange;
	
	Particle(Vector2D _Pos) : Pos(_Pos), isActive(false), isInRange(false) {}

	void Draw() {
		glBegin(GL_POINTS);
	
		glColor3f(1.f, 1.f, 1.f);

		if (isActive)
			glColor3f(1.f, 0.f, 0.f);

		if(isInRange)
			glColor3f(1.f, 1.f, 0.f);

		glVertex2d(Pos.x, Pos.y);

		glEnd();
	}

};