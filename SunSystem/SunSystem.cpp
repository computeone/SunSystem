
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

GLuint				 eclipseProgram;
GLuint               eclipseMvpMatrix;
GLuint				 uniformColors;

Eclipse              eclipse[8];
GLVertexBuffer       eclipseBuffer[8];
LoadShader           eclipseShader;
GLfloat				 eclipseVertexs[8][EclipseSlice * 6];


//const int eclipse_iSlice = 1000;
//const int eclipse_point_num = eclipse_iSlice * 2;
//GLfloat	 eclipse_vVerts[eclipse_point_num * 3];
//GLfloat	 eclipse_vNormal[eclipse_point_num * 3];
//GLfloat	 eclipse_vTexture[eclipse_point_num * 2];
//
//const int sphere_iSlice = 50;
//const int sphere_iStack = 50;
//const int sphere_point_num = sphere_iSlice*sphere_iStack * 2 * 3;
//GLfloat   sphere_vVerts[sphere_point_num*3];
//GLfloat   sphere_vNormal[sphere_point_num*3];
//GLfloat   sphere_vTexture[sphere_point_num * 2];
//
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

	viewFrame.MoveForward(4.0f);


	//绘制椭圆
	for (int i = 0; i < 8; i++){
		eclipse[i].makeEclipse(eclipseVertexs[i], NULL, NULL, LAxis[i], SAxis[i], EclipseSlice);
		eclipseBuffer[i].createBuffer(eclipseVertexs[i], NULL, NULL, EclipseSlice * 2);
	}

	//加载绘制椭圆的着色器
	eclipseShader.loadShaderPair("Identity.vp", "Identity.fp");
	std::map<GLuint, const char*> eclipseMap;
	eclipseMap[GL_VERTEX_ATTRIBUTE] = "vVertex";
	eclipseProgram = eclipseShader.createShader(eclipseMap);
	
	glUseProgram(eclipseProgram);

	eclipseMvpMatrix = glGetUniformLocation(eclipseProgram,"mvpMatrix");
	uniformColors = glGetUniformLocation(eclipseProgram, "color");

	////绘制椭球体
	//
	//makeSphere(sphere_vVerts, sphere_vNormal, sphere_vTexture, 0.3, sphere_iSlice, sphere_iStack);
	//sphere_buffer.createBuffer(sphere_vVerts, sphere_vNormal, sphere_vTexture, sphere_point_num);
	//
	////创建着色器
	//sphere_shader.loadShaderPair("RotateEllipsoid.vp", "RotateEllipsoid.fp");
	//sphere_map[GL_VERTEX_ATTRIBUTE] = "vVertex";
	//sphere_map[GL_NORMAL_ATTRIBUTE] = "vNormal";
	//sphere_map[GL_TEXTURE_ATTRIBUTE] = "vTexture";
	//sphere_program = sphere_shader.createShader(sphere_map);
	//glUseProgram(sphere_program);
	//sphere_mvpMatrix = glGetUniformLocation(sphere_program, "mvpMatrix");
	//sphere_colors = glGetUniformLocation(sphere_program, "color");

}
void RenderScene(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	
	modelviewMatrix.PushMatrix(viewFrame);

	glUseProgram(eclipseProgram);
	GLfloat  Color[] = {
		0.6f,0.3f,1.0f,1.0f
	};
	
	M3DMatrix44f mModelViewProjection;
	glUniform4fv(uniformColors, 1, Color);
	for (int i = 0; i < 8; i++){
		M3DMatrix44f mRotate;
		m3dRotationMatrix44(mRotate, m3dDegToRad(OrbitAngle[i]), 1.0, 0.0, 0.0);
		m3dMatrixMultiply44(mModelViewProjection, transformPipeline.GetModelViewProjectionMatrix(),mRotate);
		glUniformMatrix4fv(eclipseMvpMatrix, 1, GL_FALSE, mModelViewProjection);
		eclipseBuffer[i].drawBuffer(GL_LINES);
	}
	modelviewMatrix.PopMatrix();

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