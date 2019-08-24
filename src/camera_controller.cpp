#include "../include/camera_controller.h"

#include <GL/glew.h>
#include <GL/glut.h>

#include "../include/shape.h"
#include "../include/camera.h"


void CameraController::onClick(int button, int state, int x, int y){
//	cout << "Clicked by Tool::CameraTransformer at: " << x << " " << y << endl;
	captureClick(button, state, x,y);		

	if (Shape::activeCamera == NULL) return;

	if (button == 3 && state == GLUT_DOWN){	// wheel down / pinch out
		Shape::activeCamera->sc *= 1+0.1;
	
	}
	if (button == 4 && state == GLUT_DOWN){	// wheel up / pinch in
		Shape::activeCamera->sc *= 1-0.1;
	}

	Shape::activeCamera->transform();
	
	glutPostRedisplay();
}
	
		
void CameraController::onMouseMove(int x, int y){
//	float h = glutGet(GLUT_WINDOW_HEIGHT);
//	float w = glutGet(GLUT_WINDOW_WIDTH);

	if (Shape::activeCamera == NULL) return;

	if (lClick){
		Shape::activeCamera->rx += 0.02*(y - y0);
		Shape::activeCamera->ry += 0.02*(x - x0);
	}
//	if (rClick){
//		float r = (y - y0)/h;
//		cam->sc *= 1+r;
//	}
	if (mClick || rClick){
		Shape::activeCamera->ty -= (1/250.f)*(y - y0);	// -= because y is measured from top
		Shape::activeCamera->tx += (1/250.f)*(x - x0);
	}
	y0 = y;
	x0 = x;

	Shape::activeCamera->transform();
	
	glutPostRedisplay();
}

//	InteractiveCamera(glm::vec3 _position, glm::vec3 _lookingAt, glm::vec3 _Up) : Tool() {
//		cout << "Constructing ICam" << endl;
//		cam = new Camera(_position, _lookingAt, _Up);
//	}

//	virtual ~InteractiveCamera(){
//		delete cam;
//		cout << "Destroying ICam" << endl;
//	}






