#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include "Particle.h"
#include "HashTable.h"

#include "glut.h"

#define MAX_PARTICLE_COUNT		10000

using namespace std;

vector<Particle> particles;
HashTable * Hash;

void init();
void glReshape(int w, int h);
void glDisplay();

void main() {

	init();

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(700, 700);
	glutCreateWindow("Hash Table");
	glutReshapeFunc(glReshape);
	glutDisplayFunc(glDisplay);
	glutMainLoop();
}

void glReshape(int w, int h) {
	glLoadIdentity();						// �������
	glViewport(0, 0, w, h);							// ����Ʈ ���� (�����쿡 �׸� ����)
	gluOrtho2D(-100.f, 100.f, -100.f, 100.f);		// ��ǥ�� ������, X : -100 ~ 100, Y : -100 ~ 100
}

void glDisplay() {

	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0f, 1.0f, 1.0f);

	for (int i = 0; i < particles.size(); i++)
		particles[i].Draw();

	glFlush();

}

void init() {

	srand((unsigned)time(NULL));

	for (int i = 0; i < MAX_PARTICLE_COUNT; i++) {
		float x, y;

		x = ((rand() % 10000) / 10000.f * 200) - 100;
		y = ((rand() % 10000) / 10000.f * 200) - 100;

		particles.push_back(Particle(x, y));

	}

	Hash = new HashTable(particles, 30);
	Hash->Query(particles[3306], 3, 3, 15.f);
}