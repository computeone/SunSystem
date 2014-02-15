
#version 130
uniform sampler2D colorMap;
uniform vec4 color;
smooth in vec2 vVaryingTexCoords;

void main(void){
	
	//ÌùÍ¼
	gl_FragColor=texture(colorMap,vVaryingTexCoords.st);
	//gl_FragColor=color;
}