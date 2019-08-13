#include <math.h>
#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <string>
#include <fstream>
#include <list>
#include <algorithm>
#include <unistd.h>
#include "../include/shape.h"
using namespace std;

//extern list <Shape*> allShapes;

GLuint vao;

int width = 320;
int height = 240;

void onDisplay(void)
{
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClear(GL_COLOR_BUFFER_BIT);
//  ~~ Shape::render()
//  glUseProgram(program);
//  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)0);
//  ~~~
	for (auto it : Shape::allShapes) it->render();	
	  
  glutSwapBuffers();
}

void onResize(int w, int h)
{
  width = w; height = h;
  glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}


int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);	
  glutInitWindowSize(width, height);
  glutCreateWindow("mini");

  glewExperimental = GL_TRUE;
  glewInit();


  // BUFFERS etc

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLfloat vertices[] = {
	   0.5f,  0.5f,  0.0f,
	  -0.5f,  0.5f,  0.0f,
	  -0.5f, -0.5f,  0.0f,
	   0.5f, -0.5f,  0.0f
	};

	// texture setup
	GLfloat UVs[] = {
	   1.0f, 1.0f,
	   0.0f, 1.0f,
	   0.0f, 0.0f,
	   1.0f, 0.0f
	};

	float cols[] = {1,1,1,0, 1,0,0,0, 0,0,1,0, 0,1,0,0 };
	
	int indices[] = { 0, 1, 2, 2,3,0 };
	unsigned char pixels2[] = {
	  0, 0,255,0, 	0, 255, 0,0,
	  255,0,0,0,    255,255,255,0
	};

	int nVertices = 4;
	int nelements = 6;

	Shape s(4,3);
	s.setVertices(vertices);
//	s->setColors(cols);
	s.setElements(indices, 6);
//	s->applyTexture(UVs, pixels2, 2,2);


  glutDisplayFunc(onDisplay);
  glutReshapeFunc(onResize);
//	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);

	for (int i=0; i<1000; ++i){  
	glutMainLoopEvent();
	usleep(100);
	}

//	s.deleteTexture();
//	delete s;
	

  glBindVertexArray(0);
  glDeleteVertexArrays(1, &vao);


  return 0;
}



