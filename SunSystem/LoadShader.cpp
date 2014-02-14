#include "LoadShader.h"
#include <iostream>
#include <string>
using namespace std;

LoadShader::LoadShader(){
}


//���ض�����ɫ��
boolean LoadShader::loadShaderPair(const char* szVertexProgFileName,const char* szFragProgFileName){
	
	VertexShader = glCreateShader(GL_VERTEX_SHADER);
	FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	if (gltLoadShaderFile(szVertexProgFileName, VertexShader) == false){
		cout << "�޷����ض�����ɫ����" << endl;
		glDeleteShader(GL_VERTEX_SHADER);
		glDeleteShader(GL_FRAGMENT_SHADER);
		return false;
	}
	//����Ƭ����ɫ��
	if (gltLoadShaderFile(szFragProgFileName, FragmentShader) == false){
		cout << "�޷�����Ƭ����ɫ����" << endl;
		glDeleteShader(GL_VERTEX_SHADER);
		glDeleteShader(GL_FRAGMENT_SHADER);
		return false;
	}
	return true;
}

//������ɫ��
GLuint LoadShader::createShader(map<GLuint, const GLchar*> attributes){

	
	glCompileShader(VertexShader);
	glCompileShader(FragmentShader);

	GLint testVal;
	glGetShaderiv(VertexShader, GL_COMPILE_STATUS, &testVal);
	if (testVal == GL_FALSE){
		glDeleteShader(VertexShader);
		glDeleteShader(FragmentShader);
		return 0;
	}

	glGetShaderiv(FragmentShader, GL_COMPILE_STATUS, &testVal);
	if (testVal == GL_FALSE){
		glDeleteShader(VertexShader);
		glDeleteShader(FragmentShader);
		return 0;
	}

	program = glCreateProgram();
	glAttachShader(program, VertexShader);
	glAttachShader(program, FragmentShader);

	map<GLuint, const GLchar*>::iterator iter;
	for (iter = attributes.begin(); iter != attributes.end(); ++iter){
		glBindAttribLocation(program, iter->first,iter->second);
	}
	glLinkProgram(program);
	return program;

}

LoadShader::~LoadShader(){
	glDeleteProgram(program);
}