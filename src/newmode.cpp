#include <GL/glut.h>
//#include<stdio.h>
#include <GL/freeglut.h>
#include <fstream>
#include "newmode.h"
#include "drawlego.h"
using namespace std;

//basicfunc으로부터 변수 extern
extern int _width, _height;
extern double xangle, yangle;
extern int r ;
extern int bodyNum[2];//arm size

GLfloat light_position1[] = { 0.0,500.0,0,0.0 };
int state = 1;
int tempP = 0;
int tempB = 1;
int tempC = 0;
float tempx = 0.0, tempy = 0.0, tempz = 0.0, tempR = 0.0;

char guide0[40];
char guide1[40];
char guide2_1[50];
char guide2_2[100];
char guide2_3[50];
char guide3[60];
const char* kbguide[5] = { guide0,guide1,guide2_1,guide3,guide2_2 };
const char* kbstr = "<KEYBOARD GUIDE> : PRESS SPACE BAR TO NEXT";
const char* guide[4] = { "SELECT MENU!","SELECT PART!","SELECT BLK AND COLOR!","MOVE AND ROTATE!" };
const char* colorguide = " USE <- -> TO SELECT COLOR";
int previewtf = 0;

struct modeling m[20];
int mCount;

extern aabb* abpartset[4];//충돌처리
extern float bodysize[3];
extern float armz[6];

void draw_axis(void);
int save(int num);
void drawpreview();
void draw_string(int state);
void drawNew();
bool aabbcollision(int tempP, int tempB, int rotate);

void newinit(void) {
	//strcpy(guide0, "1.NEW 2.OPEN");
	strcpy(guide1, "1.HEAD 2.BODY 3.ARM 4.LEG");
	strcpy(guide2_1, "0.BACK 1.THIN 2.REGULAR 3.THICK");
	strcpy(guide2_2, " 0.BACK (SHORT)1.THIN 2.REGULAR 3.THICK (LONG)4.THIN 5.REGULAR 6.THICK");
	strcpy(guide3, " Use arrow keys to move block | (u)UP (d)DOWN (r)ROTATE");
	
	bodyNum[0] = 1;
	mCount = 0;

	xangle = 335, yangle = 60;
	r = 150;

	glClearColor(0.9411, 0.9725, 1.0, 1.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);

	GLfloat ambientMaterial[] = { 1.0,1.0,1.0,1.0 };
	GLfloat diffuseMaterial[] = { 0.7,0.7,0.7,1.0 };

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientMaterial);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseMaterial);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuseMaterial);

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	//glShadeModel(GL_SMOOTH);
	//antialiasing
	//glEnable(GL_POLYGON_SMOOTH);
	//glEnable(GL_LINE_SMOOTH);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glColorMaterial(GL_FRONT, GL_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	glDisable(GL_TEXTURE_2D);
}

void  newkeyboard(unsigned char key, int x, int y) {
	switch (key)
	{
	case '0':
		if (state == 2) {
			state = 1;
			tempB = 0; tempC = 0;
		}
		else if (state == 3) {
			state = 2;
			tempB = 0; tempC = 0;
			tempx = 0.0, tempy = 0.0, tempz = 0.0, tempR = 0.0;
		}
		break;
	case '1':
		if (state == 1) {
			tempP = 0;
			state = 2;
		}
		else if (state == 2) {
			tempB = 0;
		}
		break;
	case '2':
		if (state == 1) {
			tempP = 1;
			state = 2;
		}
		else if (state == 2) {
			tempB = 1;
		}
		break;
	case '3':
		if (state == 1) {
			tempP = 2;
			state = 2;
		}
		else if (state == 2) {
			tempB = 2;
		}
		break;
	case '4':
		if (state == 1) {
			tempP = 3;
			state = 2;
		}
		else if (state == 2) {
			tempB = 3;
		}
		break;
	case '5':
		if (state == 2) {
			tempB = 4;
		}
		break;
	case '6':
		if (state == 2) {
			tempB = 5;
		}
		break;
	case 'u'://up
		if (state == 3)tempy += 2.5;
		break;
	case 'd'://down
		if (state == 3) {
			if (tempy >= 5)tempy -= 2.5;
		}
		break;
	case 'r'://rotate
		if (state == 3) {
			if (tempR == 270) tempR = 0;
			else tempR += 90;
		}
		break;
	case 'z'://undo
		if (mCount > 0) {
			mCount--;
		}
		break;
	case ' '://space bar
		if (state == 1) {
			state = 2;
		}
		else if (state == 2) {
			if (tempP == 1) bodyNum[0] = tempB;
			state = 3;
		}
		else if (state == 3) {
			//수정
			if (aabbcollision(tempP,tempB, tempR)) {
				m[mCount] = { tempP,tempB,tempC,tempx,tempy,tempz,tempR };
				mCount = mCount + 1;
				tempP = 0; tempB = 0; tempC = 0;
				tempx = 0.0, tempy = 0.0, tempz = 0.0, tempR = 0.0;
				state = 1;
			}
		}
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

void  newspecialkb(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_UP:
		if (state == 3) {
			tempz += 2;
		}
		break;
	case GLUT_KEY_DOWN:
		if (state == 3) {
			tempz -= 2;
		}
		break;
	case GLUT_KEY_LEFT:
		if (state == 2) {
			if (tempC == 0) tempC = 9;
			else tempC--;
		}
		else if (state == 3) {
			tempx += 2;
		}
		break;
	case GLUT_KEY_RIGHT:
		if (state == 2) {
			if (tempC == 9) tempC = 0;
			else tempC++;
		}
		else if (state == 3) {
			tempx -= 2;
		}
		break;
	}
	glutPostRedisplay();
}

void newdraw(void) {
	GLfloat light_position0[] = { 0.0,0.0,0,1.0 };
	GLfloat light_position1[] = { 0.0,500.0,0,0.0 };
	glClearColor((float)204/255, 1, (float)204/255, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glLightfv(GL_LIGHT1, GL_POSITION, light_position0);
	if ((state == 1) || (state == 2)) drawpreview();
	//upvector
	float upvector = 1;
	if ((yangle > 180 && yangle <= 360)) upvector = -1;
	double ry = (yangle / 180)*3.141592;
	double rx = (xangle / 180)*3.141592;
	gluLookAt((r)*sin(ry)*cos(rx), r*cos(ry)
		, r*sin(ry)*sin(rx), 0, 0, 0, 0, upvector, 0);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position1);

	draw_axis();
	draw_string(state);
	drawNew();
}

int save(int num) {
	string filename("save/(");
	filename = filename + to_string(num) + ").txt";
	ofstream out(filename);
	if (!out.is_open()) {
		cout << "error opening file" << endl;
		return 1;
	}
	for (int i=0; i < mCount;i++) {
		out <<"["<<i<<"]"<<endl<< m[i].partNum << endl << m[i].blkNum << endl << m[i].color << endl
			<< m[i].tx << endl << m[i].ty << endl << m[i].tz << endl << m[i].r<<endl;
	}
	out.close();
	return 0;
}


void drawpreview() {
	glViewport(_width * 1 / 2, _height * 1 / 2, _width / 2, _height / 2);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glScissor(_width * 2 / 3, _height * 2 / 3, _width / 3, _height / 3);
	glEnable(GL_SCISSOR_TEST);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_SCISSOR_TEST);

	//part 종류마다 뷰 다르게
	if (tempP == 0 || tempP == 2)gluLookAt(100, 20, -20, 0, 0, 0, 0, 1, 0);
	else if (tempP == 1)gluLookAt(100, 40, -20, 0, 0, 0, 0, 1, 0); //body
	else gluLookAt(100, 80, -20, 0, 0, 0, 0, 1, 0);

	glLightfv(GL_LIGHT0, GL_POSITION, light_position1);
	draw_axis();
	draw_lego(tempP, tempB, tempC);
	glFlush();
	glViewport(0, 0, _width, _height);
	glLoadIdentity();
}

void draw_string(int state) {
	const char* gstr = guide[state];
	const char* kbg;
	if ((state == 2) && (tempP == 2 || tempP == 3)) kbg = kbguide[4];
	else kbg = kbguide[state];

	glPushAttrib(GL_LIGHTING_BIT);
	glDisable(GL_LIGHTING);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(-10, 10, -10, 10);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glColor3f(0, 0, 0);
	glRasterPos3f(-9, -7.0, 0);
	for (unsigned int i = 0; i < strlen(gstr); i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, gstr[i]);
	}
	glRasterPos3f(-9, -8.0, 0);
	for (unsigned int i = 0; i < strlen(kbstr); i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, kbstr[i]);
	}
	glRasterPos3f(-9, -8.5, 0);
	for (unsigned int i = 0; i < strlen(kbg); i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, kbg[i]);
	}
	if (state == 2) {
		glRasterPos3f(-9, -9.0, 0);
		for (unsigned int i = 0; i < strlen(colorguide); i++) {
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, colorguide[i]);
		}
	}
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopAttrib();
}

void drawNew() {
	for (int i = 0; i < mCount; i++) {
		glPushMatrix();
		glTranslatef(m[i].tx, m[i].ty, m[i].tz);
		glRotatef(m[i].r, 0, 1, 0);
		draw_lego(m[i].partNum, m[i].blkNum, m[i].color);
		glPopMatrix();
	}
	if (state == 3) {
		previewtf = 1;
		glPushMatrix();
		glTranslatef(tempx, tempy, tempz);
		glRotatef(tempR, 0, 1, 0);
		draw_lego(tempP, tempB, tempC);
		glPopMatrix();
		previewtf = 0;
	}
	glFlush();
	glutSwapBuffers();
}

//충돌하지않으면 true반환
bool aabbcollision(int tempP, int tempB, int rotate) {
	aabb curblk = abpartset[tempP][tempB];
	if (tempP == 2) {
		return true;
	}
	if (mCount == 0) return true;
	int crash = 0;
	int curz = tempz;
	int curx = tempx;

		if (rotate == 0) {
			for (int i = 0; i < mCount; i++) {
				if (m[i].r == 0) {
					//x축
					if ((curx + curblk.minx) >(m[i].tx + abpartset[m[i].partNum][m[i].blkNum].maxx) ||
						(curx + curblk.maxx) < (m[i].tx + abpartset[m[i].partNum][m[i].blkNum].minx)) {
						continue;
					}
					//z축
					if ((curz + curblk.minz) > (m[i].tz + abpartset[m[i].partNum][m[i].blkNum].maxz) ||
						(curz + curblk.maxz) < (m[i].tz + abpartset[m[i].partNum][m[i].blkNum].minz)) {
						continue;
					}
					//y축
					if ((tempy + curblk.miny) > (m[i].ty + abpartset[m[i].partNum][m[i].blkNum].maxy) ||
						(tempy + curblk.maxy) < (m[i].ty + abpartset[m[i].partNum][m[i].blkNum].miny)) {
						continue;
					}
				}
				else if (m[i].r == 90) {
					//x축
					if ((curx + curblk.minx) > (m[i].tx + abpartset[m[i].partNum][m[i].blkNum].maxz) ||
						(curx + curblk.maxx) < (m[i].tx + abpartset[m[i].partNum][m[i].blkNum].minz)) {
						continue;
					}
					//z축
					if ((curz + curblk.minz) > (m[i].tz - abpartset[m[i].partNum][m[i].blkNum].minx) ||
						(curz + curblk.maxz) < (m[i].tz - abpartset[m[i].partNum][m[i].blkNum].maxx)) {
						continue;
					}
					//y축
					if ((tempy + curblk.miny) > (m[i].ty + abpartset[m[i].partNum][m[i].blkNum].maxy) ||
						(tempy + curblk.maxy) < (m[i].ty + abpartset[m[i].partNum][m[i].blkNum].miny)) {
						continue;
					}
				}
				else if (m[i].r == 180) {
					//x축
					if ((curx + curblk.minx) > (m[i].tx - abpartset[m[i].partNum][m[i].blkNum].minx) ||
						(curx + curblk.maxx) < (m[i].tx - abpartset[m[i].partNum][m[i].blkNum].maxx)) {
						continue;
					}
					//z축
					if ((curz + curblk.minz) > (m[i].tz - abpartset[m[i].partNum][m[i].blkNum].minz) ||
						(curz + curblk.maxz) < (m[i].tz - abpartset[m[i].partNum][m[i].blkNum].maxz)) {
						continue;
					}
					//y축
					if ((tempy + curblk.miny) > (m[i].ty + abpartset[m[i].partNum][m[i].blkNum].maxy) ||
						(tempy + curblk.maxy) < (m[i].ty + abpartset[m[i].partNum][m[i].blkNum].miny)) {
						continue;
					}
				}
				else if (m[i].r == 270) {
					//x축
					if ((curx + curblk.minx) > (m[i].tx - abpartset[m[i].partNum][m[i].blkNum].minz) ||
						(curx + curblk.maxx) < (m[i].tx - abpartset[m[i].partNum][m[i].blkNum].maxz)) {
						continue;
					}
					//z축
					if ((curz + curblk.minz) > (m[i].tz + abpartset[m[i].partNum][m[i].blkNum].maxx) ||
						(curz + curblk.maxz) < (m[i].tz + abpartset[m[i].partNum][m[i].blkNum].minx)) {
						continue;
					}
					//y축
					if ((tempy + curblk.miny) > (m[i].ty + abpartset[m[i].partNum][m[i].blkNum].maxy) ||
						(tempy + curblk.maxy) < (m[i].ty + abpartset[m[i].partNum][m[i].blkNum].miny)) {
						continue;
					}
				}
				crash = 1;
			}
		}
		else if (rotate == 90) {
			for (int i = 0; i < mCount; i++) {
				if (m[i].r == 0) {
					//x축
					if ((curx + curblk.minz) >(m[i].tx + abpartset[m[i].partNum][m[i].blkNum].maxx) ||
						(curx + curblk.maxz) < (m[i].tx + abpartset[m[i].partNum][m[i].blkNum].minx)) {
						continue;
					}
					//z축
					if ((curz - curblk.maxx) > (m[i].tz + abpartset[m[i].partNum][m[i].blkNum].maxz) ||
						(curz - curblk.minx) < (m[i].tz + abpartset[m[i].partNum][m[i].blkNum].minz)) {
						continue;
					}
					//y축
					if ((tempy + curblk.miny) > (m[i].ty + abpartset[m[i].partNum][m[i].blkNum].maxy) ||
						(tempy + curblk.maxy) < (m[i].ty + abpartset[m[i].partNum][m[i].blkNum].miny)) {
						continue;
					}
				}
				else if (m[i].r == 90) {
					//x축
					if ((curx + curblk.minz) > (m[i].tx + abpartset[m[i].partNum][m[i].blkNum].maxz) ||
						(curx + curblk.maxz) < (m[i].tx + abpartset[m[i].partNum][m[i].blkNum].minz)) {
						continue;
					}
					//z축
					if ((curz - curblk.maxx) > (m[i].tz - abpartset[m[i].partNum][m[i].blkNum].minx) ||
						(curz - curblk.minx) < (m[i].tz - abpartset[m[i].partNum][m[i].blkNum].maxx)) {
						continue;
					}
					//y축
					if ((tempy + curblk.miny) > (m[i].ty + abpartset[m[i].partNum][m[i].blkNum].maxy) ||
						(tempy + curblk.maxy) < (m[i].ty + abpartset[m[i].partNum][m[i].blkNum].miny)) {
						continue;
					}
				}
				else if (m[i].r == 180) {
					//x축
					if ((curx + curblk.minz) > (m[i].tx - abpartset[m[i].partNum][m[i].blkNum].minx) ||
						(curx + curblk.maxz) < (m[i].tx - abpartset[m[i].partNum][m[i].blkNum].maxx)) {
						continue;
					}
					//z축
					if ((curz - curblk.maxx) > (m[i].tz - abpartset[m[i].partNum][m[i].blkNum].minz) ||
						(curz - curblk.minx) < (m[i].tz - abpartset[m[i].partNum][m[i].blkNum].maxz)) {
						continue;
					}
					//y축
					if ((tempy + curblk.miny) > (m[i].ty + abpartset[m[i].partNum][m[i].blkNum].maxy) ||
						(tempy + curblk.maxy) < (m[i].ty + abpartset[m[i].partNum][m[i].blkNum].miny)) {
						continue;
					}
				}
				else if (m[i].r == 270) {
					//x축
					if ((curx + curblk.minz) > (m[i].tx - abpartset[m[i].partNum][m[i].blkNum].minz) ||
						(curx + curblk.maxz) < (m[i].tx - abpartset[m[i].partNum][m[i].blkNum].maxz)) {
						continue;
					}
					//z축
					if ((curz - curblk.maxx) > (m[i].tz + abpartset[m[i].partNum][m[i].blkNum].maxx) ||
						(curz - curblk.minx) < (m[i].tz + abpartset[m[i].partNum][m[i].blkNum].minx)) {
						continue;
					}
					//y축
					if ((tempy + curblk.miny) > (m[i].ty + abpartset[m[i].partNum][m[i].blkNum].maxy) ||
						(tempy + curblk.maxy) < (m[i].ty + abpartset[m[i].partNum][m[i].blkNum].miny)) {
						continue;
					}
				}
				crash = 1;
			}
		}
		else if (rotate == 180) {
			for (int i = 0; i < mCount; i++) {
				if (m[i].r == 0) {
					//x축
					if ((curx - curblk.maxx) >(m[i].tx + abpartset[m[i].partNum][m[i].blkNum].maxx) ||
						(curx - curblk.minx) < (m[i].tx + abpartset[m[i].partNum][m[i].blkNum].minx)) {
						continue;
					}
					//z축
					if ((curz - curblk.maxz) > (m[i].tz + abpartset[m[i].partNum][m[i].blkNum].maxz) ||
						(curz - curblk.minz) < (m[i].tz + abpartset[m[i].partNum][m[i].blkNum].minz)) {
						continue;
					}
					//y축
					if ((tempy + curblk.miny) > (m[i].ty + abpartset[m[i].partNum][m[i].blkNum].maxy) ||
						(tempy + curblk.maxy) < (m[i].ty + abpartset[m[i].partNum][m[i].blkNum].miny)) {
						continue;
					}
				}
				else if (m[i].r == 90) {
					//x축
					if ((curx - curblk.maxx) > (m[i].tx - abpartset[m[i].partNum][m[i].blkNum].minx) ||
						(curx - curblk.minx) < (m[i].tx - abpartset[m[i].partNum][m[i].blkNum].maxx)) {
						continue;
					}
					//z축
					if ((curz - curblk.maxz) > (m[i].tz - abpartset[m[i].partNum][m[i].blkNum].minx) ||
						(curz - curblk.minz) < (m[i].tz - abpartset[m[i].partNum][m[i].blkNum].maxx)) {
						continue;
					}
					//y축
					if ((tempy + curblk.miny) > (m[i].ty + abpartset[m[i].partNum][m[i].blkNum].maxy) ||
						(tempy + curblk.maxy) < (m[i].ty + abpartset[m[i].partNum][m[i].blkNum].miny)) {
						continue;
					}
				}
				else if (m[i].r == 180) {
					//x축
					if ((curx - curblk.maxx) > (m[i].tx - abpartset[m[i].partNum][m[i].blkNum].minx) ||
						(curx - curblk.minx) < (m[i].tx - abpartset[m[i].partNum][m[i].blkNum].maxx)) {
						continue;
					}
					//z축
					if ((curz - curblk.maxz) > (m[i].tz - abpartset[m[i].partNum][m[i].blkNum].minz) ||
						(curz - curblk.minz) < (m[i].tz - abpartset[m[i].partNum][m[i].blkNum].maxz)) {
						continue;
					}
					//y축
					if ((tempy + curblk.miny) > (m[i].ty + abpartset[m[i].partNum][m[i].blkNum].maxy) ||
						(tempy + curblk.maxy) < (m[i].ty + abpartset[m[i].partNum][m[i].blkNum].miny)) {
						continue;
					}
				}
				else if (m[i].r == 270) {
					//x축
					if ((curx - curblk.maxx) > (m[i].tx - abpartset[m[i].partNum][m[i].blkNum].minz) ||
						(curx - curblk.minx) < (m[i].tx - abpartset[m[i].partNum][m[i].blkNum].maxz)) {
						continue;
					}
					//z축
					if ((curz - curblk.maxz) > (m[i].tz + abpartset[m[i].partNum][m[i].blkNum].maxx) ||
						(curz - curblk.minz) < (m[i].tz + abpartset[m[i].partNum][m[i].blkNum].minx)) {
						continue;
					}
					//y축
					if ((tempy + curblk.miny) > (m[i].ty + abpartset[m[i].partNum][m[i].blkNum].maxy) ||
						(tempy + curblk.maxy) < (m[i].ty + abpartset[m[i].partNum][m[i].blkNum].miny)) {
						continue;
					}
				}
				crash = 1;
			}
		}
		else if (rotate == 270) {
			for (int i = 0; i < mCount; i++) {
				if (m[i].r == 0) {
					//x축
					if ((curx - curblk.maxz) >(m[i].tx + abpartset[m[i].partNum][m[i].blkNum].maxx) ||
						(curx - curblk.minz) < (m[i].tx + abpartset[m[i].partNum][m[i].blkNum].minx)) {
						continue;
					}
					//z축
					if ((curz + curblk.minx) > (m[i].tz + abpartset[m[i].partNum][m[i].blkNum].maxz) ||
						(curz + curblk.maxx) < (m[i].tz + abpartset[m[i].partNum][m[i].blkNum].minz)) {
						continue;
					}
					//y축
					if ((tempy + curblk.miny) > (m[i].ty + abpartset[m[i].partNum][m[i].blkNum].maxy) ||
						(tempy + curblk.maxy) < (m[i].ty + abpartset[m[i].partNum][m[i].blkNum].miny)) {
						continue;
					}
				}
				else if (m[i].r == 90) {
					//x축
					if ((curx - curblk.maxz) > (m[i].tx - abpartset[m[i].partNum][m[i].blkNum].minx) ||
						(curx - curblk.minz) < (m[i].tx - abpartset[m[i].partNum][m[i].blkNum].maxx)) {
						continue;
					}
					//z축
					if ((curz + curblk.minx) > (m[i].tz - abpartset[m[i].partNum][m[i].blkNum].minx) ||
						(curz + curblk.maxx) < (m[i].tz - abpartset[m[i].partNum][m[i].blkNum].maxx)) {
						continue;
					}
					//y축
					if ((tempy + curblk.miny) > (m[i].ty + abpartset[m[i].partNum][m[i].blkNum].maxy) ||
						(tempy + curblk.maxy) < (m[i].ty + abpartset[m[i].partNum][m[i].blkNum].miny)) {
						continue;
					}
				}
				else if (m[i].r == 180) {
					//x축
					if ((curx - curblk.maxz) > (m[i].tx - abpartset[m[i].partNum][m[i].blkNum].minx) ||
						(curx - curblk.minz) < (m[i].tx - abpartset[m[i].partNum][m[i].blkNum].maxx)) {
						continue;
					}
					//z축
					if ((curz + curblk.minx) > (m[i].tz - abpartset[m[i].partNum][m[i].blkNum].minz) ||
						(curz + curblk.maxx) < (m[i].tz - abpartset[m[i].partNum][m[i].blkNum].maxz)) {
						continue;
					}
					//y축
					if ((tempy + curblk.miny) > (m[i].ty + abpartset[m[i].partNum][m[i].blkNum].maxy) ||
						(tempy + curblk.maxy) < (m[i].ty + abpartset[m[i].partNum][m[i].blkNum].miny)) {
						continue;
					}
				}
				else if (m[i].r == 270) {
					//x축
					if ((curx - curblk.maxz) > (m[i].tx - abpartset[m[i].partNum][m[i].blkNum].minz) ||
						(curx - curblk.minz) < (m[i].tx - abpartset[m[i].partNum][m[i].blkNum].maxz)) {
						continue;
					}
					//z축
					if ((curz + curblk.minx) > (m[i].tz + abpartset[m[i].partNum][m[i].blkNum].maxx) ||
						(curz + curblk.maxx) < (m[i].tz + abpartset[m[i].partNum][m[i].blkNum].minx)) {
						continue;
					}
					//y축
					if ((tempy + curblk.miny) > (m[i].ty + abpartset[m[i].partNum][m[i].blkNum].maxy) ||
						(tempy + curblk.maxy) < (m[i].ty + abpartset[m[i].partNum][m[i].blkNum].miny)) {
						continue;
					}
				}
				crash = 1;
			}
		}

		if (crash == 1) return false;
		else return true;
	}


