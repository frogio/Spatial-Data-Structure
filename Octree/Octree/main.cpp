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
	// 깊이값 사용 여부
	glEnable(GL_DEPTH_TEST);
	printf("press p key to switch point mode\n");
	printf("press s key to switch solid mode\n");
	printf("press l key to render only leaf node\n");

	tree->buildTree();
}


void draw() {
	// 새로운 행렬 연산을 조명에 적용한다. (조명 방향을 회전시킨다.)
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// 현재 행렬연산을 지우고 다시 원래의 행렬로 돌려놓는다.
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
	// 0.6f, 0.6f, 0.6f의 회색 색상으로 배경을 초기화

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// 더블버퍼링 설정
	glLoadIdentity();
	// 단위행렬을 불러옴, (행렬 연산의 초기값)

	glPushMatrix();

	glTranslatef(0.0f, 0.0f, -_zoom);
	glTranslatef(_translate_x, _translate_y, 0.0f);
	// ModelVIew로 기준 좌표계의 위치를 바꾼다. 
	// Translate를 이용해 이동 행렬 연산 수행
	// PushMatrix 이전에 수행된 행렬을 Push한다. 연산 대상은  메서드로 그린 오브젝트

	glRotatef(_rotate_x, 1, 0, 0);
	glRotatef(_rotate_y, 0, 1, 0);
	// 마우스로 회전함 따라서 z방향으로 회전하는 코드는 넣지 않음.
	// rotate_x, rotate_y 값에 따라 x, y로 회전
	// rotate_illumination의 값에 따라 true 이면 조명이 회전연산의 대상, 반대로 false이면 loadObj로 불러온 오브젝트가 회전 연산 대상.


	draw();
	// 새로운 행렬 연산을 생성한다

	glPopMatrix();

	glutSwapBuffers();
	// 더블 버퍼링

}

void reshape(int w, int h) {
	if (w == 0)
		h = 1;

	glViewport(0, 0, w, h);	// 그려질 스케치북의 사이즈
	glMatrixMode(GL_PROJECTION);
	// 연산 대상이 되는 행렬 선택 (카메라 좌표 행렬)
	glLoadIdentity();
	// 초기화
	gluPerspective(90.0f, (float)w / h, 0.1f, 100.0f);
	// Perspective 행렬 연산, 90도의 fov, 종횡비, near, far값 (카메라 위치)

	glMatrixMode(GL_MODELVIEW);
	// 연산 대상이 되는 행렬 선택 (모델의 좌표 행렬을 연산 대상으로 삼음.)	

	// OpenGL의 기본 설정
}

void mouseMotion(int x, int y) {

	int diff_x = x - last_x;
	int diff_y = y - last_y;
	// 마우스를 드래그 할 때 거리값 계산

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
	// 마우스를 클릭할 때 최초의 좌표

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
	// 변경된 _zoom 값과 _translate된 좌표값으로 Redisplay 실행
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
		// 초기화 함수
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

	// Mesh 생성 즉시 loadObj 함수를 호출.

	glutInitWindowSize(640, 480);			// 창 크기
	glutInitWindowPosition(100, 100);		// 창 첫 위치
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutCreateWindow("Camera Navigation");
	glutDisplayFunc(display);				// display callback 함수
	glutReshapeFunc(reshape);				// reshape callback 함수 // 최소화나 최대화 등 창의 상태가 변경될 경우 다시 그림 
	glutMouseFunc(Mouse);					// 마우스 콜백 함수
	glutMotionFunc(mouseMotion);			// 마우스 모션(드래그) 콜백 함수
	glutKeyboardFunc(keyboard);
	init();									// 각종 클래스, 변수 초기화
	glutMainLoop();
}

