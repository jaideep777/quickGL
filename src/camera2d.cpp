#include "../include/camera2d.h"

#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>

#include <glm/gtc/matrix_transform.hpp>

#include "../include/shape.h"

using namespace std;


// FIXME: ZOOM should be about the mouse cursor, pan should preserve content under cursor
	
Camera2D::Camera2D(float x0, float xf, float y0, float yf, float scale) : Camera(glm::vec3(0.f, 0.f, 100.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f)) {
	
	aspectRatio = (xf-x0)/(yf-y0);
	cout << "Aspect ratio = " << aspectRatio << endl;
		
	int window_width  = glutGet(GLUT_WINDOW_WIDTH);
	int window_height = glutGet(GLUT_WINDOW_HEIGHT);
	
//	int w1 = fmin(window_width, int(window_height*aspectRatio));
//	int h1 = fmin(window_height, int(w1/aspectRatio));
//	w1 = h1*aspectRatio;
	tx = -(x0+xf)/2; ty = -(y0+yf)/2;
	scale = min(xf-x0, yf-y0);
	
//	projection0 = projection = glm::ortho(glm::radians(90.0f), float() / , 0.1f, 1000.0f);
	onResize(window_width, window_height);
	
	transform();
}


void Camera2D::onResize(int w, int h){

	int w1 = min(w, int(h*aspectRatio));
	int h1 = min(h, int(w1/aspectRatio));
	w1 = h1*aspectRatio;
	
	h1 = h1*0.9;
	w1 = w1*0.9;
		
//	float viewport_aspect_ratio = w/h;
	
	float dx = float(w - w1)/2*1*aspectRatio/w1;
	float dy = float(h - h1)/2*1/h1;
	
	cout << "dx dy = " << dx << " " << dy << endl;
//	view = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 0.f, -100.0f) );

	glViewport(0,0,w,h);
 	//projection = glm::perspective(glm::radians(90.0f), float(window_width) / window_height, 0.1f, 1000.0f);
	//projection = glm::ortho(-10.0f*viewport_aspect_ratio, 110.0f*viewport_aspect_ratio, -10.0f, 110.0f, -10.f, 110.0f);
	projection = glm::ortho(-0.5f*aspectRatio-dx, 0.5f*aspectRatio+dx, -0.5f-dy, 0.5f+dy, -10.f, 110.0f);

}




