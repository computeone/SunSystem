#include "GLVertexBuffer.h"

using namespace std;

GLVertexBuffer::GLVertexBuffer(){
}
void GLVertexBuffer::createBuffer(GLfloat* vVertexs,GLfloat* vNormals,GLfloat* vTexture,int VertexCount){
	GLVertexBuffer::VertexCount = VertexCount;


	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	glGenBuffers(3, bufferObject);

	glBindBuffer(GL_ARRAY_BUFFER, bufferObject[GL_VERTEX_ATTRIBUTE]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)* 3 * VertexCount, vVertexs, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(GL_VERTEX_ATTRIBUTE);
	glVertexAttribPointer(GL_VERTEX_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, bufferObject[GL_NORMAL_ATTRIBUTE]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)* 3 * VertexCount, vNormals, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(GL_NORMAL_ATTRIBUTE);
	glVertexAttribPointer(GL_NORMAL_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, bufferObject[GL_TEXTURE_ATTRIBUTE]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)* 2 * VertexCount, vTexture, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(GL_TEXTURE_ATTRIBUTE);
	glVertexAttribPointer(GL_TEXTURE_ATTRIBUTE, 2, GL_FLOAT, GL_FALSE, 0, 0);
	
};
void GLVertexBuffer::drawBuffer(GLuint primitive){
	glBindVertexArray(vertexArrayObject);
	glDrawArrays(primitive, 0, VertexCount);
};
GLVertexBuffer::~GLVertexBuffer(){
	glDeleteBuffers(1, &bufferObject[4]);
	glDeleteVertexArrays(1, &vertexArrayObject);
};