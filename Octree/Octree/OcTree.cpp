#include "OcTree.h"

OcTree::OcTree(vector<Vector*>& vertices, int _depthLimit, int _particleLimit, Vector max, Vector min) :
depthLimit(_depthLimit), particleLimit(_particleLimit)
{
	root = new TreeNode(vertices, max, min, 1);
}

void OcTree::buildTree() {
	queue<TreeNode*> queue;
	queue.push(root);

	while (queue.empty() == false) {
		TreeNode* curNode = queue.front();
		renderTree.push_back(curNode);
		queue.pop();

		if (curNode->depth < depthLimit && curNode->vertices.size() > particleLimit) {			// 노드가 임계치를 넘었을 경우
			curNode->subdivide();																// 분할한다.

			for (int i = 0; i < 8; i++)
				queue.push(curNode->nodes[i]);													// 분할한 노드를 삽입
			
			curNode->vertices.clear();
		}
	}
}

void OcTree::RenderTree(bool onlyleaf) {

	glColor3d(0, 0, 1);
	glDisable(GL_LIGHT0);
	glBegin(GL_LINES);

	 
	for (int i = 0; i < renderTree.size(); i++) {

		if (onlyleaf && renderTree[i]->vertices.size() == 0) {		// 리프노드만 그리며, 자식노드가 존재할 경우 생략
			continue;
		}

		Vector max = renderTree[i]->max;
		Vector min = renderTree[i]->min;
		 
		glVertex3d(max.x, max.y, max.z);
		glVertex3d(min.x, max.y, max.z);

		glVertex3d(min.x, max.y, max.z);
		glVertex3d(min.x, min.y, max.z);

		glVertex3d(min.x, min.y, max.z);
		glVertex3d(max.x, min.y, max.z);
	
		glVertex3d(max.x, min.y, max.z);
		glVertex3d(max.x, max.y, max.z);
		// 1	
		glVertex3d(max.x, max.y, min.z);
		glVertex3d(min.x, max.y, min.z);

		glVertex3d(min.x, max.y, min.z);
		glVertex3d(min.x, min.y, min.z);

		glVertex3d(min.x, min.y, min.z);
		glVertex3d(max.x, min.y, min.z);

		glVertex3d(max.x, min.y, min.z);
		glVertex3d(max.x, max.y, min.z);
		// 2

		glVertex3d(max.x, max.y, max.z);
		glVertex3d(max.x, max.y, min.z);

		glVertex3d(max.x, min.y, max.z);
		glVertex3d(max.x, min.y, min.z);
		
		glVertex3d(min.x, max.y, max.z);
		glVertex3d(min.x, max.y, min.z);

		glVertex3d(min.x, min.y, max.z);
		glVertex3d(min.x, min.y, min.z);
		// 3
		}

	glEnd();
	glEnable(GL_LIGHT0);

}

TreeNode::TreeNode(vector<Vector*>& _vertices, Vector _max, Vector _min, int _depth) : vertices(_vertices), max(_max), min(_min), depth(_depth) {}
TreeNode::TreeNode(Vector _max, Vector _min, int _depth) : max(_max), min(_min), depth(_depth) {}

void TreeNode::subdivide() {

	Vector center = (max + min) / 2;

	Vector box1Min = center;
	Vector box1Max = max;
	
	nodes[0] = new TreeNode(box1Max, box1Min, depth + 1);

	for (int i = 0; i < vertices.size(); i++) {
		Vector* vertex = vertices[i];

		if (box1Min.x <= vertex->x && vertex->x <= box1Max.x &&
			box1Min.y <= vertex->y && vertex->y <= box1Max.y &&
			box1Min.z <= vertex->z && vertex->z <= box1Max.z)
			nodes[0]->vertices.push_back(vertex);
	}

	Vector box2Min = Vector(min.x, min.y, center.z);
	Vector box2Max = Vector(center.x, max.y, max.z);;

	nodes[1] = new TreeNode(box2Max, box2Min, depth + 1);

	for (int i = 0; i < vertices.size(); i++) {
		Vector* vertex = vertices[i];

		if (box2Min.x <= vertex->x && vertex->x <= box2Max.x &&
			box2Min.y <= vertex->y && vertex->y <= box2Max.y &&
			box2Min.z <= vertex->z && vertex->z <= box2Max.z)
			nodes[1]->vertices.push_back(vertex);
	}

	Vector box3Min = Vector(min.x, center.y, min.z);
	Vector box3Max = Vector(center.x, max.y, center.z);

	nodes[2] = new TreeNode(box3Max, box3Min, depth + 1);

	for (int i = 0; i < vertices.size(); i++) {
		Vector* vertex = vertices[i];

		if (box3Min.x <= vertex->x && vertex->x <= box3Max.x &&
			box3Min.y <= vertex->y && vertex->y <= box3Max.y &&
			box3Min.z <= vertex->z && vertex->z <= box3Max.z)
			nodes[2]->vertices.push_back(vertex);
	}

	Vector box4Min = Vector(center.x, center.y, min.z);
	Vector box4Max = Vector(max.x, max.y, center.z);

	nodes[3] = new TreeNode(box4Max, box4Min, depth + 1);

	for (int i = 0; i < vertices.size(); i++) {
		Vector* vertex = vertices[i];

		if (box4Min.x <= vertex->x && vertex->x <= box4Max.x &&
			box4Min.y <= vertex->y && vertex->y <= box4Max.y &&
			box4Min.z <= vertex->z && vertex->z <= box4Max.z)
			nodes[3]->vertices.push_back(vertex);
	}

	Vector box5Min = Vector(center.x, min.y ,center.z);
	Vector box5Max = Vector(max.x, center.y, max.z);

	nodes[4] = new TreeNode(box5Max, box5Min, depth + 1);

	for (int i = 0; i < vertices.size(); i++) {
		Vector* vertex = vertices[i];

		if (box5Min.x <= vertex->x && vertex->x <= box5Max.x &&
			box5Min.y <= vertex->y && vertex->y <= box5Max.y &&
			box5Min.z <= vertex->z && vertex->z <= box5Max.z)
			nodes[4]->vertices.push_back(vertex);
	}

	Vector box6Min = Vector(min.x, min.y, center.z);
	Vector box6Max = Vector(center.x, center.y, max.z);

	nodes[5] = new TreeNode(box6Max, box6Min, depth + 1);

	for (int i = 0; i < vertices.size(); i++) {
		Vector* vertex = vertices[i];

		if (box6Min.x <= vertex->x && vertex->x <= box6Max.x &&
			box6Min.y <= vertex->y && vertex->y <= box6Max.y &&
			box6Min.z <= vertex->z && vertex->z <= box6Max.z)
			nodes[5]->vertices.push_back(vertex);
	}

	Vector box7Min = min;
	Vector box7Max = center;

	nodes[6] = new TreeNode(box7Max, box7Min, depth + 1);

	for (int i = 0; i < vertices.size(); i++) {
		Vector* vertex = vertices[i];

		if (box7Min.x <= vertex->x && vertex->x <= box7Max.x &&
			box7Min.y <= vertex->y && vertex->y <= box7Max.y &&
			box7Min.z <= vertex->z && vertex->z <= box7Max.z)
			nodes[6]->vertices.push_back(vertex);
	}

	Vector box8Min = Vector(center.x, min.y, min.z);
	Vector box8Max = Vector(max.x, center.y, center.z);

	nodes[7] = new TreeNode(box8Max, box8Min, depth + 1);

	for (int i = 0; i < vertices.size(); i++) {
		Vector* vertex = vertices[i];

		if (box8Min.x <= vertex->x && vertex->x <= box8Max.x &&
			box8Min.y <= vertex->y && vertex->y <= box8Max.y &&
			box8Min.z <= vertex->z && vertex->z <= box8Max.z)
			nodes[7]->vertices.push_back(vertex);
	}

}
