#include <GLTools.h>
#include <map>
#include "Util.h"
using namespace std;
class LoadShader{

private:
	GLuint VertexShader;
	GLuint FragmentShader;
	GLuint program;
public:
	LoadShader();
	boolean loadShaderPair(const char* szVertexProgFileName, const char* szFragProgFileName);
	GLuint createShader(map<GLuint, const GLchar*> attributes);
	~LoadShader();
};