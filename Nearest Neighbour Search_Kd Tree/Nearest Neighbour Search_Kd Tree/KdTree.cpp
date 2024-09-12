#include "KdTree.h"


void KdTree::buildTree(vector<Particle *> & particles, double w, double h) {

	queue<TreeNode *> queue;
	Vector2D min(0, 0);
	Vector2D max(w, h);

	root = new TreeNode(particles, min, max, 1);
	queue.push(root);

	while (queue.empty() == false) {
	
		TreeNode* node = queue.front();
		renderTree.push_back(node);
		queue.pop();

		if (node->particles.size() > particleLimit) {
			node->subdivide();

			queue.push(node->nodes[0]);
			queue.push(node->nodes[1]);
	
			node->particles.clear();
		}

	}


}
void KdTree::query2(Particle* p, double radius) {

	Vector2D pos = p->Pos;

	queue<TreeNode*> queue;
	queue.push(root);

	while (queue.empty() == false) {
		TreeNode * curNode = queue.front();
		queue.pop();

		if (IsContainRange(curNode, p, radius) || IsIntersect(curNode, p, radius)) {
			
			if(HasChild(curNode)){						// 자식 노드가 존재할 경우
				for (int i = 0; i < 2; i++) {
					TreeNode* node = curNode->nodes[i];
					queue.push(node);
				}
			}
			else {										// 리프 노드일 경우
				for (int i = 0; i < curNode->particles.size(); i++) {
					Particle* p = curNode->particles[i];
					double dist = (pos.x - p->Pos.x) * (pos.x - p->Pos.x) + (pos.y - p->Pos.y) * (pos.y - p->Pos.y);

					if (dist <= radius * radius)
						p->isInRange = true;
					else
						p->isActive = true;
				}
			}
		}


	}

}


void KdTree::query(Particle* p, double radius) {
	
	Vector2D pos = p->Pos;

	queue<TreeNode*> queue;
	queue.push(root);

	while (queue.empty() == false) {
		TreeNode* curNode = queue.front();
		queue.pop();
	
		if (HasChild(curNode)) {	// 자식 노드가 존재할 경우
		
			for (int i = 0; i < 2; i++) {
				TreeNode* node = curNode->nodes[i];
				if (IsContainRange(node, p, radius) || IsIntersect(node, p, radius)) 		// 자식노드가 탐색 범위를 내포하거나 교차할 경우,
					queue.push(node);				// 큐에 삽입
			}

		}
		else {	// 리프노드일 경우 최근접 이웃을 찾는다.
			for (int i = 0; i < curNode->particles.size(); i++) {
				Particle* p = curNode->particles[i];
				double dist = (pos.x - p->Pos.x) * (pos.x - p->Pos.x) + (pos.y - p->Pos.y) * (pos.y - p->Pos.y);
				
				if (dist <= radius * radius)
					p->isInRange = true;
				else
					p->isActive = true;
			}
		
		}


	}

}
bool KdTree::HasChild(TreeNode* node) {
	return node->nodes[0] != NULL;
}

bool KdTree::IsContainRange(TreeNode* node, Particle* p, double radius) {

	double maxx = p->Pos.x + radius;
	double maxy = p->Pos.y + radius;
	double minx = p->Pos.x - radius;
	double miny = p->Pos.y - radius;

	return node->max.x >= maxx && node->max.y >= maxy && node->min.x <= minx && node->min.y <= miny;

}
bool KdTree::IsIntersect(TreeNode* node, Particle* p, double radius) {

	double dist = 0;

	if (node->min.x > p->Pos.x)
		dist += (node->min.x - p->Pos.x) * (node->min.x - p->Pos.x);
	
	else if(node->max.x < p->Pos.x)
		dist += (node->max.x - p->Pos.x) * (node->max.x - p->Pos.x);
	
	if (node->min.y > p->Pos.y)
		dist += (node->min.y - p->Pos.y) * (node->min.y - p->Pos.y);

	else if (node->max.y < p->Pos.y)
		dist += (node->max.y - p->Pos.y) * (node->max.y - p->Pos.y);

	return dist <= radius * radius;

}

void KdTree::RenderTree() {
		
	for (int i = 0; i < renderTree.size(); i++) {
		TreeNode* node = renderTree[i];
		glBegin(GL_LINES);

		glVertex2d(node->max.x, node->max.y);
		glVertex2d(node->min.x, node->max.y);

		glVertex2d(node->min.x, node->max.y);
		glVertex2d(node->min.x, node->min.y);

		glVertex2d(node->min.x, node->min.y);
		glVertex2d(node->max.x, node->min.y);

		glVertex2d(node->max.x, node->min.y);
		glVertex2d(node->max.x, node->max.y);

		glEnd();
	}

}


TreeNode::TreeNode(vector<Particle*>& _particles, Vector2D _min, Vector2D _max, int _depth) : particles(_particles), min(_min), max(_max), depth(_depth + 1) {}
TreeNode::TreeNode(Vector2D _min, Vector2D _max, int _depth) : min(_min), max(_max), depth(_depth + 1) {}

void TreeNode::subdivide() {

	Vector2D crossVec = max - min;								// max와 min을 가로지르는 벡터를 구한다.
	int pivotAxis = (crossVec.x > crossVec.y) ? 0 : 1;			// 만약 x 성분이 더 클 경우 x축 기준, y 축이 더 클 경우 y 축 기준으로 나눈다.
	int mid = particles.size() / 2;

	nth_element(particles.begin(), particles.begin() + mid, particles.end(),
		[pivotAxis](Particle* a, Particle* b) {
			if (pivotAxis)										// 1 일 경우 y축 기준으로 정렬
				return a->Pos.y < b->Pos.y;
			else												// 0 일 경우 x축 기준으로 정렬
				return a->Pos.x < b->Pos.x;
	});

	Vector2D localMin;
	Vector2D localMax;

	if (pivotAxis) {											// y축 기준으로 정렬되어 있을 경우
		localMin = Vector2D(min.x, particles[mid]->Pos.y);
		localMax = Vector2D(max.x, particles[mid]->Pos.y);
	}
	else {														// x축 기준으로 정렬되어 있을 경우
		localMin = Vector2D(particles[mid]->Pos.x, min.y);
		localMax = Vector2D(particles[mid]->Pos.x, max.y);
	}

	Vector2D square1Min = min;
	Vector2D square1Max = localMax;

	nodes[0] = new TreeNode(square1Min, square1Max, depth + 1);
	nodes[0]->particles.assign(particles.begin(), particles.begin() + mid);

	/*for (int i = 0; i < particles.size(); i++) {
		Particle* p = particles[i];
		if (square1Min.x <= p->Pos.x && p->Pos.x <= square1Max.x &&
			square1Min.y <= p->Pos.y && p->Pos.y <= square1Max.y)
			nodes[0]->particles.push_back(p);
	}
	*/
	Vector2D square2Min = localMin;
	Vector2D square2Max = max;

	nodes[1] = new TreeNode(square2Min, square2Max, depth + 1);
	nodes[1]->particles.assign(particles.begin() + mid, particles.end());
	/*
	for (int i = 0; i < particles.size(); i++) {
		Particle* p = particles[i];
		if (square2Min.x <= p->Pos.x && p->Pos.x <= square2Max.x &&
			square2Min.y <= p->Pos.y && p->Pos.y <= square2Max.y)
			nodes[1]->particles.push_back(p);
	}*/

}