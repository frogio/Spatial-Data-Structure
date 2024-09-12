#pragma once
#include <vector>
#include <algorithm>
#include <queue>
#include "Particle.h"
#include "glut.h"

using namespace std;

class TreeNode;

class KdTree {
public:
	vector<TreeNode *> renderTree;
	TreeNode * root;
	int particleLimit;

	KdTree(int _particleLimit) : particleLimit(_particleLimit) {}
	void buildTree(vector<Particle*>& _particles, double w, double h);
	void query(Particle* p, double radius);
	void query2(Particle* p, double radius);
	bool IsContainRange(TreeNode* node, Particle* p, double radius);
	bool IsIntersect(TreeNode* node, Particle* p, double radius);
	bool HasChild(TreeNode * node);
	void RenderTree();
};

class TreeNode {
public:
	vector<Particle *> particles;
	TreeNode* nodes[2];
	Vector2D min;
	Vector2D max;
	int depth;

	TreeNode(vector<Particle*>& _particles, Vector2D _min, Vector2D _max, int _depth);
	TreeNode(Vector2D _min, Vector2D _max, int _depth);
	void subdivide();

};