
#version 130
uniform sampler2D colorMap;
smooth in vec2 vVaryingTexCoords;

void main(void){
	
	//��ͼ
	gl_FragColor=texture(colorMap,vVaryingTexCoords.st);
}