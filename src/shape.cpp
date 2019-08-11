#include "../include/shape.h"
#include <fstream>
#include <string>
#include <iostream>
using namespace std;

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


GLuint loadShader(string filename, GLenum shader_type){
	cout << "Loading shader from " << filename << endl;
	ifstream fin(filename.c_str());
	string c((istreambuf_iterator<char>(fin)), istreambuf_iterator<char>());
	const char * glsl_src = c.c_str();

	GLuint shader_id = glCreateShader(shader_type);
	glShaderSource(shader_id, 1, &glsl_src, NULL);
	glCompileShader(shader_id);
	printStatus(filename.c_str(), shader_id, GL_COMPILE_STATUS);
	
	return shader_id;
}

//void Shape::createShaders(){


//}


Shape::Shape(int nverts, int _dim){
	nVertices = nverts;
	dim = _dim;
	
	vertexShader = loadShader("src/shaders/shader_vertex_tex.glsl", GL_VERTEX_SHADER);
	fragmentShader = loadShader("src/shaders/shader_fragment_tex.glsl", GL_FRAGMENT_SHADER);

	program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);
	printLinkStatus("Shader program", program);
	
	glGenBuffers(1, &vbo);
//	glBindBuffer(GL_ARRAY_BUFFER, vbo);
//	glBufferData(GL_ARRAY_BUFFER, dim*nVertices*sizeof(float), NULL, GL_DYNAMIC_DRAW);

	glGenBuffers(1, &cbo);
//	glBindBuffer(GL_ARRAY_BUFFER, cbo);
//	glBufferData(GL_ARRAY_BUFFER, 4*nVertices*sizeof(float), NULL, GL_DYNAMIC_DRAW);

	glGenBuffers(1, &ebo);
	glGenBuffers(1, &tbo);
	
}

void Shape::setVertices(float * verts){
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, dim*nVertices*sizeof(float), verts, GL_DYNAMIC_DRAW);
}

void Shape::setColors(float * cols){
	glBindBuffer(GL_ARRAY_BUFFER, cbo);
	glBufferData(GL_ARRAY_BUFFER, 4*nVertices*sizeof(float), cols, GL_DYNAMIC_DRAW);
}

void Shape::setElements(int * ele, int nelements){
	nElements = nelements;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, nelements*sizeof(int), ele, GL_DYNAMIC_DRAW);
}


void Shape::applyTexture(float * uvs, unsigned char * pixels, int w, int h){
	textured = true;
	glBindBuffer(GL_ARRAY_BUFFER, tbo);
	glBufferData(GL_ARRAY_BUFFER, 2*nVertices*sizeof(float), uvs, GL_DYNAMIC_DRAW);


	glGenTextures(1, &tex);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);
	glUniform1i(glGetUniformLocation(program, "tex"), 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels); // After reading one row of texels, pointer advances to next 4 byte boundary. Therefore ALWAYS use 4byte colour types. 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glGenerateMipmap(GL_TEXTURE_2D);

}

void Shape::render(){
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(glGetAttribLocation(program, "in_pos"), 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, cbo);
	glVertexAttribPointer(glGetAttribLocation(program, "in_col"), 4, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, tbo);
	glVertexAttribPointer(glGetAttribLocation(program, "in_UV"), 2, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glUseProgram(program);
	glDrawElements(GL_TRIANGLES, nElements, GL_UNSIGNED_INT, (void *)0);

	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

}


void Shape::deleteTexture(){
	if (textured){
		glBindTexture(GL_TEXTURE_2D, 0);
		glDeleteTextures(1, &tex);
	}
}

Shape::~Shape(){

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &ebo);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &vbo);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &cbo);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &tbo);

	glDetachShader(program, vertexShader);
	glDetachShader(program, fragmentShader);
	glDeleteProgram(program);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	
}



