#ifndef SIMPLEGL_SHAPE_H
#define SIMPLEGL_SHAPE_H
#include <GL/glew.h>
#include <GL/glut.h>
#include <list>

class Shape{
	public:
	int nVertices;
	int nElements;
	int dim;
	
	bool useColor, useTexture, useElements;
		
	GLuint vbo, cbo, ebo, tbo;

	GLuint tex;
	
	GLuint vertexShader, fragmentShader;
	GLuint program;

	public:
	static std::list<Shape*> allShapes;	// a list of all currently existing shapes 
	
	public:
	Shape(int nverts, int _dim);
	virtual ~Shape();

	void setVertices(float * verts);
	void setColors(float * cols);
	void setElements(int * ele, int nelements);
	void applyTexture(float * uvs, unsigned char * image, int w, int h);
	void deleteTexture();

	virtual void render();
	

};

#endif
