#pragma once
#include <vector>
#include "Particle.h"

using namespace std;

class TreeNode;

class QuadTree {
public:
	int depthLimit;
	int particleLimit;
	TreeNode* root;

	vector<TreeNode*> renderTree;

	QuadTree(int _depthLimit, int _particleLimit) 
		: depthLimit(_depthLimit), particleLimit(_particleLimit) {}

	void buildTree(vector<Particle *> & particles, int w, int h);
	void query2(Particle& particle, double radius);
	void query(Particle& particle, double radius);
	void RenderTree();
	bool IsContainRange(TreeNode * node, Vector2D pos, double radius);
	bool IsIntersect(TreeNode* node, Vector2D pos, double radius);

};

class TreeNode {
public:
	std::vector<Particle *> particles;
	TreeNode* nodes[4];
	Vector2D min;
	Vector2D max;
	int depth;
	bool isInRange;

	TreeNode(Vector2D _min, Vector2D _max, int _depth);
	TreeNode(vector<Particle*>& _particles, Vector2D _min, Vector2D _max, int _depth);
	void subdivide();
	bool HasChildNode();
};