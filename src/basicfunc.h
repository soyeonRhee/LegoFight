#pragma once
#include "ObjParser.h"

void init(void);

void resize(int width, int height);

void mouse(int button,int state, int x, int y);

void motionmouse(int x, int y);

void mouseWheel(int button, int dir, int x, int y);

void  keyboard(unsigned char key, int x, int y);

void  specialkb(int key, int x, int y);

void draw_axis(void);

void draw(void);

void createGLUTMenus();