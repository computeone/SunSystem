#include <GLTools.h>
#include "Util.h"
using namespace std;

class Eclipse{
private:
	int pointCount;
public:
	Eclipse();
	void makeEclipse(GLfloat* vVertexs,GLfloat* vNormals,GLfloat* vTexture,
		GLfloat a,GLfloat b,int iSlice);
	int getCount();
	~Eclipse();
};