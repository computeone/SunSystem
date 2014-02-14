#include <GLTools.h>
#include "Util.h"
using namespace std;

class Eclipse{

public:
	Eclipse();
	void makeEclipse(GLfloat* vVertexs,GLfloat* vNormals,GLfloat* vTexture,
		GLfloat a,GLfloat b,int iSlice);
	~Eclipse();
};