
#version 130
uniform sampler2D colorMap;
smooth in vec2 vVaryingTexCoords;

void main(void){
	
	//ÌùÍ¼
	gl_FragColor=texture(colorMap,vVaryingTexCoords.st);
}