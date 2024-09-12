#pragma once
#include <vector>
#include <queue>
#include "Model.h"

using namespace std;

class TreeNode;

class OcTree {
public:
	TreeNode* root;
	vector<TreeNode*> renderTree;
	int depthLimit;
	int particleLimit;

	OcTree(vector<Vector *> & vertices, int _depthLimit, int _particleLimit, Vector max, Vector min);
	void buildTree();
	void RenderTree(bool onlyleaf);
	
};

class TreeNode {
public:
	vector<Vector*> vertices;
	TreeNode* nodes[8];
	Vector max;
	Vector min;
	int depth;

	TreeNode(vector<Vector*> & _vertices, Vector _max, Vector _min, int _depth);
	TreeNode(Vector _max, Vector _min, int _depth);
	void subdivide();
};