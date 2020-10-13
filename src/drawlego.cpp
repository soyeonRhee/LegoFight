#include "drawlego.h"
#include "ObjParser.h"

ObjParser** partset[4];
ObjParser * head[3];
ObjParser * body[3];
ObjParser * arm[12];
ObjParser * leg[12];

float legx[6] = { 6.8,10.0,17.0,6.8,10,17 };
float legy[6] = { 22.5,23.5,24.5,32.5,33.5,34.5 };
float legz[6] = { -7,-9.5,-12,-7,-9.5,-12 };
float army[6] = { 48,47,47,37,56,57 };
float armz[6] = { -12,-15,-18,-15,-18,-21 };
float bodysize[3] = { 27,36,45 };
extern int bodyNum[2];

extern int previewtf;

//kick을 지시하는데서 이걸 1로 바꿀거
int kick[2] = { 4 ,4};
int jump[2] = { 2,2 };
int punch[2] = { 2,2 };

float kickx[6] = { 17.7,16.3,13.5,27.7,26.0,23.5 };
float kicky[6] = { 13.3,13.0,10.5,21.3,21,17.8 };
float punchx[6] = {4.0,5.0,5.3,30.5,9.0,8.0};
float punchy[6] = {4.0,4.5,5.5,26.0,7.0,7.0};
//충돌처리 bounding box값
aabb abhead[3];
aabb abbody[3];
aabb abarm[12];
aabb ableg[6];
aabb* abpartset[4] = { abhead,abbody,abarm,ableg };

int angle = 0;

/* partSet : 0- head, 1-body, 2-arm, 3-leg*/
/* 0head,1body : 0- thin 1-regular 2-thick*/
/* 2arm,3leg : 0- short&thin 1-short&regular 2-short&thick
*            3- long&thin  4-long&regular  5-long&thick*/
void initObject() {
	/* partSet : 0- head, 1-body, 2-arm, 3-leg*/
	partset[0] = head;
	partset[1] = body;
	partset[2] = arm;
	partset[3] = leg;
	/* 0head,1body : 0- thin 1-regular 2-thick*/
	/* 2arm,3leg : 01- short&thin 23-short&regular 45-short&thick
	*            67- long&thin  89-long&regular  1011-long&thick*/
	head[0] = new ObjParser("obj/thin_head.obj",0,0);
	head[1] = new ObjParser("obj/default_head.obj",0,1);
	head[2] = new ObjParser("obj/thick_head.obj",0,2);

	body[0] = new ObjParser("obj/thin_body.obj",1,0);
	body[1] = new ObjParser("obj/default_body.obj",1,1);
	body[2] = new ObjParser("obj/thick_body.obj",1,2);

	arm[0] = new ObjParser("obj/thin_shortarmleft.obj",2,0);
	arm[1] = new ObjParser("obj/thin_shortarmright.obj",2,1);
	arm[2] = new ObjParser("obj/default_shortarmleft.obj",2,2);
	arm[3] = new ObjParser("obj/default_shortarmright.obj",2,3);
	arm[4] = new ObjParser("obj/thick_shortarmleft.obj",2,4);
	arm[5] = new ObjParser("obj/thick_shortarmright.obj",2,5);
	arm[6] = new ObjParser("obj/thin_longarmleft.obj",2,6);
	arm[7] = new ObjParser("obj/thin_longarmright.obj",2,7);
	arm[8] = new ObjParser("obj/default_longarmleft.obj",2,8);
	arm[9] = new ObjParser("obj/default_longarmright.obj",2,9);
	arm[10] = new ObjParser("obj/thick_longarmleft.obj",2,10);
	arm[11] = new ObjParser("obj/thick_longarmright.obj",2,11);

	leg[0] = new ObjParser("obj/thin_shortlegleft.obj",3,0);
	leg[1] = new ObjParser("obj/thin_shortlegright.obj");
	leg[2] = new ObjParser("obj/default_shortlegleft.obj",3,1);
	leg[3] = new ObjParser("obj/default_shortlegright.obj");
	leg[4] = new ObjParser("obj/thick_shortlegleft.obj",3,2);
	leg[5] = new ObjParser("obj/thick_shortlegright.obj");
	leg[6] = new ObjParser("obj/thin_longlegleft.obj",3,3);
	leg[7] = new ObjParser("obj/thin_longlegright.obj");
	leg[8] = new ObjParser("obj/default_longlegleft.obj",3,4);
	leg[9] = new ObjParser("obj/default_longlegright.obj");
	leg[10] = new ObjParser("obj/thick_longlegleft.obj",3,5);
	leg[11] = new ObjParser("obj/thick_longlegright.obj");

}
void draw_lego(int partNum, int blkNum, int color,int character) {
	/* 2arm,3leg : 01- short&thin 23-short&regular 45-short&thick
	*            67- long&thin  89-long&regular  1011-long&thick*/
	if (jump[character] < 2) {
		glTranslatef(0, 30, 0);
		glutPostRedisplay();
	}
	switch (partNum)
	{
	case 0:
	case 1:
		draw_object(partset[partNum][blkNum], color);
		break;
	case 2:
		glTranslatef(0, army[blkNum], 0);
		glPushMatrix();
		//rightarm punch
		if (punch[character] == 1) {
			glTranslatef(punchx[blkNum], punchy[blkNum], 0);
			glRotatef(80, 0, 0, 1);
			punch[character] = 4;
			glutPostRedisplay();
		}
		draw_object(partset[partNum][blkNum * 2+1], color);
		glPopMatrix();
		glPushMatrix();
		//leftharm punch
		if (punch[character] == 3) {
			glTranslatef(punchx[blkNum], punchy[blkNum], 0);
			glRotatef(80, 0, 0, 1);
			punch[character] = 2;
			glutPostRedisplay();
		}
		glTranslatef(0, 0, -bodysize[bodyNum[character]] + armz[blkNum]);
		draw_object(partset[partNum][blkNum * 2], color);
		glPopMatrix();
		break;
	case 3:
		draw_object(partset[partNum][blkNum * 2], color);
		glTranslatef(legx[blkNum], legy[blkNum], legz[blkNum]);
		glPushMatrix();
		//kickanimation
		if (kick[character] == 1) {
			glTranslatef(kickx[blkNum], kicky[blkNum], 0);
			glRotatef(80, 0, 0, 1);
			kick[character] = 2;
			glutPostRedisplay();
		}
		draw_object(partset[partNum][blkNum * 2 + 1], color);
		glPopMatrix();
		break;
	default:
		break;
	}
}

void draw_object(ObjParser *objectparser, int color) {

	setMaterial(color);//재질 인자 설정

	glBegin(GL_TRIANGLES);
	for (unsigned int n = 0; n < objectparser->getFaceSize(); n += 3) {

		glNormal3f(objectparser->normal[objectparser->normalIdx[n] - 1].x,
			objectparser->normal[objectparser->normalIdx[n] - 1].y,
			objectparser->normal[objectparser->normalIdx[n] - 1].z);
		glVertex3f(objectparser->vertices[objectparser->vertexIdx[n] - 1].x,
			objectparser->vertices[objectparser->vertexIdx[n] - 1].y,
			objectparser->vertices[objectparser->vertexIdx[n] - 1].z);
		glNormal3f(objectparser->normal[objectparser->normalIdx[n + 1] - 1].x,
			objectparser->normal[objectparser->normalIdx[n + 1] - 1].y,
			objectparser->normal[objectparser->normalIdx[n + 1] - 1].z);
		glVertex3f(objectparser->vertices[objectparser->vertexIdx[n + 1] - 1].x,
			objectparser->vertices[objectparser->vertexIdx[n + 1] - 1].y,
			objectparser->vertices[objectparser->vertexIdx[n + 1] - 1].z);

		glNormal3f(objectparser->normal[objectparser->normalIdx[n + 2] - 1].x,
			objectparser->normal[objectparser->normalIdx[n + 2] - 1].y,
			objectparser->normal[objectparser->normalIdx[n + 2] - 1].z);
		glVertex3f(objectparser->vertices[objectparser->vertexIdx[n + 2] - 1].x,
			objectparser->vertices[objectparser->vertexIdx[n + 2] - 1].y,
			objectparser->vertices[objectparser->vertexIdx[n + 2] - 1].z);
	}
	glEnd();
}


void setMaterial(int color) {
	
	float a = 0.1;//ambient
	float alpha = 1.0;
	if (previewtf == 1) a = 0.3;

	//peach(skin)
	if (color == 0) {
		glColor4f((float)255/255, (float)204/255, (float)153/255, alpha);
		GLfloat ambientMaterial[] = { a,a,a,1.0 };
		GLfloat specularMaterial[] = { 0.0,0,0,0.0,1.0 };
		glMaterialfv(GL_FRONT, GL_AMBIENT, ambientMaterial);
		glMaterialfv(GL_FRONT, GL_SPECULAR, specularMaterial);
		glMaterialf(GL_FRONT, GL_SHININESS, 20.0);
	}
	//red
	else if (color == 1) {
		glColor4f(1, 0, 0, alpha);
		GLfloat ambientMaterial[] = { a,a,a,1.0 };
		GLfloat specularMaterial[] = { 0.0,0.0,0.0,1.0 };
		glMaterialfv(GL_FRONT, GL_AMBIENT, ambientMaterial);
		glMaterialfv(GL_FRONT, GL_SPECULAR, specularMaterial);
		glMaterialf(GL_FRONT, GL_SHININESS, 20.0);
	}
	//green(limegreen)
	else if (color == 2) {
		glColor4f((float)50 / 255, (float)205 / 255, (float)50 / 255, alpha);
		GLfloat ambientMaterial[] = { a,a,a,1.0 };
		GLfloat specularMaterial[] = { 0.0,0.0,0.0,1.0 };
		glMaterialfv(GL_FRONT, GL_AMBIENT, ambientMaterial);
		glMaterialfv(GL_FRONT, GL_SPECULAR, specularMaterial);
		glMaterialf(GL_FRONT, GL_SHININESS, 20.0);
	}
	//blue(royalblue)
	else if (color == 3) {
		glColor4f((float)65 / 255, (float)105 / 255, (float)225 / 255, alpha);
		GLfloat ambientMaterial[] = { a,a,a,1.0 };
		GLfloat specularMaterial[] = { 0.0,0.0,0.0,1.0 };
		glMaterialfv(GL_FRONT, GL_AMBIENT, ambientMaterial);
		glMaterialfv(GL_FRONT, GL_SPECULAR, specularMaterial);
		glMaterialf(GL_FRONT, GL_SHININESS, 20.0);
	}
	//cyan
	else if (color == 4) {
		glColor4f(0.0, 1, 1, alpha);
		GLfloat ambientMaterial[] = { a,a,a,1.0 };
		GLfloat specularMaterial[] = { 0.0,0.0,0.0,1.0 };
		glMaterialfv(GL_FRONT, GL_AMBIENT, ambientMaterial);
		glMaterialfv(GL_FRONT, GL_SPECULAR, specularMaterial);
		glMaterialf(GL_FRONT, GL_SHININESS, 20.0);
	}
	//magenta
	else if (color == 5) {
		glColor4f(1, 0, 1, alpha);
		GLfloat ambientMaterial[] = { a,a,a,1.0 };
		GLfloat specularMaterial[] = { 0.0,0.0,0.0,1.0 };
		glMaterialfv(GL_FRONT, GL_AMBIENT, ambientMaterial);
		glMaterialfv(GL_FRONT, GL_SPECULAR, specularMaterial);
		glMaterialf(GL_FRONT, GL_SHININESS, 20.0);
	}
	//yellow
	else if (color == 6) {
		glColor4f(1, 1,0, alpha);
		GLfloat ambientMaterial[] = { a,a,a,1.0 };
		GLfloat specularMaterial[] = { 0.0,0.0,0.0,1.0 };
		glMaterialfv(GL_FRONT, GL_AMBIENT, ambientMaterial);
		glMaterialfv(GL_FRONT, GL_SPECULAR, specularMaterial);
		glMaterialf(GL_FRONT, GL_SHININESS, 20.0);
	}
	//black
	else if (color == 7) {
		glColor4f((float)30 / 255, (float)30 / 255, (float)30 / 255, alpha);
		GLfloat ambientMaterial[] = { a,a,a,1.0 };
		GLfloat specularMaterial[] = { 0.0,0.0,0.0,1.0 };
		glMaterialfv(GL_FRONT, GL_AMBIENT, ambientMaterial);
		glMaterialfv(GL_FRONT, GL_SPECULAR, specularMaterial);
		glMaterialf(GL_FRONT, GL_SHININESS, 20.0);
	}
	//white(ivory)
	else if (color == 8) {
		glColor4f((float)1, 1, 1, alpha);
		GLfloat ambientMaterial[] = { a,a,a,1.0 };
		GLfloat specularMaterial[] = { 0.0,0.0,0.0,1.0 };
		glMaterialfv(GL_FRONT, GL_AMBIENT, ambientMaterial);
		glMaterialfv(GL_FRONT, GL_SPECULAR, specularMaterial);
		glMaterialf(GL_FRONT, GL_SHININESS, 20.0);
	}
	//lavender
	//mediumpurple	#9370DB	rgb(147, 112, 219)
	else if (color == 9) {
		glColor4f((float)147 / 255, (float)112 / 255, (float)219 / 255, alpha);
		GLfloat ambientMaterial[] = { a,a,a,1.0 };
		GLfloat specularMaterial[] = { 0.0,0.0,0.0,1.0 };
		glMaterialfv(GL_FRONT, GL_AMBIENT, ambientMaterial);
		glMaterialfv(GL_FRONT, GL_SPECULAR, specularMaterial);
		glMaterialf(GL_FRONT, GL_SHININESS, 20.0);
	}
}


void draw_axis(void) {
	glLineWidth(3);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	GLfloat specularMaterial[] = { 0.5,0.5,0.5,1.0 };

	glMaterialfv(GL_FRONT, GL_SPECULAR, specularMaterial);
	glMaterialf(GL_FRONT, GL_SHININESS, 20.0);
	glBegin(GL_LINES);

	glColor3f(1, 0, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(40, 0, 0);

	glColor3f(0, 1, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 40, 0);

	glColor3f(0, 0, 1);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, -40);
	glEnd();
	glColorMaterial(GL_FRONT, GL_DIFFUSE);
	glLineWidth(1);
}