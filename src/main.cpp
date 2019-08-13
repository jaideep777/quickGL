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


  glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);	
  glutInitWindowSize(width, height);

  glutCreateWindow("mini");

	
 	glEnable(GL_BLEND);
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	glEnable(GL_DEPTH_TEST);

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

	float cols[] = {1,1,1,0.5, 1,0,0,0.5, 0,0,1,0.5, 0,1,0,0.5 };
	
	int indices[] = { 0, 1, 2, 2,3,0 };
	unsigned char pixels2[] = {
	  0, 0,255,255, 	0, 255, 0,255,
	  255,0,0,255,    255,255,255,255
	};

	int nVertices = 4;
	int nelements = 6;

//	Shape s(4,3);
//	s.setVertices(vertices);
////	s.setColors(cols);
//	s.setElements(indices, 6);
////	s.applyTexture(UVs, pixels2, 2,2);


  glutDisplayFunc(onDisplay);
  glutReshapeFunc(onResize);
//	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);


	// default camera looks into +z direction, so -ve z values are in front.
	GLfloat vertices_b[] = {
	   0.25f,  0.25f,  0.5f,
	  -0.75f,  0.25f,  0.5f,
	  -0.75f, -0.75f,  0.5f,
	   0.25f, -0.75f,  0.5f
	};
	float cols_r[] = {1,0,0,0.5, 1,0,0,0.5, 1,0,0,0.5, 1,0,0,0.5 };

	GLfloat vertices_f[] = {
	   0.75f,  0.75f,  -0.5f,
	  -0.25f,  0.75f,  -0.5f,
	  -0.25f, -0.25f,  -0.5f,
	   0.75f, -0.25f,  -0.5f
	};
	float cols_b[] = {0,0,1,0.5, 0,0,1,0.5, 0,0,1,0.5, 0,0,1,0.5 };
	float cols_g[] = {0,1,0,0.5, 0,1,0,0.5, 0,1,0,0.5, 0,1,0,0.5 };


	Shape back(4,3);
	back.setVertices(vertices_b);
	back.setColors(cols_g);
	back.setElements(indices, 6);

	Shape front(4,3);
	front.setVertices(vertices_f);
	front.setColors(cols_b);
	front.setElements(indices, 6);



//	for (int i=0; i<10000; ++i){  
	glutMainLoop();
//	usleep(100);
//	}

//	s.deleteTexture();
//	delete s;
	

  glBindVertexArray(0);
  glDeleteVertexArrays(1, &vao);


  return 0;
}



