#include <math.h>
#include <stdio.h>
#include <GL/glew.h>
#include <GL/glut.h>


const char *vertexSource = "#version 330\n\
in  vec3 in_pos;\n\
in  vec2 in_UV;\n\
in  vec4 in_col;\n\
out vec2 ex_UV;\n\
out vec4 ex_col;\n\
void main()\n\
{\n\
  gl_Position = vec4(in_pos,1);\n\
  ex_UV = in_UV;\n\
  ex_col = in_col;\n\
}";


const char *fragmentSource = "#version 330\n\
in  vec2 ex_UV;\n\
in  vec4 ex_col;\n\
out vec4 outColor;\n\
uniform sampler2D tex;\n\
void main()\n\
{\n\
  outColor = texture(tex, ex_UV);\n\
}";


GLuint vao;
GLuint vbo;
GLuint cbo;
GLuint tbo;
GLuint ebo;
GLuint tex;
GLuint program;
int width = 320;
int height = 240;

void onDisplay(void)
{
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  glUseProgram(program);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)0);
  glutSwapBuffers();
}

void onResize(int w, int h)
{
  width = w; height = h;
  glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}

void printError(const char *context)
{
  GLenum error = glGetError();
  if (error != GL_NO_ERROR) {
    fprintf(stderr, "%s: %s\n", context, gluErrorString(error));
  };
}

void printStatus(const char *step, GLuint context, GLuint status)
{
  GLint result = GL_FALSE;
  glGetShaderiv(context, status, &result);
  if (result == GL_FALSE) {
    char buffer[1024];
    if (status == GL_COMPILE_STATUS)
      glGetShaderInfoLog(context, 1024, NULL, buffer);
    else
      glGetProgramInfoLog(context, 1024, NULL, buffer);
    if (buffer[0])
      fprintf(stderr, "%s: %s\n", step, buffer);
  };
}

void printCompileStatus(const char *step, GLuint context)
{
  printStatus(step, context, GL_COMPILE_STATUS);
}

void printLinkStatus(const char *step, GLuint context)
{
  printStatus(step, context, GL_LINK_STATUS);
}


void loadShader(string filename, GLuint &shader_id, GLenum shader_type){

	ifstream fin(filename.c_str());
	string c((istreambuf_iterator<char>(fin)), istreambuf_iterator<char>());
	const char * glsl_src = c.c_str();

	shader_id = glCreateShader(shader_type);
	glShaderSource(shader_id, 1, &glsl_src, NULL);
	glCompileShader(shader_id);
	printStatus(filename.c_str(), shader_id, GL_COMPILE_STATUS);
	
}





int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(width, height);
  glutCreateWindow("mini");

  glewExperimental = GL_TRUE;
  glewInit();

  GLuint vertexShader;
  loadShader("src/shaders/shader_vertex_tex.glsl", vertexShader, GL_VERTEX_SHADER);
//   = glCreateShader(GL_VERTEX_SHADER);
//  glShaderSource(vertexShader, 1, &vertexSource, NULL);
//  glCompileShader(vertexShader);
//  printCompileStatus("Vertex shader", vertexShader);

  GLuint fragmentShader;
  loadShader("src/shaders/shader_vertex_tex.glsl", fragmentShader, GL_FRAGMENT_SHADER);
//  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//  glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
//  glCompileShader(fragmentShader);
//  printCompileStatus("Fragment shader", fragmentShader);

  program = glCreateProgram();
  glAttachShader(program, vertexShader);
  glAttachShader(program, fragmentShader);
  glLinkProgram(program);
  printLinkStatus("Shader program", program);

  // BUFFERS etc

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLfloat vertices[] = {
	   0.5f,  0.5f,  0.0f,
	  -0.5f,  0.5f,  0.0f,
	  -0.5f, -0.5f,  0.0f,
	   0.5f, -0.5f,  0.0f
	};
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	float cols[] = {1,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1};
	glGenBuffers(1, &cbo);
	glBindBuffer(GL_ARRAY_BUFFER, cbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), cols, GL_STATIC_DRAW);

	unsigned int indices[] = { 0, 1, 2, 2,3,0 };
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	// texture setup
	GLfloat UVs[] = {
	   1.0f, 1.0f,
	   0.0f, 1.0f,
	   0.0f, 0.0f,
	   1.0f, 0.0f
	};
	glGenBuffers(1, &tbo);	// use a separate array buffer to store UVs
	glBindBuffer(GL_ARRAY_BUFFER, tbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(UVs), UVs, GL_STATIC_DRAW);

	unsigned char pixels2[] = {
	  0, 0,255,0, 	0, 255, 0,0,
	  255,0,0,0,    255,255,255,0
	};
  glGenTextures(1, &tex);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, tex);
  glUniform1i(glGetUniformLocation(program, "tex"), 0);
//  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_FLOAT, pixels);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels2); // After reading one row of texels, pointer advances to next 4 byte boundary. Therefore ALWAYS use 4byte colour types. 
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glGenerateMipmap(GL_TEXTURE_2D);


	// drawing
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glVertexAttribPointer(glGetAttribLocation(program, "in_pos"), 3, GL_FLOAT, GL_FALSE, 0, 0);

  glBindBuffer(GL_ARRAY_BUFFER, cbo);
  glVertexAttribPointer(glGetAttribLocation(program, "in_col"), 4, GL_FLOAT, GL_FALSE, 0, 0);

  glBindBuffer(GL_ARRAY_BUFFER, tbo);
  glVertexAttribPointer(glGetAttribLocation(program, "in_UV"), 2, GL_FLOAT, GL_FALSE, 0, 0);

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);




  glutDisplayFunc(onDisplay);
  glutReshapeFunc(onResize);
  glutMainLoop();

  glDisableVertexAttribArray(2);
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(0);

  glBindTexture(GL_TEXTURE_2D, 0);
  glDeleteTextures(1, &tex);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glDeleteBuffers(1, &ebo);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glDeleteBuffers(1, &vbo);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glDeleteBuffers(1, &cbo);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glDeleteBuffers(1, &tbo);

  glBindVertexArray(0);
  glDeleteVertexArrays(1, &vao);

  glDetachShader(program, vertexShader);
  glDetachShader(program, fragmentShader);
  glDeleteProgram(program);
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  return 0;
}
