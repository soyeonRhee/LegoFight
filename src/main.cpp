#include <GL/glut.h>
#include<stdio.h>
#include "basicfunc.h"

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(300, 300);
	glutCreateWindow("SOYEON LEE_PROJECT");
	init();
	createGLUTMenus();

	glutReshapeFunc(resize);
	glutDisplayFunc(draw);
	glutMouseFunc(mouse);
	glutMotionFunc(motionmouse);
	glutMouseWheelFunc(mouseWheel);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialkb);
	//glutIdleFunc(idle);

	glutMainLoop();

	return 0;
}