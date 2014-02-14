#include <GLTools.h>
#include "Util.h"
using namespace std;

#define GL_VERTEX_ATTRIBUTE  0
#define GL_NORMAL_ATTRIBUTE  1
#define GL_TEXTURE_ATTRIBUTE 2
#define GL_COLOR_ATTRIBUTE   3

/*
缓存顶点数据，生成顶点
*/
class GLVertexBuffer{

private:
	GLuint  vertexArrayObject;
	GLuint  bufferObject[4];
	GLuint  VertexCount;
public:
	GLVertexBuffer();
	void createBuffer(GLfloat* vVertexs,GLfloat* vNormals,GLfloat* vTexture,int VertexCount);
	void drawBuffer(GLuint primitive);
	~GLVertexBuffer();
};