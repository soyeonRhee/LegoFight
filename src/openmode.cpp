#include <GL/glut.h>
#include <GL/freeglut.h>
#include <fstream>
#include "openmode.h"
#include "drawlego.h"
using namespace std;

extern int _width, _height;
extern double xangle, yangle;
extern int r;
extern int bodyNum[2];

extern int kick;//to kick animation

modeling mopen[20];
int moCount = 0;

void draw_marray();

void openinit(void) {
	glClearColor(1.0, 1.0, 1.0, 1.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);

	GLfloat ambientMaterial[] = { 1.0,1.0,1.0,1.0 };
	GLfloat diffuseMaterial[] = { 0.7,0.7,0.7,1.0 };

	xangle = 335, yangle = 60;
	r = 150;

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientMaterial);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseMaterial);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuseMaterial);

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);

	glColorMaterial(GL_FRONT, GL_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	glDisable(GL_TEXTURE_2D);
};

void opendraw(void) {
	GLfloat light_position0[] = { 0.0,0.0,0,1.0 };
	GLfloat light_position1[] = { 0.0,500.0,0,0.0 };
	glClearColor(1, 1, (float)204/255, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glLightfv(GL_LIGHT1, GL_POSITION, light_position0);
	//upvector
	float upvector = 1;
	if ((yangle > 180 && yangle <= 360)) upvector = -1;
	double ry = (yangle / 180)*3.141592;
	double rx = (xangle / 180)*3.141592;
	gluLookAt((r)*sin(ry)*cos(rx), r*cos(ry)
		, r*sin(ry)*sin(rx), 0, 0, 0, 0, upvector, 0);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position1);

	draw_axis();
	draw_marray();

	glFlush();
	glutSwapBuffers();
};

int open(int num) {
	string line;
	int temp;
	int i = 0;
	string filename("save/(");
	filename = filename + to_string(num) + ").txt";
	ifstream in(filename);

	if (!in.is_open()) {
		cout << "error opening file" << endl;
		return 1;
	}
	
	while (1) {
		if (!getline(in, line)) break;
		line=line.substr(0, 2);
		moCount=atoi(line.substr(1).c_str());//몇번째 블럭인지
		
		getline(in, line);
		mopen[moCount].partNum = atoi(line.c_str());
		getline(in, line);
		mopen[moCount].blkNum = atoi(line.c_str());
		if (mopen[moCount].partNum == 1) {
			bodyNum[0] = mopen[moCount].blkNum;
		}
		getline(in, line);
		mopen[moCount].color = atoi(line.c_str());
		getline(in, line);
		mopen[moCount].tx= atoi(line.c_str());
		getline(in, line);
		mopen[moCount].ty = atoi(line.c_str());
		getline(in, line);
		mopen[moCount].tz = atoi(line.c_str());
		getline(in, line);
		mopen[moCount].r = atoi(line.c_str());
	}
	moCount++;

	glutPostRedisplay();
	return 0;
}

void draw_marray() {
	for (int i = 0; i < moCount; i++) {
		glPushMatrix();
		glTranslatef(mopen[i].tx, mopen[i].ty, mopen[i].tz);
		glRotatef(mopen[i].r, 0, 1, 0);
		draw_lego(mopen[i].partNum, mopen[i].blkNum, mopen[i].color);
		glPopMatrix();
	}
}