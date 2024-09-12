#pragma once
#include "glut.h"

class Particle {
public:
	float x;
	float y;

	int range;

	bool isActive;
	bool isInRange;

	Particle(float _x, float _y) : x(_x), y(_y), isActive(false), isInRange(false){}
	
	void Draw();
	void SetRange(int _range) {
		range = _range;
	}

};