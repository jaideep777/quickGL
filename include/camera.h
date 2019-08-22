#ifndef SIMPLEGL_CAMERA_H
#define SIMPLEGL_CAMERA_H

#include <glm/glm.hpp>

class Shape;

class Camera{
	public:
//	glm::vec3 position, lookingAt;
	glm::vec3 camUp, camRt;
	glm::mat4 projection, view;
	glm::vec3 lineOfSight;
	glm::vec3 translation;

	glm::mat4 projection0, view0;
	float tx, ty, rx, ry, sc;
	
	public:
	Camera(glm::vec3 _position, glm::vec3 _lookingAt, glm::vec3 _worldUp);
	glm::mat4 matrix();
	
	void  transform();
	void  activate();
	float distanceToShape(Shape * s);
	void  sortShapes();

};


#endif


