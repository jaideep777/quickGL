#version 330
 
in vec4 ex_col;
in vec2 ex_UV;

out vec4 outColor;

uniform sampler2D tex;

void main(void){
	outColor = texture(tex, ex_UV);//+ex_col;
}


