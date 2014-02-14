#include "Eclipse.h"


Eclipse::Eclipse(){}
void Eclipse::makeEclipse(GLfloat* vVertexs, GLfloat* vNormals, GLfloat* vTextures,
	GLfloat a, GLfloat b, int iSlice){
	GLfloat x;
	GLfloat y;
	GLfloat mSlice = (2.0 * pi)/(GLfloat)iSlice;
	GLfloat vVerts[2][3];
	GLfloat vNormal[2][3];
	GLfloat vTex[2][2];
	int vertex_num = 0;
	for (int i = 0; i < iSlice; i++){
		
		x = a*cos(i*mSlice);
		y = b*sin(i*mSlice);

		vVerts[0][0] = x;
		vVerts[0][1] = y;
		vVerts[0][2] = 0;
		
		//ÎªÁË±ÜÃâÍÖÔ²ÓÐ·ìÏ¶
		if ((i+1) == iSlice){
			x = a*cos(0);
			y = b*sin(0);
		}
		else{
			x = a*cos((i+1)*mSlice);
			y = b*sin((i+1)*mSlice);
		}
		vVerts[1][0] = x;
		vVerts[1][1] = y;
		vVerts[1][2] = 0;
		
		for (int m = 0; m < 2; m++){
			for (int n = 0; n < 3; n++){
				*(vVertexs + vertex_num++) = vVerts[m][n];
			}
		}	
	}
}
int Eclipse::getCout(){
	return Eclipse::pointCount;
}
Eclipse::~Eclipse(){}
