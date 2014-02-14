#include "Util.h"

void makeSphere(GLfloat* vVerts, GLfloat* vNormals, GLfloat* vTex, GLfloat fRadius, int iSlice, int iStack){
	GLfloat mSlice = 2 * pi / iSlice;
	GLfloat mStack = pi / iStack;

	M3DVector3f vertex[4];
	M3DVector3f normals[4];
	M3DVector2f texture[4];
	GLfloat ds = 1.0 / (GLfloat)iSlice;
	GLfloat dt = 1.0 / (GLfloat)iStack;
	int vertex_num = 0;
	int normals_num = 0;
	int tex_num = 0;
	GLfloat s = 0;
	GLfloat t = 0;
	for (int i = 0; i < iSlice; i++){
		t = 0;
		for (int j = 0; j < iStack; j++){
			vertex[0][0] = fRadius*sin(mStack*j)*cos(mSlice*i);
			vertex[0][1] = fRadius*sin(mStack*j)*sin(mSlice*i);
			vertex[0][2] = fRadius*cos(mStack*j);

			normals[0][0] = vertex[0][0] / fRadius;
			normals[0][1] = vertex[0][1] / fRadius;
			normals[0][2] = vertex[0][2] / fRadius;

			texture[0][0] = s;
			texture[0][1] = t;


			vertex[1][0] = fRadius*sin(mStack*(j + 1))*cos(mSlice*i);
			vertex[1][1] = fRadius*sin(mStack*(j + 1))*sin(mSlice*i);
			vertex[1][2] = fRadius*cos(mStack*(j + 1));

			normals[1][0] = vertex[1][0] / fRadius;
			normals[1][1] = vertex[1][1] / fRadius;
			normals[1][2] = vertex[1][2] / fRadius;

			texture[1][0] = s;
			texture[1][1] = t + dt;

			vertex[2][0] = fRadius*sin(mStack*j)*cos(mSlice*(i + 1));
			vertex[2][1] = fRadius*sin(mStack*j)*sin(mSlice*(i + 1));
			vertex[2][2] = fRadius*cos(mStack*j);

			normals[2][0] = vertex[2][0] / fRadius;
			normals[2][1] = vertex[2][1] / fRadius;
			normals[2][2] = vertex[2][2] / fRadius;

			texture[2][0] = s + ds;
			texture[2][1] = t;

			vertex[3][0] = fRadius*sin(mStack*(j + 1))*cos(mSlice*(i + 1));
			vertex[3][1] = fRadius*sin(mStack*(j + 1))*sin(mSlice*(i + 1));
			vertex[3][2] = fRadius*cos(mStack*(j + 1));

			normals[3][0] = vertex[3][0] / fRadius;
			normals[3][1] = vertex[3][1] / fRadius;
			normals[3][2] = vertex[3][2] / fRadius;

			texture[3][0] = s + ds;
			texture[3][1] = t + dt;

			t = t + dt;

			for (int m = 0; m < 3; m++){
				for (int n = 0; n < 3; n++){
					*(vVerts + vertex_num++) = vertex[m][n];
					*(vNormals + normals_num++) = normals[m][n];
					if (n < 2){
						*(vTex + tex_num++) = texture[m][n];
					}
				}

			}
			memcpy(vertex[0], vertex[1], sizeof(M3DVector3f));
			memcpy(normals[0], normals[1], sizeof(M3DVector3f));
			memcpy(texture[0], texture[1], sizeof(M3DVector2f));

			memcpy(vertex[1], vertex[3], sizeof(M3DVector3f));
			memcpy(normals[1], normals[3], sizeof(M3DVector3f));
			memcpy(texture[1], texture[3], sizeof(M3DVector2f));
			for (int m = 0; m < 3; m++){
				for (int n = 0; n < 3; n++){
					*(vVerts + vertex_num++) = vertex[m][n];
					*(vNormals + normals_num++) = normals[m][n];
					if (n < 2){
						*(vTex + tex_num++) = texture[m][n];
					}
				}

			}
		}
		s = s + ds;
	}
}