#include <GL/glut.h>
#include<stdio.h>
#include <GL/freeglut.h>
#include <fstream>
#include "basicfunc.h"
#include "newmode.h"
#include "openmode.h"
#include "drawlego.h"
#include "fightmode.h"
using namespace std;

int wheelstate = 0;
int xinit, yinit;
double xangle=335, yangle=60;//leftbutton 조정
int r = 150;//휠

int leftbuttonstate = 0;
int _width=500, _height=500;

int bodyNum[2] = {1,1};//arm size

//menu state 1- new 2-openedit 3-openview 4-fight
int menustate = 0;

bool callnewinit = false;
int menu1, menu2, menu3,menu4, submenu1;
int characternum=0;

void menu_draw_string();

void init(void) {
	initObject();
	glutSetMenu(menu1);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glClearColor(1.0, 1.0, 1.0, 1.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
}
//callnewinit인자를 사용해 mode가 바뀔 때 최초 1번 해당 mode의 init함수가 호출되도록 함
void callinit() {
	//newmode
	if (menustate == 1) {
		newinit();
	}
	//openeditmode
	else if (menustate == 2) {
		openinit();
	}
	//choose mode
	else if (menustate == 0) {
		glClearColor(1.0, 1.0, 1.0, 1.0f);
		glClearDepth(1.0f);
	}
	else if (menustate == 4) {
		fightinit();
	}
	callnewinit = false;
	glutPostRedisplay();
}
//leftbuttonstate&x,yinit -> 왼쪽클릭으로 시점이동
void mouse(int button, int state, int x, int y) {
	if (menustate != 4) {
		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
			leftbuttonstate = 1;//왼쪽 마우스가 눌린 상태 표시
			xinit = x;
			yinit = y;
		}
		else if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP) {
			leftbuttonstate = 0;//왼쪽 마우스가 눌리지 않은 상태 표시
		}
	}
}
//leftbuttonstate&xangle&yangle -> 왼쪽클릭으로 시점 이동
void motionmouse(int x, int y) {
	if (menustate != 4) {
		if (leftbuttonstate == 1) {
			xangle += 0.25*((double)(x - xinit));
			yangle += 0.25*((double)(yinit - y));
		}

		if (xangle > 360) xangle -= 360;
		else if (xangle < 0) xangle += 360;

		if (yangle > 360) yangle -= 360;
		else if (yangle < 0) yangle += 360;

		xinit = x;
		yinit = y;

		glutPostRedisplay();
	}
}
//마우스 휠을 사용해 확대 축소 구현
void mouseWheel(int button, int dir, int x, int y) {
	if (menustate == 4) {
		if (dir > 0) {
			if (r > 50) r -= 5;
		}
		else {
			if (r <= 260)r += 5;
		}
	}
	else {
		if (dir > 0) {
			if (r > 50) r -= 5;
		}
		else {
			if (r < 500)r += 5;
		}
	}
	glutPostRedisplay();
}

void  keyboard(unsigned char key, int x, int y) {
	if (menustate == 1) {
		newkeyboard(key,x,y);
	}
	if (menustate == 4) {
		fightkeyboard(key, x, y);
	}
}

void  specialkb(int key, int x, int y) {
	if (menustate == 1) {
		newspecialkb(key,x,y);
	}
	if (menustate == 4) {
		fightspecialkb(key, x, y);
	}
}

void resize(int width, int height) {
	_width = width;
	_height = height;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(100, (float)width / (float)height, 1, 500);
	glMatrixMode(GL_MODELVIEW);
}

void draw(void) {
	if (callnewinit) callinit();
	if (menustate == 0) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(10, 10, 10, 0, 0, 0, 0, 1, 0);
		menu_draw_string();
		glFlush();
		glutSwapBuffers();
	}
	if (menustate == 1) {
		newdraw();
	}
	else if (menustate == 2) {
		opendraw();
	}
	else if (menustate == 4) {
		fightdraw();
	}
}

//menu at choose menu
void processMenu1(int option) {
	menustate = option;
	callnewinit = true;//for call new init
	if (option == 1) {
		glutSetMenu(menu2);
	}
	if (option == 4) {
		glutSetMenu(menu4);
	}
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glutPostRedisplay();
}

//menu at new mode
//select save number
void processMenu2(int option) {
	//quit to choose menu
	if (option == 99) {
		menustate = 0;
		callnewinit = true;//for call new init
		glutSetMenu(menu1);
		glutAttachMenu(GLUT_RIGHT_BUTTON);
		glutPostRedisplay();
	}
	else save(option);
}
//menu at open mode
//view.edit,quit
void processMenu3(int option) {
	if (option == 99) {
		menustate = 0;
		callnewinit = true;//for call new init
		glutSetMenu(menu1);
		glutAttachMenu(GLUT_RIGHT_BUTTON);
		glutPostRedisplay();
	}
}

void processMenu4(int option) {
	fightopen(option, characternum);
	characternum++;
	if (characternum == 2) {
		characternum = 0;//원상복귀
		//단순 quit메뉴
		glutSetMenu(menu3);
		glutAttachMenu(GLUT_RIGHT_BUTTON);
		glutPostRedisplay();
	}
}

void processSubmenu1(int option) {
	menustate = 2;
	//선택한 파일오픈
	open((option-10));
	callnewinit = true;//for call new init
	//메뉴전환
	glutSetMenu(menu3);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glutPostRedisplay();
}

void createGLUTMenus() {

	submenu1 = glutCreateMenu(processSubmenu1);
	glutAddMenuEntry("(1)",11);
	glutAddMenuEntry("(2)", 12);
	glutAddMenuEntry("(3)", 13);
	glutAddMenuEntry("(4)", 14);
	glutAddMenuEntry("(5)", 15);

	menu1 = glutCreateMenu(processMenu1);
	glutAddMenuEntry("New", 1);
	glutAddSubMenu("Open", submenu1);
	glutAddMenuEntry("Fight", 4);
	//connect menu to rightbutton
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	menu2 = glutCreateMenu(processMenu2);
	glutAddMenuEntry("save to (1)", 1);
	glutAddMenuEntry("save to (2)", 2);
	glutAddMenuEntry("save to (3)", 3);
	glutAddMenuEntry("save to (4)", 4);
	glutAddMenuEntry("save to (5)", 5);
	glutAddMenuEntry("quit",99);


	menu3= glutCreateMenu(processMenu3);
	glutAddMenuEntry("quit", 99);

	menu4 = glutCreateMenu(processMenu4);
	glutAddMenuEntry("(1)", 1);
	glutAddMenuEntry("(2)", 2);
	glutAddMenuEntry("(3)", 3);
	glutAddMenuEntry("(4)", 4);
	glutAddMenuEntry("(5)", 5);
}

void menu_draw_string() {
	const char* str = "< MAIN MENU >";
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
	glRasterPos3f(-9, 9.0, 0);
	for (unsigned int i = 0; i < strlen(str); i++) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str[i]);
	}
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopAttrib();
}