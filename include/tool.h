#ifndef SIMPLEGL_CAMERA_H
#define SIMPLEGL_CAMERA_H

#include <glm/glm.hpp>

class Tool{
	public:
	Shape * s; 		// affected shape
	Shape * box; 	// tool's visualization shape

	public:
	Tool(Shape * _s);
	virtual ~Tool(){};
	virtual void update();

	virtual int  cursorLocation(int x, int y){}; // tells if cursor is outside (0), inside(1), bottom-edge (21), left-edge (22), top edge(23), right edge (24)
	virtual void changeCursor(int x, int y){};

	virtual bool blockNewSelection(int x, int y){};
	
	virtual void initialize(int x, int y){};
	virtual void transform(int x, int y){};
	virtual void finalize(int x, int y){};
};


class Camera{
	public:
	glm::vec3 worldUpAxis;
	glm::mat4 projection, view;
	
	public:
	Camera(glm::vec3 position, glm::vec3 lookingAt, glm::vec3 upAxis);
	projectPerspective();
	projectOrtho();
}
