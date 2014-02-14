
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


GLFrame				 viewFrame;
GLFrustum			 frustum;
GLMatrixStack        modelviewMatrix;
GLMatrixStack        projectionMatrix;
GLGeometryTransform  transformPipeline;


GLuint				 eclipse_program;
GLuint				 sphere_program;
GLuint				 mvpMatrix;
GLuint				 colors;
GLuint				 sphere_mvpMatrix;
GLuint				 sphere_colors;

const int eclipse_iSlice = 100;
const int eclipse_point_num = eclipse_iSlice * 2;
GLfloat	 eclipse_vVerts[eclipse_point_num * 3];
GLfloat	 eclipse_vNormal[eclipse_point_num * 3];
GLfloat	 eclipse_vTexture[eclipse_point_num * 2];

const int sphere_iSlice = 50;
const int sphere_iStack = 50;
const int sphere_point_num = sphere_iSlice*sphere_iStack * 2 * 3;
GLfloat   sphere_vVerts[sphere_point_num*3];
GLfloat   sphere_vNormal[sphere_point_num*3];
GLfloat   sphere_vTexture[sphere_point_num * 2];

Eclipse eclipse;
LoadShader eclipse_shader;
LoadShader sphere_shader;

const GLfloat dS = pi*1.5*0.8/360.0;
GLfloat  ganglar = 0;
GLfloat position[] = {
	1.5, 0.0, 0.0
};
int a = 1;
GLVertexBuffer eclipse_buffer;
GLVertexBuffer sphere_buffer;
std::map<GLuint, const GLchar*> eclipse_map;
std::map<GLuint, const GLchar*> sphere_map;
void SetupRC(){
	glClearColor(0.0f, 0.5f, 0.0f, 1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_DEPTH_TEST);
	viewFrame.MoveForward(4.0f);

	
	//绘制椭圆
	eclipse.makeEclipse(eclipse_vVerts, NULL, NULL, 1.5, 0.8, eclipse_iSlice);
	eclipse_buffer.createBuffer(eclipse_vVerts, NULL, NULL, eclipse_point_num);
	eclipse_shader.loadShaderPair("Identity.vp", "Identity.fp");
	
	eclipse_map[GL_VERTEX_ATTRIBUTE] = "vVertex";
	eclipse_program = eclipse_shader.createShader(eclipse_map);
	
	glUseProgram(eclipse_program);
	mvpMatrix = glGetUniformLocation(eclipse_program, "mvpMatrix");
	colors = glGetUniformLocation(eclipse_program, "color");
	

	//绘制椭球体
	
	makeSphere(sphere_vVerts, sphere_vNormal, sphere_vTexture, 0.3, sphere_iSlice, sphere_iStack);
	sphere_buffer.createBuffer(sphere_vVerts, sphere_vNormal, sphere_vTexture, sphere_point_num);
	
	//创建着色器
	sphere_shader.loadShaderPair("RotateEllipsoid.vp", "RotateEllipsoid.fp");
	sphere_map[GL_VERTEX_ATTRIBUTE] = "vVertex";
	sphere_program = sphere_shader.createShader(sphere_map);
	glUseProgram(sphere_program);
	sphere_mvpMatrix = glGetUniformLocation(sphere_program, "mvpMatrix");
	sphere_colors = glGetUniformLocation(sphere_program, "color");

}
void RenderScene(){
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glUseProgram(eclipse_program);
	GLfloat color[] = {
		1.0f, 1.0f, 0.4f, 0.0
	};
	GLfloat sphere_color[] = {
		0.0f, 0.4f, 0.6f, 0.0f
	};
	modelviewMatrix.PushMatrix(viewFrame);
	glUniformMatrix4fv(mvpMatrix,1,GL_FALSE,transformPipeline.GetModelViewProjectionMatrix());
	glUniform4fv(colors, 1, color);
	//glBindVertexArray(eclipse_vertexArrayObject);
	eclipse_buffer.drawBuffer(GL_LINES);
	modelviewMatrix.PopMatrix();


	glUseProgram(sphere_program);
	static CStopWatch rotTimer;
	float second=rotTimer.GetElapsedSeconds();
	M3DMatrix44f mTranslate,mRotate, mModelView,mModelViewProjection;
	
	GLfloat d = sqrt(pow(position[0], 2) + pow(position[1], 2));
	GLfloat anglar = std::asinf(dS /d);
	ganglar += anglar;
	position[0] = 1.5*cos(ganglar);
	position[1] = 0.8*sin(ganglar);
	m3dRotationMatrix44(mRotate, m3dDegToRad(second), position[0], position[1], 0.0f);
	m3dTranslationMatrix44(mTranslate, position[0],position[1],position[2]);
	modelviewMatrix.PushMatrix(viewFrame);
	m3dMatrixMultiply44(mModelView, mTranslate, mRotate);
	m3dMatrixMultiply44(mModelViewProjection, transformPipeline.GetModelViewProjectionMatrix(), mModelView);
	glUniformMatrix4fv(sphere_mvpMatrix, 1, GL_FALSE, mModelViewProjection);
	glUniform4fv(sphere_colors,1, sphere_color);
	sphere_buffer.drawBuffer(GL_TRIANGLES);
	modelviewMatrix.PopMatrix();
	glutSwapBuffers();
}
void ChangeSize(int w, int h){
	glViewport(0, 0, w, h);
	frustum.SetPerspective(35.0f, float(w) / float(h), 1.0f, 1000.0f);
	projectionMatrix.LoadMatrix(frustum.GetProjectionMatrix());
	transformPipeline.SetMatrixStacks(modelviewMatrix, projectionMatrix);
}
void SpecialKeys(int key, int x, int y){
	float angular = float(m3dDegToRad(5.0f));

	if (key == GLUT_KEY_UP){
		viewFrame.RotateWorld(m3dDegToRad(-5.0), 1.0f, 0.0f, 0.0f);

	}
	if (key == GLUT_KEY_DOWN){
		viewFrame.RotateWorld(m3dDegToRad(5.0), 1.0f, 0.0f, 0.0f);

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