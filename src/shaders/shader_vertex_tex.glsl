#version 330

in vec3 in_pos;
in vec4 in_col;
in vec2 in_UV;

out vec2 ex_UV;
out vec4 ex_col;

uniform mat4 transform;

void main()
{
  gl_Position = transform*vec4(in_pos,1);
  ex_UV = in_UV;
  ex_col = in_col;
}

