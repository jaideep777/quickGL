#include "../include/glinit.h"

#include <iostream>

#include "../include/shape.h"
#include "../include/camera.h"
#include "../include/tool_base.h"

using namespace std;

GLuint vao;
int width = 320;
int height = 240;


// TODO: Should there be a GLInterface class that controls the globals? 
//       e.g. It will store the active shapes list and active tools list? 
// 		 Maybe not?

void checkGLError(const char * file, int line){
	GLenum err;
	while((err = glGetError()) != GL_NO_ERROR){
		cout << file << ":" << line << "   Error: " << gluErrorString(err) << endl;
	}
}


void printStatus(const char *step, GLuint context, GLuint status){
	GLint result = GL_FALSE;
	CHECK_GL_ERROR();
	glGetShaderiv(context, status, &result);
	CHECK_GL_ERROR();
	if (result == GL_FALSE) {
		char buffer[1024];
		if (status == GL_COMPILE_STATUS)
			glGetShaderInfoLog(context, 1024, NULL, buffer);
		else
			glGetProgramInfoLog(context, 1024, NULL, buffer);
		if (buffer[0])
			fprintf(stderr, "%s: %s\n", step, buffer);
	}
}


void onDisplay(void){

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (auto it : Shape::allShapes) it->render();	

	glutSwapBuffers();
}


void onResize(int w, int h){
//	width = w; height = h;
	Shape::activeCamera->onResize(w,h); //glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}


void onClick(int button, int state, int x, int y){
	if (!Tool::activeTools.empty())	Tool::activeTools.front()->onClick(button, state, x, y);
}


void onMouseMove(int x, int y){
	if (!Tool::activeTools.empty())	Tool::activeTools.front()->onMouseMove(x, y);
}


void initQuickGL(int argc, char** argv){

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);	
	glutInitWindowSize(width, height);

	glutCreateWindow("mini");


	glEnable(GL_BLEND);
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_PROGRAM_POINT_SIZE);

	glewExperimental = GL_TRUE;
	glewInit();

	// BUFFERS etc

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glutDisplayFunc(onDisplay);
	glutReshapeFunc(onResize);
	glutMouseFunc(onClick);
	glutMotionFunc(onMouseMove);
	
}


void closeQuickGL(){
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &vao);

}

