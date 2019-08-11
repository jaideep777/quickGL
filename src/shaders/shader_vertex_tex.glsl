#version 330

in  vec3 in_pos;
in  vec2 in_UV;
in  vec4 in_col;

out vec2 ex_UV;
out vec4 ex_col;

void main()
{
  gl_Position = vec4(in_pos,1);
  ex_UV = in_UV;
  ex_col = in_col;
}
