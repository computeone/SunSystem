#include "Ellipsoid.h"
using namespace std;

Ellipsoid::Ellipsoid(){}

void Ellipsoid::makeEllipsoid(GLfloat* vVertexs, GLfloat* vNormals, GLfloat* vTextures,
	GLfloat fa, GLfloat fb, GLfloat fc, int iSlice, int iStack){

		GLfloat mStack = pi / iStack;
		GLfloat mSlice = 2 * pi / iSlice;

		GLfloat  x;
		GLfloat  y;
		GLfloat  z;
		M3DVector3f vertex[4];
		M3DVector3f normal[4];
		M3DVector2f texture[4];

		int vertex_num = 0;
		int normal_num = 0;
		int texture_num = 0;

		GLfloat ds = 1.0 / (GLfloat)iSlice;
		GLfloat dt = 1.0 / (GLfloat)iStack;

		GLfloat s = 0;
		GLfloat t = 0;

		for (int i = 0; i < iSlice; i++){
			t = 0;
			for (int j = 0; j < iStack; j++){


				x = fa*cos(j*mStack - pi / 2)*cos(i*mSlice - pi);
				y = fb*cos(j*mStack - pi / 2)*sin(i*mSlice - pi);
				z = fc*sin(j*mStack - pi / 2);

				vertex[0][0] = x;
				vertex[0][1] = y;
				vertex[0][2] = z;

				normal[0][0] = 1.0 / pow(fa, 2)*x;
				normal[0][1] = 1.0 / pow(fb, 2)*y;
				normal[0][2] = 1.0 / pow(fc, 2)*z;

				texture[0][0] = s;
				texture[0][1] = t;

				x = fa*cos((j + 1)*mStack - pi / 2)*cos(i*mSlice - pi);
				y = fb*cos((j + 1)*mStack - pi / 2)*sin(i*mSlice - pi);
				z = fc*sin((j + 1)*mStack - pi / 2);

				vertex[1][0] = x;
				vertex[1][1] = y;
				vertex[1][2] = z;


				normal[1][0] = 1.0 / pow(fa, 2)*x;
				normal[1][1] = 1.0 / pow(fb, 2)*y;
				normal[1][2] = 1.0 / pow(fc, 2)*z;

				texture[1][0] = s;
				texture[1][1] = t + dt;

				x = fa*cos(j*mStack - pi / 2)*cos((i + 1)*mSlice - pi);
				y = fb*cos(j*mStack - pi / 2)*sin((i + 1)*mSlice - pi);
				z = fc*sin(j*mStack - pi / 2);

				vertex[2][0] = x;
				vertex[2][1] = y;
				vertex[2][2] = z;


				normal[2][0] = 1.0 / pow(fa, 2)*x;
				normal[2][1] = 1.0 / pow(fb, 2)*y;
				normal[2][2] = 1.0 / pow(fc, 2)*z;

				texture[2][0] = s + ds;
				texture[2][1] = t;

				x = fa*cos((j + 1)*mStack - pi / 2)*cos((i + 1)*mSlice - pi);
				y = fb*cos((j + 1)*mStack - pi / 2)*sin((i + 1)*mSlice - pi);
				z = fc*sin((j + 1)*mStack - pi / 2);

				vertex[3][0] = x;
				vertex[3][1] = y;
				vertex[3][2] = z;


				normal[3][0] = 1.0 / pow(fa, 2)*x;
				normal[3][1] = 1.0 / pow(fb, 2)*y;
				normal[3][2] = 1.0 / pow(fc, 2)*z;

				texture[3][0] = s + ds;
				texture[3][1] = t + dt;

				t = t + dt;

				for (int m = 0; m < 3; m++){
					for (int n = 0; n < 3; n++){
						*(vVertexs + vertex_num++) = vertex[m][n];
						*(vNormals + normal_num++) = normal[m][n];
						if (n < 2){
							*(vTextures + texture_num++) = texture[m][n];
						}
					}

				}


				memcpy(vertex[0], vertex[1], sizeof(M3DVector3f));
				memcpy(normal[0], normal[1], sizeof(M3DVector3f));
				memcpy(texture[0], texture[1], sizeof(M3DVector2f));

				memcpy(vertex[1], vertex[3], sizeof(M3DVector3f));
				memcpy(normal[1], normal[3], sizeof(M3DVector3f));
				memcpy(texture[1], texture[3], sizeof(M3DVector2f));
				for (int m = 0; m < 3; m++){
					for (int n = 0; n < 3; n++){
						*(vVertexs + vertex_num++) = vertex[m][n];
						*(vNormals + normal_num++) = normal[m][n];
						if (n < 2){
							*(vTextures + texture_num++) = texture[m][n];
						}
					}

				}
			}
			s = s + ds;
	}
}

Ellipsoid::~Ellipsoid(){}