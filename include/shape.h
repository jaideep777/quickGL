#ifndef QUICKGL_SHAPE_H_
#define QUICKGL_SHAPE_H_

#include <list>
#include <string>

#include <glm/glm.hpp>
#include "../include/glinit.h"

class Camera;

class Shape{
//	friend class Camera;	// make Camera friend to give access to activeCamera
	public:
	int nVertices;
	int nElements;
		
	bool useColor, useTexture, useElements;
	bool useTransparency;
		
	GLuint vbo, cbo, ebo, tbo;

	GLuint tex;
	
	GLenum renderMode;
	GLuint vertexShader, fragmentShader;
	GLuint program;

	glm::mat4 model, world;		// transformations
	glm::vec3 bbox0, bbox1;		// bounding box of original shape, in untransformed space
	
	public:
	static std::list<Shape*> allShapes;	// a list of all existing shapes 
	static Camera* activeCamera;
	
	public:
	Shape(int nverts, GLenum mode);
	virtual ~Shape();

	void setVertices(float * verts);
	void setColors(float * cols);
	void setElements(int * ele, int nelements);
	void applyTexture(float * uvs, unsigned char * image, int w, int h);
	void autoExtent();

	void setShaderVariable(std::string s, glm::mat4 f);
	void setPointSize(float psize);
	void setTextureFilters(GLint minFilter, GLint magFilter);
	
	virtual void render();
	
	glm::vec3 getTransformedBBox0();
	glm::vec3 getTransformedBBox1();	


};

#endif


