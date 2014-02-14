#pragma once
#include <GLTools.h>
#include <iostream>
using namespace std;

#define pi 3.141592653589f
#define Water      0
#define Gold       1
#define Earth      2
#define Mars       3
#define Jupiter    4
#define Soil       5
#define SkyKing    6
#define SeaKing    7
#define EclipseSlice    100

//直径
const float Radius[] = {
	4878.0f,12103.0f,12756.0f,
	6786.0f,142984.0f,120536.0f,
	51118.0f,49528.0f
};
//公转周期，单位天
const float RevolutionPeriod[] = {
	87.97f, 224.70f, 365.26f,
	686.98f, 11.86f, 29.46f,
	84.01f, 164.79f
};
//自转周期，单位天
const float RotatePeriod[] = {
	58.65f, 243.0f, 0.9972f,
	1.026f,0.410f,0.4438,
	0.7181f,0.6715f
};
//短轴距离
const float SAxis[] = {
	0.3075f,0.7205f,0.9833f,
	1.3810f,4.9504f,9.0481f,
	18.3755f,29.7661f
};

//长轴距离
const float LAxis[] = {
	0.4612f,0.7255f,1.0167f,
	1.6660f,5.4581f,10.1160f,
	20.0833f,30.4413f
};

//公转倾斜角
const float OrbitAngle[] = {
	7.0048f,3.9347f,0.0000f,
	1.8506f,1.3053f,2.4846f,
	0.7699f,1.7692f
};

//自传倾斜角
const float RotateAngle[] = {
	0.0000f,177.3300f,23.4500f,
	25.1900f,3.0800f,26.7300f,
	97.9200f,28.8000f
};

void makeSphere(GLfloat* vVerts, GLfloat* vNormals, GLfloat* vTex, GLfloat fRadius, int iSlice, int iStack); 
	