#pragma once
#include "ObjParser.h"

void initObject();

void draw_object(ObjParser *objectparser, int color);

void draw_lego(int partNum, int blkNum, int color,int charactor=0);

void setMaterial(int color);

void draw_axis(void);

struct modeling {
	int partNum;//부위 번호
	int blkNum;//블록 번호
	int color;
	//레고의 위치 및 회전
	float tx;
	float ty;
	float tz;
	float r;
};