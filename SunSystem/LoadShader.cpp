#include "LoadShader.h"
#include <iostream>
#include <string>
using namespace std;

LoadShader::LoadShader(GLuint program){
	LoadShader::program = program;
}


//���ض�����ɫ��
boolean LoadShader::loadVertexShader(GLuint vertexshader,const char* filename){
	
	LoadShader::VertexShader = vertexshader;

	vertexshader = glCreateShader(GL_VERTEX_SHADER);
	if (gltLoadShaderFile(filename, vertexshader) == false){
		cout << "�޷����ض�����ɫ����" << endl;
		glDeleteShader(vertexshader);
		return false;
	}
	return true;
}

//����Ƭ����ɫ��
boolean LoadShader::loadFragmentShader(GLuint fragmentshader,const char* filename){

	FragmentShader = fragmentshader;

	fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);
	if (gltLoadShaderFile(filename, fragmentshader) == false){
		cout << "�޷�����Ƭ����ɫ����" << endl;
		glDeleteShader(fragmentshader);
		return false;
	}
	return true;
}

//������ɫ��
boolean LoadShader::createShader(map<GLuint, const GLchar*> attributes){
	glCompileShader(LoadShader::VertexShader);
	glCompileShader(LoadShader::FragmentShader);

	program = glCreateProgram();
	glAttachShader(program, VertexShader);
	glAttachShader(program, FragmentShader);

	map<GLuint, const GLchar*>::iterator iter;
	for (iter = attributes.begin(); iter != attributes.end(); ++iter){
		glBindAttribLocation(program, iter->first,iter->second);
	}

	glLinkProgram(program);
	return true;

}

LoadShader::~LoadShader(){
	glDeleteProgram(program);
}