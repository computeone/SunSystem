#pragma once
#include <GLTools.h>
#include <iostream>
using namespace std;

#define pi 3.141592653589f
#define Water      0
#define Gold       1
#define Earth      2
#define Fires      3
#define Tree       4
#define Soil       5
#define SkyKing    6
#define SeaKing    7
#define EclipseSlice       100
#define EllipsoidSlice     100
#define EllipsoidStack     100
#define AU                 92955807.0f
#define RadiusZoom         0.0001f
#define SunRadiusZoom      0.00001f
#define DistanceZoom       0.0000005f
#define SunRadius          1392000.0f
#define SunRoatePeriod     25.5f
#define SunRotateAnglar    7.25f
#define SunRotateDirection 1.0f
//直径
const float Radius[] = {
	4878.0f,12103.0f,12756.0f,
	6786.0f,142984.0f,120536.0f,
	51118.0f,49528.0f
};
//公转周期，单位天
const float RevolutionPeriod[] = {
	87.97f, 224.70f, 365.26f,
	686.98f, 11.86f*365.0f, 29.46f*365.0f,
	84.01f*365.0f, 164.79f*365.0f
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
//每天扫过的面积
const  GLfloat   dS[8] = {
	pi*LAxis[Water] * AU *DistanceZoom * SAxis[Water] *AU*DistanceZoom / RevolutionPeriod[Water],
	pi*LAxis[Gold]  * AU *DistanceZoom * SAxis[Gold]  *AU*DistanceZoom / RevolutionPeriod[Gold],
	pi*LAxis[Earth] * AU *DistanceZoom * SAxis[Earth] *AU*DistanceZoom / RevolutionPeriod[Earth],
	pi*LAxis[Fires] * AU *DistanceZoom * SAxis[Fires] *AU*DistanceZoom / RevolutionPeriod[Fires],
	pi*LAxis[Tree]  * AU *DistanceZoom * SAxis[Tree]  *AU*DistanceZoom / RevolutionPeriod[Tree],
	pi*LAxis[Soil]  * AU *DistanceZoom * SAxis[Soil]  *AU*DistanceZoom / RevolutionPeriod[Soil],
	pi*LAxis[SkyKing] *AU*DistanceZoom * SAxis[SkyKing]*AU*DistanceZoom/ RevolutionPeriod[SkyKing],
	pi*LAxis[SeaKing] *AU*DistanceZoom * SAxis[SeaKing]*AU*DistanceZoom/ RevolutionPeriod[SeaKing]
};

//自转轴方向，自西向东为1.0，自东向西为-1.0
const GLfloat  RotateDirection[]{
	1.0f,-1.0f,1.0f,
	1.0f,1.0f,1.0f,
	-1.0f,1.0f
};
void makeSphere(GLfloat* vVerts, GLfloat* vNormals, GLfloat* vTex, GLfloat fRadius, int iSlice, int iStack); 