#include <stdio.h>
#include <stdlib.h>
#include "glut.h"'
#include "Model.h"
#include "Octree.h"

float _zoom = 15.0f;
float _rotate_x = 0.0f;
float _rotate_y = 0.001f;
float _translate_x = 0.0f;
float _translate_y = 0.0f;
int last_x = 0;
int last_y = 0;

bool point = false;
bool onlyleaf = false;


unsigned char _btnState[3] = { 0, };

Model* model = new Model("obj\\buddha.obj");
OcTree* tree = new OcTree(model->GetVertices(),5, 100, model->max, model->min);

void init() {
	// ���̰� ��� ����
	glEnable(GL_DEPTH_TEST);
	printf("press p key to switch point mode\n");
	printf("press s key to switch solid mode\n");
	printf("press l key to render only leaf node\n");

	tree->buildTree();
}


void draw() {
	// ���ο� ��� ������ ���� �����Ѵ�. (���� ������ ȸ����Ų��.)
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// ���� ��Ŀ����� ����� �ٽ� ������ ��ķ� �������´�.
	if (point)
		model->DrawPoint();
	else
		model->DrawModel();

	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHTING);

	tree->RenderTree(onlyleaf); 

}


void display() {
	glClearColor(0.6f, 0.6f, 0.6f, 1.0f);
	// 0.6f, 0.6f, 0.6f�� ȸ�� �������� ����� �ʱ�ȭ

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// ������۸� ����
	glLoadIdentity();
	// ��������� �ҷ���, (��� ������ �ʱⰪ)

	glPushMatrix();

	glTranslatef(0.0f, 0.0f, -_zoom);
	glTranslatef(_translate_x, _translate_y, 0.0f);
	// ModelVIew�� ���� ��ǥ���� ��ġ�� �ٲ۴�. 
	// Translate�� �̿��� �̵� ��� ���� ����
	// PushMatrix ������ ����� ����� Push�Ѵ�. ���� �����  �޼���� �׸� ������Ʈ

	glRotatef(_rotate_x, 1, 0, 0);
	glRotatef(_rotate_y, 0, 1, 0);
	// ���콺�� ȸ���� ���� z�������� ȸ���ϴ� �ڵ�� ���� ����.
	// rotate_x, rotate_y ���� ���� x, y�� ȸ��
	// rotate_illumination�� ���� ���� true �̸� ������ ȸ�������� ���, �ݴ�� false�̸� loadObj�� �ҷ��� ������Ʈ�� ȸ�� ���� ���.


	draw();
	// ���ο� ��� ������ �����Ѵ�

	glPopMatrix();

	glutSwapBuffers();
	// ���� ���۸�

}

void reshape(int w, int h) {
	if (w == 0)
		h = 1;

	glViewport(0, 0, w, h);	// �׷��� ����ġ���� ������
	glMatrixMode(GL_PROJECTION);
	// ���� ����� �Ǵ� ��� ���� (ī�޶� ��ǥ ���)
	glLoadIdentity();
	// �ʱ�ȭ
	gluPerspective(90.0f, (float)w / h, 0.1f, 100.0f);
	// Perspective ��� ����, 90���� fov, ��Ⱦ��, near, far�� (ī�޶� ��ġ)

	glMatrixMode(GL_MODELVIEW);
	// ���� ����� �Ǵ� ��� ���� (���� ��ǥ ����� ���� ������� ����.)	

	// OpenGL�� �⺻ ����
}

void mouseMotion(int x, int y) {

	int diff_x = x - last_x;
	int diff_y = y - last_y;
	// ���콺�� �巡�� �� �� �Ÿ��� ���

	if (_btnState[2]) {
		_zoom -= (float)0.05f * diff_x;
	}
	else if (_btnState[0]) {
		_rotate_x += (float)0.05f * diff_y;
		_rotate_y += (float)0.05f * diff_x;
	}
	else if (_btnState[1]) {
		_translate_x += (float)0.005f * diff_x;
		_translate_y -= (float)0.005f * diff_y;
	}

	glutPostRedisplay();
}

void Mouse(int button, int state, int x, int y) {
	last_x = x;
	last_y = y;
	// ���콺�� Ŭ���� �� ������ ��ǥ

	switch (button) {
	case GLUT_LEFT_BUTTON:
		printf("left button\n");
		_btnState[0] = ((GLUT_DOWN == state) ? 1 : 0);
		break;
	case GLUT_MIDDLE_BUTTON:
		printf("middle button\n");
		_btnState[1] = ((GLUT_DOWN == state) ? 1 : 0);
		break;
	case GLUT_RIGHT_BUTTON:
		printf("right button\n");
		_btnState[2] = ((GLUT_DOWN == state) ? 1 : 0);
		break;
	}

	glutPostRedisplay();
	// ����� _zoom ���� _translate�� ��ǥ������ Redisplay ����
}


void keyboard(unsigned char key, int x, int y) {

	switch (key) {
	case 'Q':
	case 'q':
		printf("quit\n");
		exit(0);
		break;

	case 'I':
	case 'i':
		printf("initialzation\n");
		_zoom = 15.0f;
		_rotate_x = 0.0f;
		_rotate_y = 0.001f;
		_translate_x = 0.0f;
		_translate_y = 0.0f;
		last_x = 0;
		last_y = 0;
		break;
		// �ʱ�ȭ �Լ�
	case 'S':
	case 's':
		point = false;
		break;
	case 'P':
	case 'p':
		point = true;
		break;

	case 'L':
	case 'l':
		onlyleaf = !onlyleaf;
		break;
	}

	glutPostRedisplay();

}
void main() {

	// Mesh ���� ��� loadObj �Լ��� ȣ��.

	glutInitWindowSize(640, 480);			// â ũ��
	glutInitWindowPosition(100, 100);		// â ù ��ġ
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutCreateWindow("Camera Navigation");
	glutDisplayFunc(display);				// display callback �Լ�
	glutReshapeFunc(reshape);				// reshape callback �Լ� // �ּ�ȭ�� �ִ�ȭ �� â�� ���°� ����� ��� �ٽ� �׸� 
	glutMouseFunc(Mouse);					// ���콺 �ݹ� �Լ�
	glutMotionFunc(mouseMotion);			// ���콺 ���(�巡��) �ݹ� �Լ�
	glutKeyboardFunc(keyboard);
	init();									// ���� Ŭ����, ���� �ʱ�ȭ
	glutMainLoop();
}

