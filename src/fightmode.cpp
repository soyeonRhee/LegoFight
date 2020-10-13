#include <GL/glut.h>
#include <GL/freeglut.h>
#include <fstream>
#include "fightmode.h"
#include "drawlego.h"
#include "bmpfuncs.h"
#include "openmode.h"
using namespace std;

extern int _width, _height;
extern double xangle, yangle;
extern int r;
extern int bodyNum[2];
//충돌처리를 위해 bounding box 좌표값가져옴
extern aabb* abpartset[4];
//animation을 위해
extern int kick[2];
extern int jump[2];
extern int punch[2];

struct character {
	modeling chm[20];//block리스트
	int cCount=0;//속한 blk개수
	//충돌처리를 위한 캐릭터의 최대 최소 x,z
	int minx=150;
	int minz=150;
	int maxx=0;
	int maxz=0;
	//애니메이션 충돌처리위한 
	int kicklength;
	int punchlength;

	int tx=0;
	int ty=0;
	int tz=0;
	int r=0;

	float hp = 10;
};

character ch[2];

int fstate = 0;
const char* fguide[2] = { "CHOOSE CHARACTOR 1!","CHOOSE CHARACTOR 2!" };
const char* fightguide = "FIGHT!!";
const char* hpguide[2] = { "CHARACTER 1 HP","CHARACTER 2 HP" };
const char* winguide[2] = { "CHARACTER 1 WIN","CHARACTER 2 WIN" };

GLuint texName[2];	// make cube
typedef unsigned char uchar;

void cubeTextureMapping();
void draw_textureCube();
void material_default();
bool defaultcollision(int cNum, int tempx, int tempz);
bool kickcollision(int cNum);
bool punchcollision(int cNum);
void draw_charcter(int cNum);
void f_draw_string();

void fightinit(void) {

	/* Background color :  Black */
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	xangle = 360, yangle = 51.5;
	r = 265;
	fstate = 0;
	ch[0].cCount = 0;
	ch[1].cCount = 0;

	cubeTextureMapping();

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glEnable(GL_TEXTURE_2D);

	/* front face */
	glFrontFace(GL_CW);


	/* Enable depth buffer */
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_LIGHTING);

	GLfloat ambientMaterial[] = { 1.0,1.0,1.0,1.0 };
	GLfloat diffuseMaterial[] = { 0.7,0.7,0.7,1.0 };

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientMaterial);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseMaterial);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuseMaterial);

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);

	material_default();
};

void  fightkeyboard(unsigned char key, int x, int y) {
	int temp = 0;
	switch (key) {
	case 'w':
		//c1 go
		if (ch[0].r == 0.0) {
			temp = ch[0].tx + 10;
			if (defaultcollision(0, temp, ch[0].tz)) ch[0].tx = temp;
		}
		else if (ch[0].r == 90.0) {
			temp = ch[0].tz - 10;
			if (defaultcollision(0, ch[0].tx, temp)) ch[0].tz = temp;
		}
		else if (ch[0].r == 180.0) {
			temp = ch[0].tx - 10;
			if (defaultcollision(0, temp, ch[0].tz)) ch[0].tx = temp;
		}
		else {
			temp = ch[0].tz + 10;
			if (defaultcollision(0, ch[0].tx, temp)) ch[0].tz = temp;
		}
		break;
	case 's':
		//c1 back
		if (ch[0].r == 0.0) {
			temp = ch[0].tx - 10;
			if (defaultcollision(0, temp, ch[0].tz)) ch[0].tx = temp;
		}
		else if (ch[0].r == 90.0) {
			temp = ch[0].tz + 10;
			if (defaultcollision(0, ch[0].tx, temp)) ch[0].tz = temp;
		}
		else if (ch[0].r == 180.0) {
			temp = ch[0].tx + 10;
			if (defaultcollision(0, temp, ch[0].tz)) ch[0].tx = temp;
		}
		else {
			temp = ch[0].tz - 10;
			if (defaultcollision(0, ch[0].tx, temp)) ch[0].tz = temp;
		}
		break;
	case 'd':
		//c1 rotate
		if (ch[0].r == 270) ch[0].r = 0;
		else ch[0].r += 90;
		break;
	case 'a':
		//c1 rotate
		if (ch[0].r == 0.0) ch[0].r = 270.0;
		else ch[0].r -= 90;
		break;
	case 'q':
		//c1 kick
		kick[0] = 1;
		kickcollision(0);
		break;
	case '.':
		kick[1] = 1;
		kickcollision(1);
		break;
	case 'r':
		jump[0] = 1;
		break;
	case 13:
		jump[1] = 1;
		break;
	case 'e':
		if((punch[0]==2)|| (punch[0] == 4))punch[0]--;
		punchcollision(0);
		break;
	case '/':
		if ((punch[1] == 2) || (punch[1] == 4))punch[1]--;
		punchcollision(1);
		break;
	}
	glutPostRedisplay();
};

void  fightspecialkb(int key, int x, int y) {
	int temp = 0;
	switch (key) {
	case GLUT_KEY_UP:
		//c2 go
		if (ch[1].r == 0.0) {
			temp = ch[1].tx + 10;
			if (defaultcollision(1, temp, ch[1].tz)) ch[1].tx = temp;
		}
		else if (ch[1].r == 90.0) {
			temp = ch[1].tz - 10;
			if (defaultcollision(1, ch[1].tx, temp)) ch[1].tz = temp;
		}
		else if (ch[1].r == 180.0) {
			temp = ch[1].tx - 10;
			if (defaultcollision(1, temp, ch[1].tz)) ch[1].tx = temp;
		}
		else {
			temp = ch[1].tz + 10;
			if (defaultcollision(1, ch[1].tx, temp)) ch[1].tz = temp;
		}
		break;
	case GLUT_KEY_DOWN:
		//c2 back
		if (ch[1].r == 0.0) {
			temp = ch[1].tx - 10;
			if (defaultcollision(1, temp, ch[1].tz)) ch[1].tx = temp;
		}
		else if (ch[1].r == 90.0) {
			temp = ch[1].tz + 10;
			if (defaultcollision(1, ch[1].tx, temp)) ch[1].tz = temp;
		}
		else if (ch[1].r == 180.0) {
			temp = ch[1].tx + 10;
			if (defaultcollision(1, temp, ch[1].tz)) ch[1].tx = temp;
		}
		else {
			temp = ch[1].tz - 10;
			if (defaultcollision(1, ch[1].tx, temp)) ch[1].tz = temp;
		}
		break;
	case GLUT_KEY_RIGHT:
		//c2 rotate
		if (ch[1].r == 270) ch[1].r = 0;
		else ch[1].r += 90;
		break;
	case GLUT_KEY_LEFT:
		//c2 rotate
		if (ch[1].r == 0.0) ch[1].r = 270.0;
		else ch[1].r -= 90;
		break;

	}
	glutPostRedisplay();
};

void draw_hp() {
	for (int i = 0; i < 5; i++) {
		glPushMatrix();
			glColor3f(1, 1, 1);
			glTranslatef(30+i*22, -20, 0);
			glBegin(GL_POLYGON);
				glVertex2f(-16, 16);
				glVertex2f(-16, -16);
				glVertex2f(16, -16);
				glVertex2f(16, 16);
			glEnd();
		glPopMatrix();
	}
}

void fightdraw(void) {
	GLfloat light_position0[] = { 0.0,0.0,0,1.0 };
	GLfloat light_position1[] = { 0.0,500.0,0,0.0 };
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
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

	glPushMatrix();
	draw_textureCube();
	glPopMatrix();

	f_draw_string();

	glPushMatrix();
	glTranslatef(ch[0].tx, ch[0].ty, ch[0].tz);
	glRotatef(ch[0].r, 0, 1, 0);
	draw_charcter(0);
	glPopMatrix();
	if (jump[0] == 1) jump[0]++;

	glPushMatrix();
	glTranslatef(ch[1].tx, ch[1].ty, ch[1].tz);
	glRotatef(ch[1].r, 0, 1, 0);
	draw_charcter(1);
	glPopMatrix();
	if (jump[1] == 1) jump[1]++;

	draw_hp();

	glFlush();
	glutSwapBuffers();
};

//cNum==characoter 번호
//cNum번 캐릭터로 num파일을 불러오기
int fightopen(int num,int cNum) {
	fstate++;
	string line;
	int i = 0;
	int temp;
	int tempB, tempP;

	if (cNum == 1) {
		//겹쳐서 rendering되지 않도록
		ch[cNum].tz = -20;
	}
	else {
		ch[cNum].tz = 50;
	}
	string filename("save/(");
	filename = filename + to_string(num) + ").txt";
	ifstream in(filename);

	if (!in.is_open()) {
		cout << "error opening file" << endl;
		return 1;
	}
	while (1) {
		if (!getline(in, line)) break;
		line = line.substr(0, 2);
		temp= atoi(line.substr(1).c_str());//몇번째 블럭인지

		getline(in, line);
		tempP= atoi(line.c_str());
		ch[cNum].chm[temp].partNum = tempP;
		getline(in, line);
		tempB= atoi(line.c_str());
		ch[cNum].chm[temp].blkNum = tempB;
		//body blk partnum / armblk size위해
		if (tempP == 1) {
			bodyNum[cNum] = ch[cNum].chm[temp].blkNum;
		}
		getline(in, line);
		ch[cNum].chm[temp].color = atoi(line.c_str());
		getline(in, line);
		ch[cNum].chm[temp].tx = atoi(line.c_str());
		getline(in, line);
		ch[cNum].chm[temp].ty = atoi(line.c_str());
		getline(in, line);
		ch[cNum].chm[temp].tz = atoi(line.c_str());
		getline(in, line);
		ch[cNum].chm[temp].r = atoi(line.c_str());
		//leg blk rotation(항상 앞으로 서있게 시작하기 위해)
		if (tempP == 3) {
			ch[cNum].r = ch[cNum].chm[temp].r;
			ch[cNum].kicklength = abpartset[tempP][tempB].maxy;
		}
		//arm punchlength
		if (tempP == 2) {
			ch[cNum].punchlength = abpartset[tempP][tempB].maxy- abpartset[tempP][tempB].miny-5;
		}
		//character당 최대최소 x,z구하기
		if (ch[cNum].chm[temp].r == 0.0) {
			if (ch[cNum].chm[temp].tx+abpartset[tempP][tempB].minx < ch[cNum].minx)ch[cNum].minx = abpartset[tempP][tempB].minx;
			if (ch[cNum].chm[temp].tz+abpartset[tempP][tempB].minz < ch[cNum].minz)ch[cNum].minz = abpartset[tempP][tempB].minz;
			if (ch[cNum].chm[temp].tx+abpartset[tempP][tempB].maxx > ch[cNum].maxx)ch[cNum].maxx = abpartset[tempP][tempB].maxx;
			if (ch[cNum].chm[temp].tz+abpartset[tempP][tempB].maxz > ch[cNum].maxz)ch[cNum].maxz = abpartset[tempP][tempB].maxz;
		}
		else if (ch[cNum].chm[temp].r == 90.0) {
			if (ch[cNum].chm[temp].tx+abpartset[tempP][tempB].minz < ch[cNum].minx)ch[cNum].minx = abpartset[tempP][tempB].minx;
			if ((ch[cNum].chm[temp].tz -abpartset[tempP][tempB].maxx) < ch[cNum].minz)ch[cNum].minz = abpartset[tempP][tempB].minz;
			if (ch[cNum].chm[temp].tx+abpartset[tempP][tempB].maxz > ch[cNum].maxx)ch[cNum].maxx = abpartset[tempP][tempB].maxx;
			if ((ch[cNum].chm[temp].tz -abpartset[tempP][tempB].minx) > ch[cNum].maxz)ch[cNum].maxz = abpartset[tempP][tempB].maxz;
		}
		else if (ch[cNum].chm[temp].r=180) {
			if ((ch[cNum].chm[temp].tx -abpartset[tempP][tempB].maxx) < ch[cNum].minx)ch[cNum].minx = abpartset[tempP][tempB].minx;
			if (ch[cNum].chm[temp].tz-abpartset[tempP][tempB].maxz < ch[cNum].minz)ch[cNum].minz = abpartset[tempP][tempB].minz;
			if ((ch[cNum].chm[temp].tx -abpartset[tempP][tempB].minx) > ch[cNum].maxx)ch[cNum].maxx = abpartset[tempP][tempB].maxx;
			if (ch[cNum].chm[temp].tz-abpartset[tempP][tempB].minz > ch[cNum].maxz)ch[cNum].maxz = abpartset[tempP][tempB].maxz;
		}
		else if(ch[cNum].chm[temp].r=270){
			if (ch[cNum].chm[temp].tx-abpartset[tempP][tempB].maxz < ch[cNum].minx)ch[cNum].minx = abpartset[tempP][tempB].minx;
			if (ch[cNum].chm[temp].tz+abpartset[tempP][tempB].minx < ch[cNum].minz)ch[cNum].minz = abpartset[tempP][tempB].minz;
			if (ch[cNum].chm[temp].tx-abpartset[tempP][tempB].minz > ch[cNum].maxx)ch[cNum].maxx = abpartset[tempP][tempB].maxx;
			if (ch[cNum].chm[temp].tz+abpartset[tempP][tempB].maxx > ch[cNum].maxz)ch[cNum].maxz = abpartset[tempP][tempB].maxz;
		}
		ch[cNum].cCount++;
	}

	glutPostRedisplay();
	return 0;
};

void cubeTextureMapping() {
	glGenTextures(2, texName);
	int imgWidth, imgHeight, channels;
	glBindTexture(GL_TEXTURE_2D, texName[0]);
	uchar* img = readImageData("img/wall.bmp", &imgWidth, &imgHeight, &channels);
	printf("[cube]img width : %d, height : %d, ch : %d\n", imgWidth, imgHeight, channels);
	glTexImage2D(GL_TEXTURE_2D, 0, /*INPUT CHANNEL*/3, imgWidth, imgHeight, 0, /*TEXEL CHANNEL*/GL_RGB, GL_UNSIGNED_BYTE, img);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, texName[1]);
	img = readImageData("img/paper.bmp", &imgWidth, &imgHeight, &channels);
	printf("[cube]img width : %d, height : %d, ch : %d\n", imgWidth, imgHeight, channels);
	glTexImage2D(GL_TEXTURE_2D, 0, /*INPUT CHANNEL*/3, imgWidth, imgHeight, 0, /*TEXEL CHANNEL*/GL_RGB, GL_UNSIGNED_BYTE, img);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

void draw_textureCube() {
	glEnable(GL_TEXTURE_2D);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	glColor3f(1.0, 1.0, 1.0);	// white로 color를 set해주어야 texture색상이 제대로 적용 됨!
								//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE); // polygon의 원래 색상은 무시하고 texture로 덮음
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); // polygon의 원래 색상과 texture 색상을 곱하여 덮음, texture가 입혀진 표면에 광원 효과 설정 가능
																 // glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

	glBindTexture(GL_TEXTURE_2D, texName[0]);
	glBegin(GL_QUADS);
	glNormal3f(-1.0, 0, 0);	// -x axis
	glTexCoord2f(0, 0); glVertex3f(-300, 0, -300);
	glTexCoord2f(1, 0); glVertex3f(-300, 0, 300);
	glTexCoord2f(1, 1); glVertex3f(-300, 600, 300);
	glTexCoord2f(0, 1); glVertex3f(-300, 600, -300);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texName[0]);
	glBegin(GL_QUADS);
	glNormal3f(1.0, 0, 0);	//x axis
	glTexCoord2f(0, 0); glVertex3f(300, 600, -300);
	glTexCoord2f(1, 0); glVertex3f(300, 600, 300);
	glTexCoord2f(1, 1); glVertex3f(300, 0, 300);
	glTexCoord2f(0, 1); glVertex3f(300, 0, -300);
	glEnd();
	//bottom
	glBindTexture(GL_TEXTURE_2D, texName[1]);
	glBegin(GL_QUADS);
	glNormal3f(0, -1.0, 0);	// -y axis
	glTexCoord2f(0, 0); glVertex3f(300, 0, -300);
	glTexCoord2f(1, 0); glVertex3f(300, 0, 300);
	glTexCoord2f(1, 1); glVertex3f(-300, 0, 300);
	glTexCoord2f(0, 1); glVertex3f(-300, 0, -300);
	glEnd();
	
	glBindTexture(GL_TEXTURE_2D, texName[0]);
	glBegin(GL_QUADS);
	glNormal3f(0, 1.0, 0);	// y axis
	glTexCoord2f(0, 0); glVertex3f(300, 600, -300);
	glTexCoord2f(1, 0); glVertex3f(-300, 600, -300);
	glTexCoord2f(1, 1); glVertex3f(-300, 600, 300);
	glTexCoord2f(0, 1); glVertex3f(300, 600, 300);
	glEnd();
	
	glBindTexture(GL_TEXTURE_2D, texName[0]);
	glBegin(GL_QUADS);
	glNormal3f(0, 0, 1.0);	//z axis
	glTexCoord2f(0, 0); glVertex3f(-300, 0, 300);
	glTexCoord2f(1, 0); glVertex3f(300, 0, 300);
	glTexCoord2f(1, 1); glVertex3f(300, 600, 300);
	glTexCoord2f(0, 1); glVertex3f(-300, 600, 300);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texName[0]);
	glBegin(GL_QUADS);
	glNormal3f(0, 0, -1.0);	//-z축
	glTexCoord2f(1, 1); glVertex3f(-300,600, -300);
	glTexCoord2f(0, 1); glVertex3f(300, 600, -300);
	glTexCoord2f(0, 0); glVertex3f(300, 0, -300);
	glTexCoord2f(1, 0); glVertex3f(-300, 0, -300);
	glEnd();

	glColorMaterial(GL_FRONT, GL_DIFFUSE);
	glDisable(GL_TEXTURE_2D);
}

void material_default() {

	GLfloat ambientMaterial[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat diffuseMaterial[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat specularMaterial[] = { 0.9f, 0.9f, 0.9f, 1.0f };

	/************* Material  setting *************/
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambientMaterial);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseMaterial);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specularMaterial);
	glMaterialf(GL_FRONT, GL_SHININESS, 60);

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
}

void draw_charcter(int cNum) {
	if (ch[cNum].cCount == 0) return;
	if(fstate<15) fstate++;
	if (ch[cNum].hp <= 0) {
		fstate=20;
		if(cNum==0)glRotatef(90, 1, 0, 0); 
		else glRotatef(90, 0, 0, 1);
	}
	for (int i = 0; i < ch[cNum].cCount; i++) {
		glPushMatrix();
		glTranslatef(ch[cNum].chm[i].tx, ch[cNum].chm[i].ty, ch[cNum].chm[i].tz);
		glRotatef(ch[cNum].chm[i].r, 0, 1, 0);
		draw_lego(ch[cNum].chm[i].partNum, ch[cNum].chm[i].blkNum, ch[cNum].chm[i].color,cNum);
		glPopMatrix();
	}
}

//이동 충돌 검사 충돌시 return 0
bool defaultcollision(int cNum,int tempx,int tempz) {
	character me, opponent;
	if (cNum == 0) {
		me = ch[0]; opponent = ch[1];
	}
	else {
		me = ch[1]; opponent = ch[0];
	}

	if (me.r == 0) {
		if (opponent.r == 0) {
			//x축
			if ((tempx + me.minx) > (opponent.tx + opponent.maxx) ||
				(tempx + me.maxx) < (opponent.tx + opponent.minx)) {
				return true;
			}
			//z축
			if ((tempz + me.minz) > (opponent.tz + opponent.maxz) ||
				(tempz + me.maxz) < (opponent.tz + opponent.minz)) {
				return true;
			}
		}
		else if (opponent.r == 90) {
			//x축
			if ((tempx + me.minx) > (opponent.tx + opponent.maxz) ||
				(tempx + me.maxx) < (opponent.tx + opponent.minz)) {
				return true;
			}
			//z축
			if ((tempz + me.minz) > (opponent.tz - opponent.minx) ||
				(tempz + me.maxz) < (opponent.tz - opponent.maxx)) {
				return true;
			}
		}
		else if (opponent.r == 180) {
			//x축
			if ((tempx + me.minx) > (opponent.tx - opponent.minx) ||
				(tempx + me.maxx) < (opponent.tx - opponent.maxx)) {
				return true;
			}
			//z축
			if ((tempz + me.minz) > (opponent.tz - opponent.minz) ||
				(tempz + me.maxz) < (opponent.tz - opponent.maxz)) {
				return true;
			}
		}
		else if (opponent.r == 270) {
			//x축
			if ((tempx + me.minx) > (opponent.tx - opponent.minz) ||
				(tempx + me.maxx) < (opponent.tx - opponent.maxz)) {
				return true;
			}
			//z축
			if ((tempz + me.minz) > (opponent.tz + opponent.maxx) ||
				(tempz + me.maxz) < (opponent.tz + opponent.minx)) {
				return true;
			}
		}
	}
	else if (me.r == 90) {
		if (opponent.r == 0) {
			//x축
			if ((tempx + me.minz) > (opponent.tx + opponent.maxx) ||
				(tempx + me.maxz) < (opponent.tx + opponent.minx)) {
				return true;
			}
			//z축
			if ((tempz - me.maxx) > (opponent.tz + opponent.maxz) ||
				(tempz - me.minx) < (opponent.tz + opponent.minz)) {
				return true;
			}
		}
		else if (opponent.r == 90) {
			//x축
			if ((tempx + me.minz) > (opponent.tx + opponent.maxz) ||
				(tempx + me.maxz) < (opponent.tx + opponent.minz)) {
				return true;
			}
			//z축
			if ((tempz - me.maxx) > (opponent.tz - opponent.minx) ||
				(tempz - me.minx) < (opponent.tz - opponent.maxx)) {
				return true;
			}
		}
		else if (opponent.r == 180) {
			//x축
			if ((tempx + me.minz) > (opponent.tx - opponent.minx) ||
				(tempx + me.maxz) < (opponent.tx - opponent.maxx)) {
				return true;
			}
			//z축
			if ((tempz - me.maxx) > (opponent.tz - opponent.minz) ||
				(tempz - me.minx) < (opponent.tz - opponent.maxz)) {
				return true;
			}
		}
		else if (opponent.r == 270) {
			//x축
			if ((tempx + me.minz) > (opponent.tx - opponent.minz) ||
				(tempx + me.maxz) < (opponent.tx - opponent.maxz)) {
				return true;
			}
			//z축
			if ((tempz - me.maxx) > (opponent.tz + opponent.maxx) ||
				(tempz - me.minx) < (opponent.tz + opponent.minx)) {
				return true;
			}
		}
	}
	else if (me.r == 180) {
		if (opponent.r == 0) {
			//x축
			if ((tempx - me.maxx) > (opponent.tx + opponent.maxx) ||
				(tempx - me.minx) < (opponent.tx + opponent.minx)) {
				return true;
			}
			//z축
			if ((tempz - me.maxz) > (opponent.tz + opponent.maxz) ||
				(tempz - me.minz) < (opponent.tz + opponent.minz)) {
				return true;
			}
		}
		else if (opponent.r == 90) {
			//x축
			if ((tempx - me.maxx) > (opponent.tx - opponent.minx) ||
				(tempx - me.minx) < (opponent.tx - opponent.maxx)) {
				return true;
			}
			//z축
			if ((tempz - me.maxz) > (opponent.tz - opponent.minx) ||
				(tempz - me.minz) < (opponent.tz - opponent.maxx)) {
				return true;
			}
		}
		else if (opponent.r == 180) {
			//x축
			if ((tempx - me.maxx) > (opponent.tx - opponent.minx) ||
				(tempx - me.minx) < (opponent.tx - opponent.maxx)) {
				return true;
			}
			//z축
			if ((tempz - me.maxz) > (opponent.tz - opponent.minz) ||
				(tempz - me.minz) < (opponent.tz - opponent.maxz)) {
				return true;
			}
		}
		else if (opponent.r == 270) {
			//x축
			if ((tempx - me.maxx) > (opponent.tx - opponent.minz) ||
				(tempx - me.minx) < (opponent.tx - opponent.maxz)) {
				return true;
			}
			//z축
			if ((tempz - me.maxz) > (opponent.tz + opponent.maxx) ||
				(tempz - me.minz) < (opponent.tz + opponent.minx)) {
				return true;
			}
		}
	}
	else if (me.r == 270) {
		if (opponent.r == 0) {
			//x축
			if ((tempx - me.maxz) > (opponent.tx + opponent.maxx) ||
				(tempx - me.minz) < (opponent.tx + opponent.minx)) {
				return true;
			}
			//z축
			if ((tempz + me.minx) > (opponent.tz + opponent.maxz) ||
				(tempz + me.maxx) < (opponent.tz + opponent.minz)) {
				return true;
			}
		}
		else if (opponent.r == 90) {
			//x축
			if ((tempx - me.maxz) > (opponent.tx - opponent.minx) ||
				(tempx - me.minz) < (opponent.tx - opponent.maxx)) {
				return true;
			}
			//z축
			if ((tempz + me.minx) > (opponent.tz - opponent.minx) ||
				(tempz + me.maxx) < (opponent.tz - opponent.maxx)) {
				return true;
			}
		}
		else if (opponent.r == 180) {
			//x축
			if ((tempx - me.maxz) > (opponent.tx - opponent.minx) ||
				(tempx - me.minz) < (opponent.tx - opponent.maxx)) {
				return true;
			}
			//z축
			if ((tempz + me.minx) > (opponent.tz - opponent.minz) ||
				(tempz + me.maxx) < (opponent.tz - opponent.maxz)) {
				return true;
			}
		}
		else if (opponent.r == 270) {
			//x축
			if ((tempx - me.maxz) > (opponent.tx - opponent.minz) ||
				(tempx - me.minz) < (opponent.tx - opponent.maxz)) {
				return true;
			}
			//z축
			if ((tempz + me.minx) > (opponent.tz + opponent.maxx) ||
				(tempz + me.maxx) < (opponent.tz + opponent.minx)) {
				return true;
			}
		}
	}
	return false;
}

bool kickcollision(int cNum) {
	character me, opponent;
	if (cNum == 0) {
		me = ch[0]; opponent = ch[1];
	}
	else {
		me = ch[1]; opponent = ch[0];
	}

	if (me.r == 0) {
		if (opponent.r == 0) {
			//x축
			if ((me.tx + me.minx) > (opponent.tx + opponent.maxx)||
				(me.tx + me.kicklength) < (opponent.tx + opponent.minx)) {
				return true;
			}
			//z축
			if ((me.tz + me.minz) > (opponent.tz + opponent.maxz) ||
				(me.tz + me.maxz) < (opponent.tz + opponent.minz)) {
				return true;
			}
		}
		else if (opponent.r == 90) {
			//x축
			if ((me.tx + me.minx) > (opponent.tx + opponent.maxz) || 
				(me.tx + me.kicklength) < (opponent.tx + opponent.minz)) {
				return true;
			}//z축
			if ((me.tz + me.minz) > (opponent.tz - opponent.minx) ||
				(me.tz + me.maxz) < (opponent.tz - opponent.maxx)) {
				return true;
			}
		}
		else if (opponent.r == 180) {
			//x축
			if ((me.tx + me.minx) > (opponent.tx - opponent.minx) || 
				(me.tx + me.kicklength) < (opponent.tx - opponent.maxx)) {
				return true;
			}//z축
			if ((me.tz + me.minz) > (opponent.tz - opponent.minz) ||
				(me.tz + me.maxz) < (opponent.tz - opponent.maxz)) {
				return true;
			}
		}
		else if (opponent.r == 270) {
			//x축
			if ((me.tx + me.minx) > (opponent.tx - opponent.minz) || 
				(me.tx + me.kicklength) < (opponent.tx - opponent.maxz)) {
				return true;
			}//z축
			if ((me.tz + me.minz) > (opponent.tz + opponent.maxx) ||
				(me.tz + me.maxz) < (opponent.tz + opponent.minx)) {
				return true;
			}
		}
		//뒤로 날라감
		if(cNum==0) ch[1].tx -= 20;
		else ch[0].tx -= 20;
	}
	else if (me.r == 90) {
		if (opponent.r == 0) {
			//x축
			if ((me.tx + me.minz) > (opponent.tx + opponent.maxx) ||
				(me.tx + me.maxz) < (opponent.tx + opponent.minx)) {
				return true;
			}
			//z축
			if ((me.tz - me.kicklength) > (opponent.tz + opponent.maxz) ||
				(me.tz - me.minx) < (opponent.tz + opponent.minz)) {
				return true;
			}
		}
		else if (opponent.r == 90) {
			//x축
			if ((me.tx + me.minz) > (opponent.tx + opponent.maxz) ||
				(me.tx + me.maxz) < (opponent.tx + opponent.minz)) {
				return true;
			}
			//z축
			if ((me.tz - me.kicklength) > (opponent.tz - opponent.minx) ||
				(me.tz - me.minx) < (opponent.tz - opponent.maxx)) {
				return true;
			}
		}
		else if (opponent.r == 180) {
			//x축
			if ((me.tx + me.minz) > (opponent.tx - opponent.minx) ||
				(me.tx + me.maxz) < (opponent.tx - opponent.maxx)) {
				return true;
			}
			//z축
			if ((me.tz - me.kicklength) > (opponent.tz - opponent.minz) ||
				(me.tz - me.minx) < (opponent.tz - opponent.maxz)) {
				return true;
			}
		}
		else if (opponent.r == 270) {
			//x축
			if ((me.tx + me.minz) > (opponent.tx - opponent.minz) ||
				(me.tx + me.maxz) < (opponent.tx - opponent.maxz)) {
				return true;
			}
			//z축
			if ((me.tz - me.kicklength) > (opponent.tz + opponent.maxx) ||
				(me.tz - me.minx) < (opponent.tz + opponent.minx)) {
				return true;
			}
		}//뒤로 날라감
		if (cNum == 0) ch[1].tz -= 20;
		else ch[0].tz -= 20;
	}
	//잘못
	else if (me.r == 180) {
		if (opponent.r == 0) {
			//x축
			if ((me.tx - me.kicklength) > (opponent.tx + opponent.maxx) ||
				(me.tx - me.minx) < (opponent.tx + opponent.minx)) {
				return true;
			}//z축
			if ((me.tz - me.maxz) > (opponent.tz + opponent.maxz) ||
				(me.tz - me.minz) < (opponent.tz + opponent.minz)) {
				return true;
			}

		}
		else if (opponent.r == 90) {
			//x축
			if ((me.tx - me.kicklength) > (opponent.tx - opponent.minx) ||
				(me.tx - me.maxz) < (opponent.tx - opponent.maxx)) {
				return true;
			}//z축
			if ((me.tz - me.maxz) > (opponent.tz - opponent.minx) ||
				(me.tz - me.minz) < (opponent.tz - opponent.maxx)) {
				return true;
			}
		}
		else if (opponent.r == 180) {
			//x축
			if ((me.tx - me.kicklength) > (opponent.tx - opponent.minx) ||
				(me.tx - me.minx) < (opponent.tx - opponent.maxx)) {
				return true;
			}//z축
			if ((me.tz - me.maxz) > (opponent.tz - opponent.minz) ||
				(me.tz - me.minz) < (opponent.tz - opponent.maxz)) {
				return true;
			}
		}
		else if (opponent.r == 270) {
			//x축
			if ((me.tx - me.kicklength)> (opponent.tx - opponent.minx) ||
				(me.tx - me.minx) < (opponent.tx - opponent.maxx)) {
				return true;
			}//z축
			if ((me.tz - me.maxz) > (opponent.tz + opponent.maxx) ||
				(me.tz - me.minz) < (opponent.tz + opponent.minx)) {
				return true;
			}
		}//뒤로 날라감
		if (cNum == 0) ch[1].tx += 20;
		else ch[0].tx += 20;
	}
	else if (me.r == 270) {
		if (opponent.r == 0) {
			//x축
			if ((me.tx - me.maxz) > (opponent.tx + opponent.maxx) ||
				(me.tx - me.minz) < (opponent.tx + opponent.minx)) {
				return true;
			}
			//z축
			if ((me.tz - me.maxz) > (opponent.tz + opponent.maxx) || 
				(me.tz + me.kicklength) < (opponent.tz + opponent.minz)) {
				return true;
			}
		}
		else if (opponent.r == 90) {
			//x축
			if ((me.tx - me.maxz) > (opponent.tx - opponent.minx) ||
				(me.tx - me.minz) < (opponent.tx - opponent.maxx)) {
				return true;
			}
			//z축
			if ((me.tz + me.minx) > (opponent.tz + opponent.maxz) || 
				(me.tz + me.kicklength) < (opponent.tz - opponent.maxx)) {
				return true;
			}
		}
		else if (opponent.r == 180) {
			//x축
			if ((me.tx - me.maxz) > (opponent.tx - opponent.minx) ||
				(me.tx - me.minz) < (opponent.tx - opponent.maxx)) {
				return true;
			}
			//z축
			if ((me.tz + me.minx) > (opponent.tz - opponent.minx) || 
				(me.tz + me.kicklength) < (opponent.tz - opponent.maxz)) {
				return true;
			}
		}
		else if (opponent.r == 270) {
			//x축
			if ((me.tx - me.maxz) > (opponent.tx - opponent.minz) ||
				(me.tx - me.minz) < (opponent.tx - opponent.maxz)) {
				return true;
			}
			//z축
			if ((me.tz + me.minx) > (opponent.tz - opponent.minz) || 
				(me.tz + me.kicklength) < (opponent.tz + opponent.minx)) {
				return true;
			}
		}//뒤로 날라감
		if (cNum == 0) ch[1].tz += 20;
		else ch[0].tz += 20;
	}
	if (cNum == 0) ch[1].hp -= 1;
	else ch[0].hp -= 1;

	glutPostRedisplay();
	return false;
}

bool punchcollision(int cNum) {
	character me, opponent;
	if (cNum == 0) {
		me = ch[0]; opponent = ch[1];
	}
	else {
		me = ch[1]; opponent = ch[0];
	}

	if (me.r == 0) {
		if (opponent.r == 0) {
			//x축
			if ((me.tx + me.minx) > (opponent.tx + opponent.maxx) ||
				(me.tx + me.punchlength) < (opponent.tx + opponent.minx)) {
				return true;
			}
			//z축
			if ((me.tz + me.minz) > (opponent.tz + opponent.maxz) ||
				(me.tz + me.maxz) < (opponent.tz + opponent.minz)) {
				return true;
			}
		}
		else if (opponent.r == 90) {
			//x축
			if ((me.tx + me.minx) > (opponent.tx + opponent.maxz) ||
				(me.tx + me.punchlength) < (opponent.tx + opponent.minz)) {
				return true;
			}//z축
			if ((me.tz + me.minz) > (opponent.tz - opponent.minx) ||
				(me.tz + me.maxz) < (opponent.tz - opponent.maxx)) {
				return true;
			}
		}
		else if (opponent.r == 180) {
			//x축
			if ((me.tx + me.minx) > (opponent.tx - opponent.minx) ||
				(me.tx + me.punchlength) < (opponent.tx - opponent.maxx)) {
				return true;
			}//z축
			if ((me.tz + me.minz) > (opponent.tz - opponent.minz) ||
				(me.tz + me.maxz) < (opponent.tz - opponent.maxz)) {
				return true;
			}
		}
		else if (opponent.r == 270) {
			//x축
			if ((me.tx + me.minx) > (opponent.tx - opponent.minz) ||
				(me.tx + me.punchlength) < (opponent.tx - opponent.maxz)) {
				return true;
			}//z축
			if ((me.tz + me.minz) > (opponent.tz + opponent.maxx) ||
				(me.tz + me.maxz) < (opponent.tz + opponent.minx)) {
				return true;
			}
		}
		//뒤로 날라감
		if (cNum == 0) ch[1].tx -= 10;
		else ch[0].tx -= 10;
	}
	else if (me.r == 90) {
		if (opponent.r == 0) {
			//x축
			if ((me.tx + me.minz) > (opponent.tx + opponent.maxx) ||
				(me.tx + me.maxz) < (opponent.tx + opponent.minx)) {
				return true;
			}
			//z축
			if ((me.tz - me.punchlength) > (opponent.tz + opponent.maxz) ||
				(me.tz - me.minx) < (opponent.tz + opponent.minz)) {
				return true;
			}
		}
		else if (opponent.r == 90) {
			//x축
			if ((me.tx + me.minz) > (opponent.tx + opponent.maxz) ||
				(me.tx + me.maxz) < (opponent.tx + opponent.minz)) {
				return true;
			}
			//z축
			if ((me.tz - me.punchlength) > (opponent.tz - opponent.minx) ||
				(me.tz - me.minx) < (opponent.tz - opponent.maxx)) {
				return true;
			}
		}
		else if (opponent.r == 180) {
			//x축
			if ((me.tx + me.minz) > (opponent.tx - opponent.minx) ||
				(me.tx + me.maxz) < (opponent.tx - opponent.maxx)) {
				return true;
			}
			//z축
			if ((me.tz - me.punchlength) > (opponent.tz - opponent.minz) ||
				(me.tz - me.minx) < (opponent.tz - opponent.maxz)) {
				return true;
			}
		}
		else if (opponent.r == 270) {
			//x축
			if ((me.tx + me.minz) > (opponent.tx - opponent.minz) ||
				(me.tx + me.maxz) < (opponent.tx - opponent.maxz)) {
				return true;
			}
			//z축
			if ((me.tz - me.punchlength) > (opponent.tz + opponent.maxx) ||
				(me.tz - me.minx) < (opponent.tz + opponent.minx)) {
				return true;
			}
		}//뒤로 날라감
		if (cNum == 0) ch[1].tz -= 10;
		else ch[0].tz -= 10;
	}
	//잘못
	else if (me.r == 180) {
		if (opponent.r == 0) {
			//x축
			if ((me.tx - me.punchlength) > (opponent.tx + opponent.maxx) ||
				(me.tx - me.minx) < (opponent.tx + opponent.minx)) {
				return true;
			}//z축
			if ((me.tz - me.maxz) > (opponent.tz + opponent.maxz) ||
				(me.tz - me.minz) < (opponent.tz + opponent.minz)) {
				return true;
			}

		}
		else if (opponent.r == 90) {
			//x축
			if ((me.tx - me.punchlength) > (opponent.tx - opponent.minx) ||
				(me.tx - me.maxz) < (opponent.tx - opponent.maxx)) {
				return true;
			}//z축
			if ((me.tz - me.maxz) > (opponent.tz - opponent.minx) ||
				(me.tz - me.minz) < (opponent.tz - opponent.maxx)) {
				return true;
			}
		}
		else if (opponent.r == 180) {
			//x축
			if ((me.tx - me.punchlength) > (opponent.tx - opponent.minx) ||
				(me.tx - me.minx) < (opponent.tx - opponent.maxx)) {
				return true;
			}//z축
			if ((me.tz - me.maxz) > (opponent.tz - opponent.minz) ||
				(me.tz - me.minz) < (opponent.tz - opponent.maxz)) {
				return true;
			}
		}
		else if (opponent.r == 270) {
			//x축
			if ((me.tx - me.punchlength)> (opponent.tx - opponent.minx) ||
				(me.tx - me.minx) < (opponent.tx - opponent.maxx)) {
				return true;
			}//z축
			if ((me.tz - me.maxz) > (opponent.tz + opponent.maxx) ||
				(me.tz - me.minz) < (opponent.tz + opponent.minx)) {
				return true;
			}
		}//뒤로 날라감
		if (cNum == 0) ch[1].tx += 10;
		else ch[0].tx += 10;
	}
	else if (me.r == 270) {
		if (opponent.r == 0) {
			//x축
			if ((me.tx - me.maxz) > (opponent.tx + opponent.maxx) ||
				(me.tx - me.minz) < (opponent.tx + opponent.minx)) {
				return true;
			}
			//z축
			if ((me.tz - me.maxz) > (opponent.tz + opponent.maxx) ||
				(me.tz + me.punchlength) < (opponent.tz + opponent.minz)) {
				return true;
			}
		}
		else if (opponent.r == 90) {
			//x축
			if ((me.tx - me.maxz) > (opponent.tx - opponent.minx) ||
				(me.tx - me.minz) < (opponent.tx - opponent.maxx)) {
				return true;
			}
			//z축
			if ((me.tz + me.minx) > (opponent.tz + opponent.maxz) ||
				(me.tz + me.punchlength) < (opponent.tz - opponent.maxx)) {
				return true;
			}
		}
		else if (opponent.r == 180) {
			//x축
			if ((me.tx - me.maxz) > (opponent.tx - opponent.minx) ||
				(me.tx - me.minz) < (opponent.tx - opponent.maxx)) {
				return true;
			}
			//z축
			if ((me.tz + me.minx) > (opponent.tz - opponent.minx) ||
				(me.tz + me.punchlength) < (opponent.tz - opponent.maxz)) {
				return true;
			}
		}
		else if (opponent.r == 270) {
			//x축
			if ((me.tx - me.maxz) > (opponent.tx - opponent.minz) ||
				(me.tx - me.minz) < (opponent.tx - opponent.maxz)) {
				return true;
			}
			//z축
			if ((me.tz + me.minx) > (opponent.tz - opponent.minz) ||
				(me.tz + me.punchlength) < (opponent.tz + opponent.minx)) {
				return true;
			}
		}//뒤로 날라감
		if (cNum == 0) ch[1].tz += 10;
		else ch[0].tz += 10;
	}
	if (cNum == 0) ch[1].hp -= 0.5;
	else ch[0].hp -= 0.5;

	glutPostRedisplay();
	return false;
}

void f_draw_string() {
	const char* str;
	if(fstate<2) str = fguide[fstate];
	else str = fightguide;
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
	if (fstate >= 2) {
		glRasterPos3f(-9.5, -8, 0);
		for (unsigned int i = 0; i < strlen(hpguide[0]); i++) {
			glutBitmapCharacter(GLUT_BITMAP_9_BY_15, hpguide[0][i]);

		}
		glRasterPos3f(4.5, -8, 0);
		for (unsigned int i = 0; i < strlen(hpguide[1]); i++) {
			glutBitmapCharacter(GLUT_BITMAP_9_BY_15, hpguide[1][i]);
		}
		//체력 그리기
		glColor3f(1, 0, 0);
		if (_width <= 500)glPointSize(7.0f);
		else glPointSize(20.0f);
		for (int i = 0; i < ch[0].hp; i++) {
			glBegin(GL_POINTS);
			glVertex2f(-9+i*0.4, -9);
			glEnd();
		}
		for (int i = 0; i < ch[1].hp; i++) {
			glBegin(GL_POINTS);
			glVertex2f(5 + i * 0.4, -9);
			glEnd();
		}
		//start fight message
		glColor3f(1, 0, 0);
		if (fstate<15) {
			glRasterPos3f(-1, 2.0, 0);
			for (unsigned int i = 0; i < strlen(str); i++) {
				glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str[i]);
			}
		}
		//win message
		else if (fstate == 20) {
			const char* win;
			if (ch[1].hp <= 0)win = winguide[0];
			else win = winguide[1];
			glRasterPos3f(-2, 2.0, 0);
			for (unsigned int i = 0; i < strlen(win); i++) {
				glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, win[i]);
			}
		}
	}
	else if(fstate < 2){
		glColor3f(1, 1, 0);
		glRasterPos3f(-2, 2.0, 0);
		for (unsigned int i = 0; i < strlen(str); i++) {
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str[i]);
		}
	}
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopAttrib();
}