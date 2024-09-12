#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <vector>
#include "glut.h"
#include "Particle.h"
#include "QuadTree.h"

#define MAX_PARTICLES			10000

using namespace std;

void display();
void reshape(int w, int h);
void Init();
void keyboard(unsigned char key, int x, int y);

int pointIndex = 3306;

vector<Particle *> particles;
QuadTree* quadTree;

int main()
{

	printf("Press A key to switch pivot position\n");
	Init();

	glutInitWindowSize(700, 700);			// 창 크기
	glutInitWindowPosition(100, 100);		// 창 첫 위치
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutCreateWindow("Nearest Neighbor Search - Quad Tree");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'A':
	case 'a':
		for (int i = 0; i < particles.size(); i++) {
			particles[i]->isInRange = false;
			particles[i]->isActive = false;
		}
		for (int i = 0; i < quadTree->renderTree.size(); i++) {
			TreeNode* node = quadTree->renderTree[i];
			node->isInRange = false;
		}

		pointIndex = rand() % particles.size();
		quadTree->query(*particles[pointIndex], 0.1);
		glutPostRedisplay();
		break;
	}

}

void display() {
	glClearColor(0.f, 0.f, 0.f, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	for (int i = 0; i < particles.size(); i++)
		particles[i]->Draw();
	
	quadTree->RenderTree();

	glFlush();
}

void reshape(int w, int h) {
	glLoadIdentity();						// 단위행렬
	glViewport(0, 0, w, h);	// 그려질 스케치북의 사이즈
	gluOrtho2D(0, 1, 0, 1);
}

void Init() {
	srand((unsigned)time(NULL));
	
	for (int i = 0; i < MAX_PARTICLES; i++) {
		double x = (rand() % 10000) / 10000.f;
		double y = (rand() % 10000) / 10000.f;

		double sdf1 = x * x + y * y - 0.4 * 0.4;
		double sdf2 = (1.0 - x) * (1.0 - x) + (1.0 - y) * (1.0 - y) - 0.4 * 0.4;
		double sdf3 = (0.5 - x) * (0.5 - x) + (0.5 - y) * (0.5 - y) - 0.3 * 0.3;

		double sdf = fmin(fmin(sdf1, sdf2), sdf3);

		if (sdf <= 0.0)
			particles.push_back(new Particle(Vector2D(x, y)));

	}
	quadTree = new QuadTree(7, 2);
	quadTree->buildTree(particles, 1, 1);
	//quadTree->query(*particles[3306], 0.3);
	quadTree->query2(*particles[pointIndex], 0.1);
}