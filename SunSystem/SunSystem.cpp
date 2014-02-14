
#include "Eclipse.h"
#include "Ellipsoid.h"
#include "LoadShader.h"
#include "GLVertexBuffer.h"
#include "Util.h"
#include <GL\freeglut.h>
#include <algorithm>
#include <math3d.h>
#include <StopWatch.h>
#include <GLFrame.h>
#include <GLFrustum.h>
#include <GLMatrixStack.h>
#include <GLGeometryTransform.h>
using namespace std;

GLFrame				 viewFrame;
GLFrustum			 frustum;
GLMatrixStack        modelviewMatrix;
GLMatrixStack        projectionMatrix;
GLGeometryTransform  transformPipeline;

GLuint				 eclipseProgram;
GLuint               ellipsoidProgram;
GLuint               eclipseMvpMatrix;
GLuint               ellipsoidMvpMatrix;
GLuint				 uniformColors;
GLuint				 ellipsoidCubeTextures[1];

Eclipse              eclipse[8];
GLVertexBuffer       eclipseBuffer[8];
LoadShader           eclipseShader;
GLfloat				 eclipseVertexs[8][EclipseSlice * 6];

GLVertexBuffer       ellipsoidBuffer[8];
LoadShader           ellipsoidShader;
GLfloat              ellipsoidVertexs[8][EllipsoidSlice*EllipsoidStack * 2 * 3 * 3];
GLfloat              ellipsoidNormals[8][EllipsoidSlice*EllipsoidStack * 2 * 3 * 3];
GLfloat				 ellipsoidTextures[8][EllipsoidSlice*EllipsoidStack * 2 * 3 * 2];


//Eclipse eclipse;
//LoadShader eclipse_shader;
//LoadShader sphere_shader;
//
//const GLfloat dS = pi*1.5*0.8/360.0;
//GLfloat  ganglar = 0;
//GLfloat position[] = {
//	1.5, 0.0, 0.0
//};
//int a = 1;
//GLVertexBuffer eclipse_buffer;
//GLVertexBuffer sphere_buffer;
//std::map<GLuint, const GLchar*> eclipse_map;
//std::map<GLuint, const GLchar*> sphere_map;
void SetupRC(){
	//glClearColor(0.0f, 0.5f, 0.0f, 1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_POINT_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_POLYGON_SMOOTH);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);

	viewFrame.MoveForward(4.0f);


	//加载椭球体天体纹理
	for (int i = 0; i < 1; i++){
		GLbyte *pBytes;
		GLint  iWidth, iHeight, iComponents;
		GLenum eFormat;
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);


		glGenTextures(1, &ellipsoidCubeTextures[0]);
		glBindTexture(GL_TEXTURE_2D, ellipsoidCubeTextures[0]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		pBytes = gltReadTGABits("earth.tga", &iWidth, &iHeight, &iComponents, &eFormat);
		glTexImage2D(GL_TEXTURE_2D, 0, iComponents, iWidth, iHeight, 0, eFormat, GL_UNSIGNED_BYTE,
			pBytes);
		free(pBytes);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	

	//绘制椭圆
	for (int i = 0; i < 8; i++){
		eclipse[i].makeEclipse(eclipseVertexs[i], NULL, NULL, LAxis[i], SAxis[i], EclipseSlice);
		eclipseBuffer[i].createBuffer(eclipseVertexs[i], NULL, NULL, EclipseSlice * 2);
	}

	//加载绘制椭圆的着色器
	eclipseShader.loadShaderPair("Identity.vp", "Identity.fp");
	map<GLuint, const char*> eclipseMap;
	eclipseMap[GL_VERTEX_ATTRIBUTE] = "vVertex";
	eclipseProgram = eclipseShader.createShader(eclipseMap);
	
	glUseProgram(eclipseProgram);

	eclipseMvpMatrix = glGetUniformLocation(eclipseProgram,"mvpMatrix");
	uniformColors = glGetUniformLocation(eclipseProgram, "color");

	//绘制椭球体
	for (int i = 0; i < 8; i++){
		makeSphere(ellipsoidVertexs[i], ellipsoidNormals[i], ellipsoidTextures[i],
			Radius[i]/0.0000001,EllipsoidSlice,EllipsoidStack);
		ellipsoidBuffer[i].createBuffer(ellipsoidVertexs[i], ellipsoidNormals[i],
			ellipsoidTextures[i], EllipsoidSlice*EllipsoidStack * 2 * 3);
	}


	//创建着色器
	ellipsoidShader.loadShaderPair("RotateEllisoid.vp", "RotateEllisoid.fp");
	map<GLuint, const char*> ellipsoidMap;
	ellipsoidMap[GL_VERTEX_ATTRIBUTE] = "vVertex";
	ellipsoidMap[GL_NORMAL_ATTRIBUTE] = "vNormal";
	ellipsoidMap[GL_TEXTURE_ATTRIBUTE] = "vTexture";
	ellipsoidProgram=ellipsoidShader.createShader(ellipsoidMap);
	
	ellipsoidMvpMatrix = glGetUniformLocation(ellipsoidProgram, "mvpMatrix");
	
}
void RenderScene(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	
	//渲染椭圆轨道
	modelviewMatrix.PushMatrix(viewFrame);

	glUseProgram(eclipseProgram);
	GLfloat  Color[] = {
		0.6f,0.3f,1.0f,1.0f
	};
	
	M3DMatrix44f mModelViewProjection;
	glUniform4fv(uniformColors, 1, Color);
	for (int i = 0; i < 8; i++){
		//旋转到轨道倾斜角
		M3DMatrix44f mRotate;
		m3dRotationMatrix44(mRotate, m3dDegToRad(OrbitAngle[i]), 1.0, 0.0, 0.0);
		m3dMatrixMultiply44(mModelViewProjection, transformPipeline.GetModelViewProjectionMatrix(),mRotate);
		glUniformMatrix4fv(eclipseMvpMatrix, 1, GL_FALSE, mModelViewProjection);
		eclipseBuffer[i].drawBuffer(GL_LINES);
	}
	modelviewMatrix.PopMatrix();

	// 渲染椭球体天体
	//modelviewMatrix.PushMatrix(viewFrame);
	//glUseProgram(ellipsoidProgram);


	glutSwapBuffers();
	
	/*glUseProgram(sphere_program);
	static CStopWatch rotTimer;
	float second=rotTimer.GetElapsedSeconds();
	M3DMatrix44f mTranslate,mRotate, mModelView,mModelViewProjection;
	
	GLfloat d = sqrt(pow(position[0], 2) + pow(position[1], 2));
	GLfloat anglar = std::asinf(dS /d);
	ganglar += anglar;
	position[0] = 1.5*cos(ganglar);
	position[1] = 0.8*sin(ganglar);
	m3dRotationMatrix44(mRotate, m3dDegToRad(10), position[0], position[1], 0.0f);
	m3dTranslationMatrix44(mTranslate, position[0],position[1],position[2]);
	modelviewMatrix.PushMatrix(viewFrame);
	m3dMatrixMultiply44(mModelView, mTranslate, mRotate);
	m3dMatrixMultiply44(mModelViewProjection, transformPipeline.GetModelViewProjectionMatrix(), mModelView);
	glUniformMatrix4fv(sphere_mvpMatrix, 1, GL_FALSE, mModelViewProjection);
	glUniform4fv(sphere_colors,1, sphere_color);
	sphere_buffer.drawBuffer(GL_TRIANGLES);
	modelviewMatrix.PopMatrix();
	glutSwapBuffers();*/
}
void ChangeSize(int w, int h){
	glViewport(0, 0, w, h);
	frustum.SetPerspective(35.0f, float(w) / float(h), 1.0f, 1000.0f);
	projectionMatrix.LoadMatrix(frustum.GetProjectionMatrix());
	transformPipeline.SetMatrixStacks(modelviewMatrix, projectionMatrix);
}
void SpecialKeys(int key, int x, int y){
	float angular = float(m3dDegToRad(1.0f));

	if (key == GLUT_KEY_UP){
		//viewFrame.RotateWorld(m3dDegToRad(-5.0), 1.0f, 0.0f, 0.0f);
		viewFrame.MoveForward(-0.1f);

	}
	if (key == GLUT_KEY_DOWN){
		//viewFrame.RotateWorld(m3dDegToRad(5.0), 1.0f, 0.0f, 0.0f);
		viewFrame.MoveForward(0.1f);
	}
	if (key == GLUT_KEY_LEFT){
		viewFrame.RotateWorld(angular, 0.0f, 1.0f, 0.0f);

	}
	if (key == GLUT_KEY_RIGHT){
		viewFrame.RotateWorld(-angular, 0.0f, 1.0f, 0.0f);

	}
	glutPostRedisplay();
}  
void timer(int value){
	cout << "timer:" << endl;
	glutTimerFunc(100, timer, 1);
	glutPostRedisplay();
}
int main(int argc, char* argv[]){
	gltSetWorkingDirectory(argv[0]);
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA
		| GLUT_DEPTH | GLUT_STENCIL);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Triangle");
	glutReshapeFunc(ChangeSize);
	glutDisplayFunc(RenderScene);
	glutSpecialFunc(SpecialKeys);
	glutTimerFunc(100, timer, 1);
	GLenum err = glewInit();
	if (GLEW_OK != err){
		fprintf(stderr, "GLEW Error:%s\n", glewGetErrorString(err));
	}
	SetupRC();
	glutMainLoop();
	
} 