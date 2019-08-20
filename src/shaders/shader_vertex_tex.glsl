#version 130

in vec3 in_pos;
in vec4 in_col;
in vec2 in_UV;

out vec2 ex_UV;
out vec4 ex_col;

uniform mat4 transform;
uniform float pointsize = 2;

void main()
{
  gl_Position = transform*vec4(in_pos,1);
  gl_PointSize = pointsize;
  ex_UV = in_UV;
  ex_col = in_col;
}

