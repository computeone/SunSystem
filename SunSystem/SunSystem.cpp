
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
GLuint				 ellipsoidCubeTextures[9];

Eclipse              eclipse[8];
GLVertexBuffer       eclipseBuffer[8];
LoadShader           eclipseShader;
GLfloat				 eclipseVertexs[8][EclipseSlice * 6];

GLVertexBuffer       ellipsoidBuffer[8];
LoadShader           ellipsoidShader;
GLfloat              ellipsoidVertexs[8][EllipsoidSlice*EllipsoidStack * 2 * 3 * 3];
GLfloat              ellipsoidNormals[8][EllipsoidSlice*EllipsoidStack * 2 * 3 * 3];
GLfloat				 ellipsoidTextures[8][EllipsoidSlice*EllipsoidStack * 2 * 3 * 2];


GLVertexBuffer      SunEllipsoidBuffer;
GLfloat             SunEllipsoidVertexs[EllipsoidSlice*EllipsoidStack * 2 * 3 * 3];
GLfloat             SunEllipsoidNormals[EllipsoidSlice*EllipsoidStack * 2 * 3 * 3];
GLfloat             SunEllipsoidTextures[EllipsoidSlice*EllipsoidStack * 2 * 3 * 3];

const char* textures[9] = {
	"water.tga", "gold.tga", "earth.tga",
	"fire.tga", "tree.tga", "soil.tga",
	"skyking.tga", "seaking.tga", "sun.tga"
};

GLfloat  Currentposition[8][3];
GLfloat  CurrentAnglar[8];
GLfloat  CurrentRotateAxis[8][3];

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

	viewFrame.MoveForward(100.0f);

	//初始化天体参数
	for (int i = 0; i < 8; i++){
		Currentposition[i][0] = LAxis[i]*AU*DistanceZoom;
		Currentposition[i][1] = 0.0f;
		Currentposition[i][2] = 0.0f;

		CurrentRotateAxis[i][0] = 0.0f;
		CurrentRotateAxis[i][1] = 0.0f;
		CurrentRotateAxis[i][2] = RotateDirection[i];
	}


	//加载椭球体天体纹理
	for (int i = 0; i < 9; i++){
		GLbyte *pBytes;
		GLint  iWidth, iHeight, iComponents;
		GLenum eFormat;
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);


		glGenTextures(1, &ellipsoidCubeTextures[i]);
		glBindTexture(GL_TEXTURE_2D, ellipsoidCubeTextures[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		pBytes = gltReadTGABits(textures[i], &iWidth, &iHeight, &iComponents, &eFormat);
		glTexImage2D(GL_TEXTURE_2D, 0, iComponents, iWidth, iHeight, 0, eFormat, GL_UNSIGNED_BYTE,
			pBytes);
		free(pBytes);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	

	//绘制椭圆
	for (int i = 0; i < 8; i++){
		eclipse[i].makeEclipse(eclipseVertexs[i], NULL, NULL, LAxis[i] * AU * DistanceZoom, SAxis[i] * AU * DistanceZoom, EclipseSlice);
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
			Radius[i] * RadiusZoom, EllipsoidSlice, EllipsoidStack);
		ellipsoidBuffer[i].createBuffer(ellipsoidVertexs[i], ellipsoidNormals[i],
			ellipsoidTextures[i], EllipsoidSlice*EllipsoidStack * 2 * 3);
	}
	
	//绘制太阳
	makeSphere(SunEllipsoidVertexs, SunEllipsoidNormals, SunEllipsoidTextures,
		SunRadius*SunRadiusZoom, EllipsoidSlice,EllipsoidStack);
	SunEllipsoidBuffer.createBuffer(SunEllipsoidVertexs, SunEllipsoidNormals,
		SunEllipsoidTextures, EllipsoidSlice*EllipsoidStack * 2 * 3);


	//创建着色器
	ellipsoidShader.loadShaderPair("RotateEllipsoid.vp", "RotateEllipsoid.fp");
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
	
	M3DMatrix44f eclipseModelViewProjection;
	glUniform4fv(uniformColors, 1, Color);
	for (int i = 0; i < 8; i++){
		//旋转到轨道倾斜角
		M3DMatrix44f mRotate;
		m3dRotationMatrix44(mRotate, m3dDegToRad(OrbitAngle[i]), 1.0, 0.0, 0.0);
		m3dMatrixMultiply44(eclipseModelViewProjection, transformPipeline.GetModelViewProjectionMatrix(),mRotate);
		glUniformMatrix4fv(eclipseMvpMatrix, 1, GL_FALSE, eclipseModelViewProjection);
		eclipseBuffer[i].drawBuffer(GL_LINES);
	}

	modelviewMatrix.PopMatrix();

	// 渲染椭球体天体
	modelviewMatrix.PushMatrix(viewFrame);
	glUseProgram(ellipsoidProgram);


	
	M3DMatrix44f ellipsoidModelViewProjection;

	for (int i = 0; i < 8; i++){

		static CStopWatch rotTimer;
		float time = rotTimer.GetElapsedSeconds();
		
		M3DMatrix44f  mModelView,mTranslate,Rotate,RevolutionRoate,AnglarRoate;
		//计算天体的自转
		m3dRotationMatrix44(Rotate, m3dDegToRad(360.0 / (RotatePeriod[i])*time), 0.0, 0.0,
			RotateDirection[i]);
		//计算自转倾斜角旋转
		m3dRotationMatrix44(AnglarRoate, m3dDegToRad(RotateAngle[i]), 1.0, 0.0, 0.0);

		//计算当前天体位置到原点的距离
		GLfloat  d = sqrt(pow(Currentposition[i][0], 2) + pow(Currentposition[i][1], 2));

		//计算当前天体的角度
		GLfloat  anglar = asinf(2.0*dS[i] /pow(d,2));
		CurrentAnglar[i] += anglar;

		//计算天体当前位置
		Currentposition[i][0] = LAxis[i] *AU*DistanceZoom* cos(CurrentAnglar[i]);
		Currentposition[i][1] = SAxis[i] *AU*DistanceZoom* sin(CurrentAnglar[i]);

		m3dTranslationMatrix44(mTranslate, Currentposition[i][0], Currentposition[i][1], 0.0f);

		//轨道倾斜角旋转
		m3dRotationMatrix44(RevolutionRoate, m3dDegToRad(OrbitAngle[i]), 1.0f, 0.0f, 0.0f);

		//所有变换矩阵相乘
		M3DMatrix44f tempMatrix;
		m3dMatrixMultiply44(mModelView, AnglarRoate, Rotate);
		m3dMatrixMultiply44(tempMatrix, mTranslate,mModelView );
		m3dMatrixMultiply44(mModelView, RevolutionRoate, tempMatrix);
		m3dMatrixMultiply44(ellipsoidModelViewProjection ,transformPipeline.
			GetModelViewProjectionMatrix(), mModelView);
		glUniformMatrix4fv(ellipsoidMvpMatrix, 1, GL_FALSE, ellipsoidModelViewProjection);
		glBindTexture(GL_TEXTURE_2D, ellipsoidCubeTextures[i]);
		ellipsoidBuffer[i].drawBuffer(GL_TRIANGLES);
	}
	glUseProgram(ellipsoidProgram);

	static CStopWatch rotTimer;
	float time = rotTimer.GetElapsedSeconds();
	//绘制太阳
	M3DMatrix44f  mModelView, mModelViewProjection,Rotate, AnglarRoate;
	//计算天体的自转
	m3dRotationMatrix44(Rotate, m3dDegToRad(360.0 / (SunRoatePeriod)*time), 0.0, 0.0,
		SunRotateDirection);
	//计算自转倾斜角旋转
	m3dRotationMatrix44(AnglarRoate, m3dDegToRad(SunRotateAnglar), 1.0, 0.0, 0.0);


	m3dMatrixMultiply44(mModelView, AnglarRoate, Rotate);
	m3dMatrixMultiply44(ellipsoidModelViewProjection, transformPipeline.GetModelViewProjectionMatrix(),
		mModelView);
	
	glUniformMatrix4fv(ellipsoidMvpMatrix, 1, GL_FALSE, ellipsoidModelViewProjection);
	glBindTexture(GL_TEXTURE_2D, ellipsoidCubeTextures[8]);
	SunEllipsoidBuffer.drawBuffer(GL_TRIANGLES);

	modelviewMatrix.PopMatrix();
		
	glutSwapBuffers();
}
void ChangeSize(int w, int h){
	glViewport(0, 0, w, h);
	frustum.SetPerspective(35.0f, float(w) / float(h), 1.0f, 10000.0f);
	projectionMatrix.LoadMatrix(frustum.GetProjectionMatrix());
	transformPipeline.SetMatrixStacks(modelviewMatrix, projectionMatrix);
}
void SpecialKeys(int key, int x, int y){
	float angular = float(m3dDegToRad(1.0f));

	if (key == GLUT_KEY_UP){
		//viewFrame.RotateWorld(m3dDegToRad(-5.0), 1.0f, 0.0f, 0.0f);
		viewFrame.MoveForward(-1.0f);

	}
	if (key == GLUT_KEY_DOWN){
		//viewFrame.RotateWorld(m3dDegToRad(5.0), 1.0f, 0.0f, 0.0f);
		viewFrame.MoveForward(1.0f);
	}
	if (key == GLUT_KEY_LEFT){
		viewFrame.RotateLocal(angular, 0.0f, 1.0f, 0.0f);

	}
	if (key == GLUT_KEY_RIGHT){
		viewFrame.RotateLocal(-angular, 0.0f, 1.0f, 0.0f);

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