#include "vertexBuffer.h"
using namespace std;

vertexBuffer::vertexBuffer(GLuint vertexArrayObject){
	vertexBuffer::vertexArrayObject = vertexArrayObject;
	glGenVertexArrays(1, &vertexArrayObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexArrayObject);
}
void vertexBuffer::addVertexBuffer(GLuint index,GLuint vertexArray,GLfloat* vertexs, GLuint size,GLuint vertexCount,
	GLuint mode){
	glGenBuffers(1, &vertexArray);
	glBindBuffer(GL_ARRAY_BUFFER, vertexArray);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*size*vertexCount, vertexs, mode);
	glEnableVertexAttribArray(index);
	glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, 0, 0);
}

vertexBuffer::~vertexBuffer(){
	glDeleteVertexArrays(1,&vertexArrayObject);
}