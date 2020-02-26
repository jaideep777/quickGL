#ifndef QUICKGL_GL_INIT_H_
#define QUICKGL_GL_INIT_H_


#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include <string>

#define CHECK_GL_ERROR() checkGLError(__FILE__, __LINE__)

extern GLuint vao;
/*extern int width;*/
/*extern int height;*/
extern GLFWwindow* window;

void checkGLError(const char * file, int line);
void printStatus(const char *step, GLuint context, GLuint status);

GLuint loadShader(std::string filename, GLenum shader_type);

bool windowIsOpen();

int initQuickGL(int argc, char** argv);
void closeQuickGL();

void onDisplay(void);

/*void onResize(int w, int h);*/

/*void onClick(int button, int state, int x, int y);*/

/*void onMouseMove(int x, int y);*/


#endif


