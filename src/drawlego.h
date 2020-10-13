#pragma once
#include "ObjParser.h"

void initObject();

void draw_object(ObjParser *objectparser, int color);

void draw_lego(int partNum, int blkNum, int color,int charactor=0);

void setMaterial(int color);

void draw_axis(void);

struct modeling {
	int partNum;//���� ��ȣ
	int blkNum;//��� ��ȣ
	int color;
	//������ ��ġ �� ȸ��
	float tx;
	float ty;
	float tz;
	float r;
};