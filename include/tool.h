#ifndef SIMPLEGL_CAMERA_H
#define SIMPLEGL_CAMERA_H

#include <glm/glm.hpp>

class Tool{
	public:
	Shape * affectedShape; 		// affected shape
	Shape * displayShape; 		// tool's visualization shape

	public:
	Tool(Shape * _affectedShape);
	virtual ~Tool(){};
	virtual void update();

	virtual int  cursorLocation(int x, int y){}; // tells if cursor is outside (0), inside(1), bottom-edge (21), left-edge (22), top edge(23), right edge (24)
	virtual void changeCursor(int x, int y){};

	virtual bool blockNewSelection(int x, int y){};
	
	virtual void initialize(int x, int y){};
	virtual void transform(int x, int y){};
	virtual void finalize(int x, int y){};
	virtual void mouseMove(int x, int y);
	virtual void mouseHover(int x, int y);
	virtual void mouseClick(int x, int y){};
};




