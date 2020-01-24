#include  "../include/tool_base.h"

#include <algorithm>

#include "../include/camera.h"
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


void Tool::onKeyPress(unsigned char key, int x, int y){
	cout << key << " captured by Tool at: " << x << " " << y << endl;
}


void Tool::onSpecialKeyPress(int key, int x, int y){
	cout << key << " (special) captured by Tool at: " << x << " " << y << endl;
}


void Tool::onClick(int button, int state, int x, int y){
	captureClick(button, state, x,y);		
}


void Tool::onMouseMove(int x, int y){

}











	




