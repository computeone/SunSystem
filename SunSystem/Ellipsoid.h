#include <GLTools.h>
using namespace std;

class Ellipsoid{
public:
	Ellipsoid();
	void makeEllipsoid(GLfloat* vVertexs,GLfloat* vNormals,GLfloat* vTextures,
		GLfloat a,GLfloat b,GLfloat c,int iSlice,int iStack);
	~Ellipsoid();
};