#include <stdio.h>
#include "HashTable.h"
#include "glut.h"

HashTable::HashTable(vector<Particle> & _Particles, int _resolution) : Particles(_Particles), resolution(_resolution)
{
	// table initialize
	table = new vector<vector<Particle*> *>[resolution];
	
	for (int i = 0; i < resolution; i++){
		for(int j = 0; j < resolution; j++)
			table[i].push_back(new vector<Particle*>());
	}
	for (int i = 0; i < Particles.size(); i++) {
		int grid_x, grid_y;
		
		grid_x = (int)fmaxf(0, fminf(resolution - 1, resolution * (Particles[i].x + 100) / 200));
		grid_y = (int)fmaxf(0, fminf(resolution - 1, resolution * (Particles[i].y + 100) / 200));
		// 좌표계를 0 ~ 1 사이로 노말라이즈 한 뒤, resolution을 곱해 해당 포인트가 어떤 셀에 포함되어있는지 알아낸다
		// 노말라이즈 식
		// x + abs(max x coordinate value) / window width
		// y + abs(max y coordinate value) / window height

		table[grid_x][grid_y]->push_back(&Particles[i]);
		Particles[i].SetRange(resolution * grid_y + grid_x);
	}
		
	
}

void HashTable::Query(Particle particle, int w, int h, double radius) {

	int grid_x = (int)fmaxf(0, fminf(resolution - 1, resolution * (particle.x + 100) / 200));
	int grid_y = (int)fmaxf(0, fminf(resolution - 1, resolution * (particle.y + 100) / 200));

	int neighbour_count = 0;

	printf("Grid x, y : %d %d\n", grid_x, grid_y);
	printf("position x, y : %f, %f\n", particle.x, particle.y);

	for (int x = grid_x - w; x <= grid_x + w; x++) {
		for (int y = grid_y - h; y <= grid_y + h; y++) {

			if (x < 0 || y < 0 || x >= resolution || y >= resolution)
				continue;
			
			for (int i = 0; i < table[x][y]->size(); i++) {

				Particle* p = (*table[x][y])[i];
				p->isActive = true;

				double sdf = (particle.x - p->x) * (particle.x - p->x) + (particle.y - p->y) * (particle.y - p->y) - radius * radius;

				if (sdf <= 0.0){
					p->isInRange = true;
					printf("neighbour detected! : %lf %lf %lf\n", p->x, p->y, sdf);
					neighbour_count++;
				}
			}
		}


	}

	printf("total neighbour count : %d", neighbour_count);

}