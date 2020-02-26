#version 130

in  vec2 ex_UV;
in  vec4 ex_col;

out vec4 outColor;

uniform sampler2D tex;

void main()
{	
	outColor = mix(texture(tex, ex_UV), ex_col, 0.5); 
}

