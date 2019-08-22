#include  "../include/tool.h"
#include <algorithm>
using namespace std;


list <Tool*> Tool::activeTools;


Tool::Tool(){
	mClick = lClick = rClick = 0;
	activeTools.push_front(this);
	cout << "Constructing tool #" << activeTools.size() << ", " << activeTools.size() << " in list" << endl;
}


Tool::~Tool(){
	try{
		list<Tool*>::iterator it = find(activeTools.begin(), activeTools.end(), this);
		if (it == activeTools.end()) throw string("Tool destructor could not find tool in active list!");
		activeTools.erase(it);
		cout << "Destroying tool, " << activeTools.size() << " left in list." << endl;
	}
	catch(string s){ 
		cout << "FATAL ERROR: " << s << endl; 
	}
}


void Tool::activate(){
	// bring to front of queue
	try{
		list<Tool*>::iterator it = find(activeTools.begin(), activeTools.end(), this);
		if (it == activeTools.end()) throw string("Tool destructor could not find tool in active list!");
		activeTools.splice(activeTools.begin(), activeTools, it);	// erase *it from current location (it) and put it at begin()
	}
	catch(string s){ 
		cout << "FATAL ERROR: " << s << endl; 
	}
}


void Tool::captureClick(int button, int state, int x, int y){
	if (button == GLUT_LEFT_BUTTON){
		if (state == GLUT_DOWN){
			cout << "L Click captured by Tool at: " << x << " " << y << endl;
			lClick = true;
			x0 = x;
			y0 = y;
		}
		else{
			lClick = false;
		} 
	}

	if (button == GLUT_MIDDLE_BUTTON){
		if (state == GLUT_DOWN){
			cout << "M Click captured by Tool at: " << x << " " << y << endl;
			mClick = true;
			x0 = x;
			y0 = y;
		}
		else{
			mClick = false;
		} 
	}

	if (button == GLUT_RIGHT_BUTTON){
		if (state == GLUT_DOWN){
			cout << "R Click captured by Tool at: " << x << " " << y << endl;
			rClick = true;
			x0 = x;
			y0 = y;
		}
		else{
			rClick = false;
		} 

	}
}


void Tool::onClick(int button, int state, int x, int y){
	captureClick(button, state, x,y);		
}


void Tool::onMouseMove(int x, int y){

}











	
void InteractiveCameraTool::onClick(int button, int state, int x, int y){
//	cout << "Clicked by Tool::CameraTransformer at: " << x << " " << y << endl;
	captureClick(button, state, x,y);		

	if (button == 3 && state == GLUT_DOWN){	// wheel down / pinch out
		Shape::activeCamera->sc *= 1+0.1;
	
	}
	if (button == 4 && state == GLUT_DOWN){	// wheel up / pinch in
		Shape::activeCamera->sc *= 1-0.1;
	}

	Shape::activeCamera->transform();
	
	glutPostRedisplay();
}
	
		
void InteractiveCameraTool::onMouseMove(int x, int y){
	float h = glutGet(GLUT_WINDOW_HEIGHT);
	float w = glutGet(GLUT_WINDOW_WIDTH);

	if (lClick){
		Shape::activeCamera->rx += 0.02*(y - y0);
		Shape::activeCamera->ry += 0.02*(x - x0);
	}
//	if (rClick){
//		float r = (y - y0)/h;
//		cam->sc *= 1+r;
//	}
	if (mClick || rClick){
		Shape::activeCamera->ty -= (h/250.f)*(y - y0)/h;	// -= because y is measured from top
		Shape::activeCamera->tx += (w/250.f)*(x - x0)/w;
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









