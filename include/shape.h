#ifndef SIMPLEGL_SHAPE_H
#define SIMPLEGL_SHAPE_H
#include <GL/glew.h>
#include <GL/glut.h>

class Shape{
	public:
	int nVertices;
	int nElements;
	int dim;
	
	bool textured;
		
	GLuint vbo, cbo, ebo, tbo;

	GLuint tex;
	
	GLuint vertexShader, fragmentShader;
	GLuint program;

	public:
//	createShaders();
	Shape(int nverts, int _dim);
	void setVertices(float * verts);
	void setColors(float * cols);
	void setElements(int * ele, int nelements);
	void applyTexture(float * uvs, unsigned char * image, int w, int h);
	void deleteTexture();
	virtual void render();
	
	virtual ~Shape();

};

#endif
