#pragma once
#include <math.h>
#include <vector>

#include "Particle.h"


using namespace std;

class HashTable {
public:
	vector<Particle> & Particles;
	vector<vector<Particle *> *> * table;
	int resolution;

	HashTable(vector<Particle>& _Particles, int _resolution);
	void Query(Particle particle, int w, int h, double radius);


};