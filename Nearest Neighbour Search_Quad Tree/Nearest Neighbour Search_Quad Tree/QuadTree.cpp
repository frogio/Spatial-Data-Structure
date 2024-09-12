#include <queue>
#include "glut.h"
#include "QuadTree.h"

void QuadTree::buildTree(vector<Particle*>& particles, int w, int h) {
	
	queue<TreeNode*> queue;
	Vector2D min = Vector2D(0, 0);
	Vector2D max = Vector2D(w, h);

	root = new TreeNode(particles, min, max, 1);
	queue.push(root);
	while (queue.empty() == false) {																// bfs로 순회하면서 Tree를 구성한다.
		TreeNode* curNode = queue.front();
		renderTree.push_back(curNode);
		queue.pop();

		if (curNode->particles.size() > particleLimit && curNode->depth < depthLimit) {				// 입자 개수가 임계 개수보다 크고, 임계 깊이보다 낮을 때
			curNode->subdivide();																	// 분할
			
			for (int i = 0; i < 4; i++)
				queue.push(curNode->nodes[i]);														// 분할 후 자식 노드를 queue에 집어넣는다.

			curNode->particles.clear();																// 분할 후에 모든 파티클을 비운다.
		}

	}
	

}


void QuadTree::RenderTree() {
	for (int i = 0; i < renderTree.size(); i++) {
		TreeNode* node = renderTree[i];
		
		glBegin(GL_LINES);

		glColor3f(1.f, 1.f, 1.f);

		if (node->isInRange)
			glColor3f(0.f, 0.f, 1.f);
		glVertex2d(node->max.x, node->max.y);
		glVertex2d(node->min.x, node->max.y);

		glVertex2d(node->min.x, node->max.y);
		glVertex2d(node->min.x, node->min.y);

		glVertex2d(node->min.x, node->min.y);
		glVertex2d(node->max.x, node->min.y);

		glVertex2d(node->max.x, node->min.y);
		glVertex2d(node->max.x, node->max.y);
		//}
		glEnd();
	}
}

void QuadTree::query(Particle& particle, double radius) {

	Vector2D pos = particle.Pos;
	queue<TreeNode*> queue;

	queue.push(root);

	while (queue.empty() == false) {														// QuadTree를 BFS로 순회하며 최근접 이웃을 찾는다.
		TreeNode* curNode = queue.front();
		queue.pop();

			// 먼저 탐색 범위를 포함하거나 교차한 노드부터 찾고 큐에 삽입한다.
			if (IsContainRange(curNode, pos, radius) || IsIntersect(curNode, pos, radius)) {		// node가 range를 포함하거나 교차할 경우
				curNode->isInRange = true;

				if (curNode->HasChildNode()) {														// 리프노드가 아닐경우
					for(int i = 0; i < 4; i++)
						queue.push(curNode->nodes[i]);												// 자식 노드를 큐에 삽입
				}
				else {																				// 리프노드일 경우, 최근접 이웃을 찾는다.
					vector<Particle*>& curNodeParticle = curNode->particles;

					for (int i = 0; i < curNodeParticle.size(); i++) {
						Particle* p = curNodeParticle[i];
						double sdf = (p->Pos.x - pos.x) * (p->Pos.x - pos.x) +
										(p->Pos.y - pos.y) * (p->Pos.y - pos.y) - radius * radius;

						if (sdf <= 0)
							p->isInRange = true;
						else
							p->isActive = true;

				}

			}

		}
	}

}	

void QuadTree::query2(Particle& particle, double radius) {

	Vector2D pos = particle.Pos;
	queue<TreeNode*> queue;

	queue.push(root);

	while (queue.empty() == false) {															// QuadTree를 BFS로 순회하며 최근접 이웃을 찾는다.
		TreeNode* curNode = queue.front();
		queue.pop();
		// 먼저 탐색 범위를 포함하거나 교차한 노드부터 찾고 큐에 삽입한다.
		if (curNode->HasChildNode()) {															// 자식 노드가 있고
			for (int i = 0; i < 4; i++) {
				TreeNode* node = curNode->nodes[i];												// 자식 노드 i 에 대해

				if (IsContainRange(node, pos, radius) || IsIntersect(node, pos, radius)) {		// 자식 노드 i가 범위를 내포하거나 교차하면
					node->isInRange = true;
					queue.push(node);															// 자식 노드를 큐에 집어넣는다.
				}
			}
		}		// HasChildNode
		else {																					// 리프노드이면
			vector<Particle*>& curNodeParticle = curNode->particles;							// 거리를 계산한다.

			for (int i = 0; i < curNodeParticle.size(); i++) {
				Particle* p = curNodeParticle[i];
				double sdf = (p->Pos.x - pos.x) * (p->Pos.x - pos.x) +
					(p->Pos.y - pos.y) * (p->Pos.y - pos.y) - radius * radius;

				if (sdf <= 0)
					p->isInRange = true;
				else
					p->isActive = true;
			}
		}
	}




}

bool QuadTree::IsContainRange(TreeNode* node, Vector2D pos, double radius) {

	double maxx = pos.x + radius;
	double maxy = pos.y + radius;
	double minx = pos.x - radius;
	double miny = pos.y - radius;

	return node->max.x >= maxx && node->max.y >= maxy && node->min.x <= minx && node->min.y <= miny;

}
bool QuadTree::IsIntersect(TreeNode* node, Vector2D pos, double radius) {

	// 사각형과 원의 충돌 알고리즘...
	Vector2D squareMin = node->min;
	Vector2D squareMax = node->max;

	double dist = 0;

	if (pos.x < squareMin.x) 
		dist += (squareMin.x - pos.x) * (squareMin.x - pos.x);

	else if(pos.x > squareMax.x)
		dist += (pos.x - squareMax.x) * (pos.x - squareMax.x);
	
	if (pos.y < squareMin.y)
		dist += (squareMin.y - pos.y) * (squareMin.y - pos.y);

	else if (pos.y > squareMax.y)
		dist += (pos.y - squareMax.y) * (pos.y - squareMax.y);


	return dist <= radius * radius;

}

TreeNode::TreeNode(vector<Particle *> & _particles,Vector2D _min, Vector2D _max, int _depth) : particles(_particles), min(_min), max(_max), depth(_depth){}

TreeNode::TreeNode(Vector2D _min, Vector2D _max, int _depth) : min(_min), max(_max), depth(_depth), isInRange(false){}

bool TreeNode::HasChildNode() {
	return nodes[0] != NULL;
}

void TreeNode::subdivide() {
	
	Vector2D center = (max + min) / 2;

	Vector2D quadrant1Min = center;
	Vector2D quadrant1Max = max;

	nodes[0] = new TreeNode(quadrant1Min, quadrant1Max, depth + 1);

	for (int i = 0; i < particles.size(); i++) {
		Vector2D pos = particles[i]->Pos;

		if (quadrant1Min.x <= pos.x && pos.x <= quadrant1Max.x &&
			quadrant1Min.y <= pos.y && pos.y <= quadrant1Max.y)
			nodes[0]->particles.push_back(particles[i]);

	}

	Vector2D quadrant2Min(min.x, center.y);
	Vector2D quadrant2Max(center.x, max.y);

	nodes[1] = new TreeNode(quadrant2Min, quadrant2Max, depth + 1);

	for (int i = 0; i < particles.size(); i++) {
		Vector2D pos = particles[i]->Pos;

		if (quadrant2Min.x <= pos.x && pos.x <= quadrant2Max.x &&
			quadrant2Min.y <= pos.y && pos.y <= quadrant2Max.y)
			nodes[1]->particles.push_back(particles[i]);

	}

	Vector2D quadrant3Min = min;
	Vector2D quadrant3Max = center;

	nodes[2] = new TreeNode(quadrant3Min, quadrant3Max, depth + 1);

	for (int i = 0; i < particles.size(); i++) {
		Vector2D pos = particles[i]->Pos;

		if (quadrant3Min.x <= pos.x && pos.x <= quadrant3Max.x &&
			quadrant3Min.y <= pos.y && pos.y <= quadrant3Max.y)
			nodes[2]->particles.push_back(particles[i]);

	}

	Vector2D quadrant4Min(center.x, min.y);
	Vector2D quadrant4Max(max.x, center.y);

	nodes[3] = new TreeNode(quadrant4Min, quadrant4Max, depth + 1);

	for (int i = 0; i < particles.size(); i++) {
		Vector2D pos = particles[i]->Pos;

		if (quadrant4Min.x <= pos.x && pos.x <= quadrant4Max.x &&
			quadrant4Min.y <= pos.y && pos.y <= quadrant4Max.y)
			nodes[3]->particles.push_back(particles[i]);

	}

}