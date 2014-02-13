#include <GLTools.h>
#include <map>
using namespace std;
class LoadShader{

private:
	GLuint VertexShader;
	GLuint FragmentShader;
	GLuint program;
public:
	LoadShader(GLuint program);
	boolean loadVertexShader(GLuint vertexshader,const char* filename);
	boolean loadFragmentShader(GLuint fragmentshader,const char*  filename);
	boolean createShader(map<GLuint, const GLchar*> attributes);
	~LoadShader();
};