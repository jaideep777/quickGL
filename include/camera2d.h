#ifndef QUICKGL_CAMERA_2D_H_
#define QUICKGL_CAMERA_2D_H_

#include <glm/glm.hpp>

#include "../include/camera.h"

class Camera2D : public Camera{
	private:
	float aspectRatio;
	
	public:
	Camera2D(float x0, float xf, float y0, float yf, float scale);
	
	virtual void onResize(int w, int h);
};


#endif


