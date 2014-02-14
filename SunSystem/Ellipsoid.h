#include <GLTools.h>
#include "Util.h"
using namespace std;

class Ellipsoid{
public:
	Ellipsoid();
	void makeEllipsoid(GLfloat* vVertexs,GLfloat* vNormals,GLfloat* vTextures,
		GLfloat fa,GLfloat fb,GLfloat fc,int iSlice,int iStack);
	~Ellipsoid();
};