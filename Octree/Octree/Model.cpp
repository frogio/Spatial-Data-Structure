#include "Model.h"

Model::Model(const char* obj_path) {

	FILE* fp = fopen(obj_path, "r");
	char buffer[50];

	if (fp == NULL) {
		printf("error occured while open object file...\n");
		system("pause");
		exit(0);
	}
		
	max = Vector(-INF, -INF, -INF);
	min = Vector(INF, INF, INF);

	while (fgets(buffer, sizeof(buffer), fp) != NULL) {
		if (buffer[0] == 'v') {
			
			strtok(buffer, " ");

			double x = atof(strtok(NULL, " "));
			double y = atof(strtok(NULL, " "));
			double z = atof(strtok(NULL, " "));

			vertices.push_back(new Vector(x, y, z));
			
			max.x = fmaxf(x, max.x);
			max.y = fmaxf(y, max.y);
			max.z = fmaxf(z, max.z);

			min.x = fminf(x, min.x);
			min.y = fminf(y, min.y);
			min.z = fminf(z, min.z);

		}
		else if (buffer[0] == 'f') {
			strtok(buffer, " ");

			double idx1 = atoi(strtok(NULL, " "));
			double idx2 = atoi(strtok(NULL, " "));
			double idx3 = atoi(strtok(NULL, " "));

			faces.push_back(Face(vertices[idx1 - 1], vertices[idx2 - 1], vertices[idx3 - 1]));
		
		}

	}

	printf("vertex count : %d\n", vertices.size());
	printf("max vertex : %lf %lf %lf\n", max.x, max.y, max.z);
	printf("min vertex : %lf %lf %lf\n", min.x, min.y, min.z);

	fclose(fp);

}
void Model::DrawPoint() {
	glBegin(GL_POINTS);

	for (int i = 0; i < vertices.size(); i++) 
		glVertex3f(vertices[i]->x, vertices[i]->y, vertices[i]->z);

	glEnd();

}
void Model::DrawModel() {

	for (int i = 0; i < faces.size(); i++) {

		glBegin(GL_POLYGON);

		Face & face = faces[i];

		Vector v1_2 = *face.vertex[1] - *face.vertex[0];
		Vector v1_3 = *face.vertex[2] - *face.vertex[0];

		Vector normal = v1_2.Cross(v1_3).GetNormalizedVector();

		glNormal3d(normal.x, normal.y, normal.z);

		for (int i = 0; i < 3; i++)
			glVertex3f(face.vertex[i]->x, face.vertex[i]->y, face.vertex[i]->z);

		glEnd();

	}
	/*
	glBegin(GL_LINES);

	glDisable(GL_LIGHT0);

	glColor3f(0.f, 0.f, 1.f);

	glVertex3d(max.x, max.y, max.z);
	glVertex3d(min.x, min.y, min.z);

	glEnable(GL_LIGHT0);
	glEnd();*/
}