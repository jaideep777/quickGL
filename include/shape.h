#ifndef SIMPLEGL_SHAPE_H
#define SIMPLEGL_SHAPE_H
#include <GL/glew.h>
#include <GL/glut.h>
#include <list>
#include <string>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/vector_angle.hpp"

class Camera{
	public:
	glm::vec3 position, lookingAt, worldUp;
	glm::mat4 projection, view;
	glm::vec3 lineOfSight;
//	glm::mat4 trans;

	glm::mat4 projection0, view0;
	float tx, ty, rx, ry, sc;
	
	public:
	Camera(glm::vec3 _position, glm::vec3 _lookingAt, glm::vec3 _worldUp);
	glm::mat4 matrix();
	
	void transform();
};


class Shape{
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

	glm::vec3 getTransformedBBox0();
	glm::vec3 getTransformedBBox1();	

	void setVertices(float * verts);
	void setColors(float * cols);
	void setElements(int * ele, int nelements);
	void applyTexture(float * uvs, unsigned char * image, int w, int h);
	void autoExtent();

	void setShaderVariable(std::string s, glm::mat4 f);

	virtual void render();
	

};

#endif
