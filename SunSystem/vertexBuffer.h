#include <GLTools.h>
using namespace std;

#define GL_VERTEX_ATTRIBUTE  0
#define GL_NORMAL_ATTRIBUTE  1
#define GL_TEXTURE_ATTRIBUTE 2
#define GL_COLOR_ATTRIBUTE   3

/*
缓存顶点数据，生成顶点
*/
class vertexBuffer{

private:
	GLuint  vertexArrayObject;
public:
	vertexBuffer(GLuint vertexArrayObject);
	void addVertexBuffer(GLuint index,GLuint vertexArray,GLfloat* vertexs,GLuint size,GLuint vertexCount, GLuint
		mode);
	~vertexBuffer();
};