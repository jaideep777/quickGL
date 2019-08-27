#include "../include/camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_access.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/string_cast.hpp>

#include "../include/shape.h"

using namespace std;

// DONE: Transform camera after construction of axis aligned one to match specs
// Create axis-aligned camera, then rotate it to desired specs.
// Having the camera axis-aligned creates predictable camera panning independent of camera orientation
Camera::Camera(glm::vec3 _position, glm::vec3 _lookingAt, glm::vec3 _Up){

	// create camera aligned to x-axis
	glm::vec3 pos0 = glm::vec3(glm::length(_position - _lookingAt),0.f,0.f);

	camUp = _Up;
	lineOfSight = glm::normalize(0.f-pos0);	// at pos0 (on x axis), looking at origin
	camRt =  glm::normalize(glm::cross(lineOfSight, camUp));
	tx = ty = rx = ry = 0;
	sc = 1;

	view0 = view = glm::lookAt(pos0, glm::vec3(0.f, 0.f, 0.f), camUp);

	glm::vec3 dir = _position - _lookingAt;
	
	// calculate initial camera rotations required to bring it to specified position and lookAt
//	rx = asin(dir.z/glm::length(dir)); ry = -atan2(dir.y, dir.x);	// works only for z-up
	rx = asin(glm::dot(dir, camUp)/glm::length(dir));
	ry = -atan2(glm::dot(dir, camRt), glm::dot(dir, -lineOfSight));	

	// since we want rotation axis pinned at the actual origin, pre-rotate the translation vector by the initial rotations
	glm::mat4 tv = glm::mat4(1.f);
  	tv = glm::rotate(tv, rx, camRt); 	
  	tv = glm::rotate(tv, ry, camUp);	
	translation = tv*glm::vec4(-_lookingAt, 1.f); //1.f*tv*glm::vec4(+pos0-, 1.f);
		
	transform();
	
	projection0 = projection = glm::perspective(glm::radians(90.0f), float(glutGet(GLUT_WINDOW_WIDTH)) / glutGet(GLUT_WINDOW_HEIGHT), 0.1f, 1000.0f);
}


glm::mat4 Camera::matrix(){
	return projection*view;
}


void Camera::transform(){
//	glm::vec3 camRt = glm::cross(lineOfSight, camUp);
	view = view0;
  	view = glm::translate(view, translation);	
  	view = glm::translate(view, camRt*tx + camUp*ty);	
  	view = glm::scale(view, glm::vec3(sc, sc, sc));
  	view = glm::rotate(view, rx, camRt);  // camRt stays constant because Camera rotations are NOT incremental
  	view = glm::rotate(view, ry, camUp);	// First Rotation along up axis 
//	view = glm::translate(view, translation);
	
	// Sort shapes by distance to newly transformed camera. 
	// WARNING: This operation is expensive. Comment out this line if low graphics quality is required
	if (Shape::activeCamera == this) this->sortShapes();

}

void Camera::activate(){
	Shape::activeCamera = this;
	
	// Sort shapes by distance to newly activated camera
	this->sortShapes();
}


float Camera::distanceToShape(Shape * s){
	glm::mat4 invView = glm::inverse(view);
	glm::vec3 camDir = -glm::column(invView, 2);
	glm::vec3 camPos =  glm::column(invView, 3);
	
	glm::vec3 toCentroid = (s->getTransformedBBox0()+s->getTransformedBBox1())/2.f - camPos;
	
	float dist = glm::length(glm::dot(camDir, toCentroid));

	return dist;
//	cout << "LookAtDir: " << glm::to_string(camDir) << endl;  // 3rd Row is lookat DIRECTION
//	cout << "Cam Pos: "       << glm::to_string(camPos) << endl;  // 4th Row is camera POSITION
//	cout << "Shape Pos: "       << glm::to_string((s->getTransformedBBox0()+s->getTransformedBBox1())/2.f) << endl;  // 4th Row is camera POSITION
//	cout << ">> Dist = " << dist << endl;	
}


void Camera::sortShapes(){
	if (!glIsEnabled(GL_BLEND)) return; // No need of sorting if transparency is not enabled
	
	glm::mat4 invView = glm::inverse(view);
	glm::vec3 camDir = -glm::column(invView, 2);	// 3rd column contains the camera DIRECTION (not the original lookAt position)
//	cout << glm::to_string(camDir) << endl;
	glm::vec3 camPos =  glm::column(invView, 3);	// 4th column contains the camera POSITION  (in world coordintes)
	
	Shape::allShapes.sort([invView, camDir, camPos](Shape* &s1, Shape* &s2){ 
								glm::vec3 toCentroid1 = camPos - (s1->getTransformedBBox0()+s1->getTransformedBBox1())/2.f;
								float dist1 = (glm::dot(camDir, toCentroid1));

								glm::vec3 toCentroid2 = camPos - (s2->getTransformedBBox0()+s2->getTransformedBBox1())/2.f;
								float dist2 = (glm::dot(camDir, toCentroid2));
								
//								cout << "distances = " << dist1 << ", " << dist2 << endl;
								return dist1 < dist2; 
						   });
	
}


void Camera::onResize(int w, int h){
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	projection = glm::perspective(glm::radians(90.0f), float(w) / h, 0.1f, 1000.0f);
}




