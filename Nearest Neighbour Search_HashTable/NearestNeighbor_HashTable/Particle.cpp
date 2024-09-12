#include "Particle.h"

void Particle::Draw() {
	
	glBegin(GL_POINTS);
	
	glColor3f(1.f, 1.f, 1.f);

	if (isActive)
		glColor3f(1.f, 0.f, 0.f);

	if (isInRange){
		glPointSize(10.f);
		glColor3f(1.f, 1.f, 0.f);
	}
	/*
	switch (range) {		// FOR DEBUG IN RESOLUTION 3 * 3
	case 0:
		glColor3f(1.f, 0.f, 0.f);
		break;
	case 1:
		glColor3f(1.f, 1.f, 0.f);
		break;
	case 2:
		glColor3f(1.f, 1.f, 1.f);
		break;
	case 3:
		glColor3f(1.f, 0.f, 1.F);
		break;
	case 4:
		glColor3f(0.f, 0.f, 1.F);
		break;
	case 5:
		glColor3f(0.f, 1.f, 1.F);
		break;
	case 6:
		glColor3f(0.f, 1.f, 0.F);
		break;
	case 7:
		glColor3f(1.f, 0.f, 0.F);
		break;
	case 8:
		glColor3f(0.5f, 0.5f, 0.5F);
		break;
	}
	*/
	glVertex2f(x, y);
	
	glEnd();

}