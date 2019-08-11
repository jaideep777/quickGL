#version 330

in vec3 in_pos;
in vec2 in_UV;

out vec2 ex_UV;

void main(){
  gl_Position = vec4(in_pos, 1);
  ex_UV = in_UV;
}
