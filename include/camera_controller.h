#ifndef QUICKGL_CAMERA_CONTROLLER_H_
#define QUICKGL_CAMERA_CONTROLLER_H_

#include "../include/tool_base.h"

class CameraController : public Tool{
	public:

	virtual void onClick(int button, int state, int x, int y);
	virtual void onMouseMove(int x, int y);
	
};


#endif


