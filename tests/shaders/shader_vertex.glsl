#version 130

in vec3 in_pos;
in vec4 in_col;
in vec2 in_UV;
in vec3 in_normal;

out vec3 ex_pos;
out vec2 ex_UV;
out vec4 ex_col;
out vec3 ex_normal;

uniform mat4 PVWM; // full transformation matix: projection*view*world*model
uniform mat4 WM;   // object transformation matrix: world*model
uniform mat3 normalMatrix; // matrix for transforming normals (rotation only)

uniform float pointsize = 2;

void main()
{
  gl_Position = PVWM * vec4(in_pos,1);
  gl_PointSize = pointsize;
  ex_normal = in_normal;
  ex_UV = in_UV;
  ex_col = in_col;
}

