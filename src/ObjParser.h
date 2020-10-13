/************************************************************************
* Simple objParser														*
* This simple objParser works under STRICT RULES using .obj file		*
* 1. Triangle meshs only												*
* 2. Only one object in obj file										*
* 3. Not considering mtl files											*
* 4. Must include faces informations	(vertex, uv, normal)			*
*************************************************************************/

#pragma once
#pragma warning (disable : 4996)
#include <iostream>
#include <vector>
#include <string>
#include <GL/freeglut.h>
#include <GL/glut.h>

using std::vector;

struct aabb {
	float minx;
	float maxx;
	float miny;
	float maxy;
	float minz;
	float maxz;
};

extern aabb abhead[3];
extern aabb abbody[3];
extern aabb abarm[12];
extern aabb ableg[6];

struct Vec3f {
	float x, y, z;
};

struct Vec2f {
	float x, y;
};

class ObjParser
{
public:
	ObjParser();
	ObjParser(const char * path);
	ObjParser(const char * path, int partNum, int blkNum);
	~ObjParser();
	void read(const char* path);
	void write(const char* path);
	void clear();
	unsigned int getFaceSize();
	std::string obj_name;
	vector<Vec3f> vertices;
	vector<Vec2f> textures;
	vector<Vec3f> normal;
	vector<unsigned int> vertexIdx, textureIdx, normalIdx;
private:
};


inline ObjParser::ObjParser(const char * path)
{
	FILE *f = fopen(path, "r+");	// Open for both reading and writing, If the file does not exist, fopen() returns NULL.
	if (f == NULL) {
		printf("No such files : %s\n", path);
		return;
	}
	while (true) {
		char lineHeader[128]; // Expected max length of 1 line = 128bytes;
		int line = fscanf(f, "%s", lineHeader);
		if (line == EOF) break;
		if (strcmp(lineHeader, "v") == 0) {
			Vec3f vertices;
			fscanf(f, "%f %f %f\n", &vertices.x, &vertices.y, &vertices.z);
			this->vertices.push_back(vertices);
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			Vec2f textures;
			fscanf(f, "%f %f\n", &textures.x, &textures.y);
			this->textures.push_back(textures);
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			Vec3f normal;
			fscanf(f, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			this->normal.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(f, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9) {
				printf("File can't be read by our simple parser, try exporting with other options\n");
				break;
			}
			vertexIdx.push_back(vertexIndex[0]);
			vertexIdx.push_back(vertexIndex[1]);
			vertexIdx.push_back(vertexIndex[2]);
			textureIdx.push_back(uvIndex[0]);
			textureIdx.push_back(uvIndex[1]);
			textureIdx.push_back(uvIndex[2]);
			normalIdx.push_back(normalIndex[0]);
			normalIdx.push_back(normalIndex[1]);
			normalIdx.push_back(normalIndex[2]);
		}
		else if (strcmp(lineHeader, "o") == 0) {
			char buf[128];
			fscanf(f, "%s", buf);
			std::string tmp_name = std::string(buf);
			this->obj_name = tmp_name;
		}
		else continue;
	}
	fclose(f);

}
inline ObjParser::ObjParser(const char * path,int partNum,int blkNum)
{
	int tmaxx=0, tmaxy=0, tmaxz=0, tminx=150, tminy=150, tminz=150;
	FILE *f = fopen(path, "r+");	// Open for both reading and writing, If the file does not exist, fopen() returns NULL.
	if (f == NULL) {
		printf("No such files : %s\n", path);
		return;
	}
	while (true) {
		char lineHeader[128]; // Expected max length of 1 line = 128bytes;
		int line = fscanf(f, "%s", lineHeader);
		if (line == EOF) break;
		if (strcmp(lineHeader, "v") == 0) {
			Vec3f vertices;
			fscanf(f, "%f %f %f\n", &vertices.x, &vertices.y, &vertices.z);
			if (vertices.x < tminx) tminx = vertices.x;
			if (vertices.y < tminy) tminy = vertices.y;
			if (vertices.z < tminz) tminz = vertices.z;
			if (vertices.x > tmaxx) tmaxx = vertices.x;
			if (vertices.y > tmaxy) tmaxy = vertices.y;
			if (vertices.z > tmaxz) tmaxz = vertices.z;
			this->vertices.push_back(vertices);
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			Vec2f textures;
			fscanf(f, "%f %f\n", &textures.x, &textures.y);
			this->textures.push_back(textures);
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			Vec3f normal;
			fscanf(f, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			this->normal.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(f, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9) {
				printf("File can't be read by our simple parser, try exporting with other options\n");
				break;
			}
			vertexIdx.push_back(vertexIndex[0]);
			vertexIdx.push_back(vertexIndex[1]);
			vertexIdx.push_back(vertexIndex[2]);
			textureIdx.push_back(uvIndex[0]);
			textureIdx.push_back(uvIndex[1]);
			textureIdx.push_back(uvIndex[2]);
			normalIdx.push_back(normalIndex[0]);
			normalIdx.push_back(normalIndex[1]);
			normalIdx.push_back(normalIndex[2]);
		}
		else if (strcmp(lineHeader, "o") == 0) {
			char buf[128];
			fscanf(f, "%s", buf);
			std::string tmp_name = std::string(buf);
			this->obj_name = tmp_name;
		}
		else continue;
	}
	switch (partNum) {
	case 0:
		abhead[blkNum].minx = tminx;
		abhead[blkNum].miny = tminy;
		abhead[blkNum].minz = tminz;
		abhead[blkNum].maxx = tmaxx;
		abhead[blkNum].maxy = tmaxy;
		abhead[blkNum].maxz = tmaxz;
		break;
	case 1:
		abbody[blkNum].minx = tminx;
		abbody[blkNum].miny = tminy;
		abbody[blkNum].minz = tminz;
		abbody[blkNum].maxx = tmaxx;
		abbody[blkNum].maxy = tmaxy-12;
		abbody[blkNum].maxz = tmaxz;
		break;
	case 2:
		abarm[blkNum].minx = tminx;
		abarm[blkNum].miny = tminy;
		abarm[blkNum].minz = tminz;
		abarm[blkNum].maxx = tmaxx;
		abarm[blkNum].maxy = tmaxy;
		abarm[blkNum].maxz = tmaxz;
		break;
	case 3:
		ableg[blkNum].minx = tminx;
		ableg[blkNum].miny = tminy;
		ableg[blkNum].minz = tminz;
		ableg[blkNum].maxx = tmaxx;
		ableg[blkNum].maxy = tmaxy-12;
		ableg[blkNum].maxz = tmaxz;
		break;
	}
	fclose(f);

}


inline void ObjParser::read(const char * path)
{
	FILE *f = fopen(path, "r+");	// Open for both reading and writing, If the file does not exist, fopen() returns NULL.
	if (f == NULL) {
		printf("No such files : %s\n", path);
		return;
	}

	this->clear();

	while (true) {
		char lineHeader[128]; // Expected max length of 1 line = 128bytes;
		int line = fscanf(f, "%s", lineHeader);
		if (line == EOF) break;
		if (strcmp(lineHeader, "v") == 0) {
			Vec3f vertices;
			fscanf(f, "%f %f %f\n", &vertices.x, &vertices.y, &vertices.z);
		
			this->vertices.push_back(vertices);
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			Vec2f textures;
			fscanf(f, "%f %f\n", &textures.x, &textures.y);
			this->textures.push_back(textures);
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			Vec3f normal;
			fscanf(f, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			this->normal.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(f, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9) {
				printf("File can't be read by our simple parser, try exporting with other options\n");
				return;
			}
			vertexIdx.push_back(vertexIndex[0]);
			vertexIdx.push_back(vertexIndex[1]);
			vertexIdx.push_back(vertexIndex[2]);
			textureIdx.push_back(uvIndex[0]);
			textureIdx.push_back(uvIndex[1]);
			textureIdx.push_back(uvIndex[2]);
			normalIdx.push_back(normalIndex[0]);
			normalIdx.push_back(normalIndex[1]);
			normalIdx.push_back(normalIndex[2]);
		}
		else if (strcmp(lineHeader, "o") == 0) {
			char buf[128];
			fscanf(f, "%s", buf);
			std::string tmp_name = std::string(buf);
			this->obj_name = tmp_name;
		}
		else continue;	//# or s or usemtl
	}
	fclose(f);
}

inline void ObjParser::write(const char * path)
{
	std::string tmp(path);
	if (tmp.substr(tmp.size() - 4, 4).compare(".obj") != 0) {
		printf("File format is invalid, you must use obj file\n");
		return;
	}

	FILE *f = fopen(path, "w+");	// Open for both reading and writing, If the file does not exist, fopen makes empty file.
									// If the file exists, its contents are overwritten

									// write obj name
	fprintf(f, "o %s\n", tmp.substr(0, tmp.size() - 4));

	// write vertices
	for (int i = 0; i < this->vertices.size(); i++) {
		fprintf(f, "v %f %f %f\n", this->vertices[i].x, this->vertices[i].y, vertices[i].z);
	}
	// write UVs
	for (int i = 0; i < this->textures.size(); i++) {
		fprintf(f, "vt %f %f\n", this->textures[i].x, this->textures[i].y);
	}
	// write normals
	for (int i = 0; i < this->normal.size(); i++) {
		fprintf(f, "vn %f %f %f\n", this->normal[i].x, this->normal[i].y, normal[i].z);
	}
	// write faces
	for (int i = 0; i < this->vertexIdx.size() / 3; i++) {
		fprintf(f, "f %d/%d/%d %d/%d/%d %d/%d/%d\n",
			vertexIdx[3 * i + 0], textureIdx[3 * i + 0], normalIdx[3 * i + 0],
			vertexIdx[3 * i + 1], textureIdx[3 * i + 1], normalIdx[3 * i + 1],
			vertexIdx[3 * i + 2], textureIdx[3 * i + 2], normalIdx[3 * i + 2]);
	}

	fclose(f);
}

inline void ObjParser::clear()
{
	if (!this->vertices.empty()) this->vertices.clear();
	if (!this->textures.empty()) this->textures.clear();
	if (!this->normal.empty()) this->normal.clear();
	if (!this->vertexIdx.empty()) this->vertexIdx.clear();
	if (!this->textureIdx.empty()) this->textureIdx.clear();
	if (!this->normalIdx.empty()) this->normalIdx.clear();
}

inline unsigned int ObjParser::getFaceSize()
{
	return this->vertexIdx.size();
}