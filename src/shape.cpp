#include "../include/shape.h"
#include <fstream>
#include <string>
#include <iostream>
#include <algorithm>
#include <numeric>
using namespace std;

#define CHECK_GL_ERROR() checkGLError(__FILE__, __LINE__)

//glm::mat4 projection(1.f);
//glm::mat4 view(1.f); 



void checkGLError(const char * file, int line){
	GLenum err;
	while((err = glGetError()) != GL_NO_ERROR){
		cout << file << ":" << line << "   Error: " << gluErrorString(err) << endl;
	}
}

void printStatus(const char *step, GLuint context, GLuint status){
	GLint result = GL_FALSE;
	CHECK_GL_ERROR();
	glGetShaderiv(context, status, &result);
	CHECK_GL_ERROR();
	if (result == GL_FALSE) {
		char buffer[1024];
		if (status == GL_COMPILE_STATUS)
			glGetShaderInfoLog(context, 1024, NULL, buffer);
		else
			glGetProgramInfoLog(context, 1024, NULL, buffer);
		if (buffer[0])
			fprintf(stderr, "%s: %s\n", step, buffer);
	}
}


// DONE: Transform camera after construction of axis aligned one to match specs
//Camera::Camera(glm::vec3 _position, glm::vec3 _lookingAt, glm::vec3 _worldUp){
//	position = _position;
//	lookingAt = _lookingAt;
//	worldUp = _worldUp;
//	
//	view0 = view = glm::lookAt(position, lookingAt, worldUp);
//	projection0 = projection = glm::perspective(glm::radians(90.0f), float(glutGet(GLUT_WINDOW_WIDTH)) / glutGet(GLUT_WINDOW_HEIGHT), 0.1f, 1000.0f);
//	lineOfSight = glm::normalize(lookingAt - position);
//	tx = ty = rx = ry = 0;
//	sc = 1;
//}


// Create axis-aligned camera, then rotate it to desired specs.
// Having the camera axis-aligned creates predictable camera panning independent of camera orientation
Camera::Camera(glm::vec3 _position, glm::vec3 _lookingAt, glm::vec3 _worldUp){

	// create camera aligned to x-axis
	glm::vec3 pos0 = glm::vec3(glm::length(_position - _lookingAt),0.f,0.f);

	worldUp = _worldUp;
	lineOfSight = glm::normalize(0.f-pos0);	// at pos0 (on x axis), looking at origin
	worldRt =  glm::normalize(glm::cross(lineOfSight, worldUp));
	tx = ty = rx = ry = 0;
	sc = 1;

	view0 = view = glm::lookAt(pos0, glm::vec3(0.f, 0.f, 0.f), worldUp);

	glm::vec3 dir = _position - _lookingAt;
	
	// calculate initial camera rotations required to bring it to specified position and lookAt
//	rx = asin(dir.z/glm::length(dir))*10; ry = -atan2(dir.y, dir.x)*10;	// works only for z-up
	rx = asin(glm::dot(dir, worldUp)/glm::length(dir));
	ry = -atan2(glm::dot(dir, worldRt), glm::dot(dir, -lineOfSight));	

	// since we want rotation axis pinned at the actual origin, pre-rotate the translation vector by the initial rotations
	glm::mat4 tv = glm::mat4(1.f);
  	tv = glm::rotate(tv, rx, worldRt); 	
  	tv = glm::rotate(tv, ry, worldUp);	
	translation = tv*glm::vec4(-_lookingAt, 1.f); //1.f*tv*glm::vec4(+pos0-, 1.f);
		
	transform();
	
	projection0 = projection = glm::perspective(glm::radians(90.0f), float(glutGet(GLUT_WINDOW_WIDTH)) / glutGet(GLUT_WINDOW_HEIGHT), 0.1f, 1000.0f);
}


glm::mat4 Camera::matrix(){
	return projection*view;
}


void Camera::transform(){
//	glm::vec3 worldRt = glm::cross(lineOfSight, worldUp);
	view = view0;
  	view = glm::translate(view, translation);	
  	view = glm::translate(view, worldRt*tx + worldUp*ty);	
  	view = glm::scale(view, glm::vec3(sc, sc, sc));
  	view = glm::rotate(view, rx, worldRt);  // worldRt stays constant because Camera rotations are NOT incremental
  	view = glm::rotate(view, ry, worldUp);	// First Rotation along up axis 
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
	glm::vec3 camPos =  glm::column(invView, 3);	// 4th column contains the camera POSITION  (in world coordintes)
	
	Shape::allShapes.sort([invView, camDir, camPos](Shape* &s1, Shape* &s2){ 
								glm::vec3 toCentroid1 = (s1->getTransformedBBox0()+s1->getTransformedBBox1())/2.f - camPos;
								float dist1 = (glm::dot(camDir, toCentroid1));

								glm::vec3 toCentroid2 = (s2->getTransformedBBox0()+s2->getTransformedBBox1())/2.f - camPos;
								float dist2 = (glm::dot(camDir, toCentroid2));
								
//								cout << "distances = " << dist1 << ", " << dist2 << endl;
								return dist1 > dist2; 
						   });
	
}


GLuint loadShader(string filename, GLenum shader_type){
	cout << "Loading shader from " << filename << endl;
	ifstream fin(filename.c_str());
	string c((istreambuf_iterator<char>(fin)), istreambuf_iterator<char>());
	const char * glsl_src = c.c_str();

	GLuint shader_id = glCreateShader(shader_type);
	glShaderSource(shader_id, 1, &glsl_src, NULL);
	glCompileShader(shader_id);
	printStatus(filename.c_str(), shader_id, GL_COMPILE_STATUS);
	
	return shader_id;
}


list<Shape*> Shape::allShapes;
Camera* Shape::activeCamera = NULL;

Shape::Shape(int nverts, GLenum mode){
	nVertices = nverts;
	renderMode = mode;
	useColor = useTexture = useElements = false;
	model = world = glm::mat4(1.f);
	
	vertexShader = loadShader("src/shaders/shader_vertex_tex.glsl", GL_VERTEX_SHADER);
	fragmentShader = loadShader("src/shaders/shader_fragment_tex.glsl", GL_FRAGMENT_SHADER);
	CHECK_GL_ERROR();

	program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);
//	printStatus("Shader program", program, GL_LINK_STATUS);
	
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &cbo);
	glGenBuffers(1, &ebo);
	glGenBuffers(1, &tbo);
	CHECK_GL_ERROR();
	
	glUseProgram(program);
	// apply a dummy 1x1 white texture 
	unsigned char pixels[] = {255,255,255,255}; 
	glGenTextures(1, &tex);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);
	glUniform1i(glGetUniformLocation(program, "tex"), 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels); // After reading one row of texels, pointer advances to next 4 byte boundary. Therefore ALWAYS use 4byte colour types. 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glGenerateMipmap(GL_TEXTURE_2D);
	CHECK_GL_ERROR();
	
	// Put shape into global list. 
	allShapes.push_back(this);
	
	// sort the shapes by distance to camera, so that transparency is handled correctly. We are just sorting shapes, not individual triangles, and praying to god that thats good enough! 
	// Sort shapes list so that newly inserted shape gets into the correct location.
	// allShapes.sort([](Shape* &s1, Shape* &s2){ return min(s1->bbox0.z,s1->bbox1.z) < min(s2->bbox0.z,s2->bbox1.z);}); // FIXME: sort by distance from camera rather than absolute z
	if (activeCamera != NULL) activeCamera->sortShapes();
}

Shape::~Shape(){

	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteTextures(1, &tex);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &ebo);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &vbo);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &cbo);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &tbo);

	glDetachShader(program, vertexShader);
	glDetachShader(program, fragmentShader);
	glDeleteProgram(program);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	try{
		list<Shape*>::iterator it = find(allShapes.begin(), allShapes.end(), this);
		if (it == allShapes.end()) throw string("Shape destructor could not find shape in global list!");
		allShapes.erase(it);
	}
	catch(string s){ 
		cout << "FATAL ERROR: " << s << endl; 
	}
	
}

glm::vec3 Shape::getTransformedBBox0(){
	return world*model*glm::vec4(bbox0, 1.f);
}

glm::vec3 Shape::getTransformedBBox1(){
	return world*model*glm::vec4(bbox1, 1.f);
}


void Shape::setVertices(float * verts){
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 3*nVertices*sizeof(float), verts, GL_DYNAMIC_DRAW);

	bbox0 = accumulate((glm::vec3*)verts, (glm::vec3*)verts +nVertices, glm::vec3(1e20f,1e20f,1e20f),
						[](const glm::vec3& p1, const glm::vec3& p2){ return glm::vec3(min(p1.x,p2.x), min(p1.y,p2.y), min(p1.z,p2.z));});

	bbox1 = accumulate((glm::vec3*)verts, (glm::vec3*)verts +nVertices, glm::vec3(-1e20f,-1e20f,-1e20f),
						[](const glm::vec3& p1, const glm::vec3& p2){ return glm::vec3(max(p1.x,p2.x), max(p1.y,p2.y), max(p1.z,p2.z));});
	
	cout << "Shape bounding box: [" << bbox0.x << " " << bbox0.y << " " << bbox0.z << "] ["  
								    << bbox1.x << " " << bbox1.y << " " << bbox1.z << "]" << endl;
}


void Shape::setColors(float * cols){
	useColor = true;
	glBindBuffer(GL_ARRAY_BUFFER, cbo);
	glBufferData(GL_ARRAY_BUFFER, 4*nVertices*sizeof(float), cols, GL_DYNAMIC_DRAW);
}

void Shape::setElements(int * ele, int nelements){
	useElements = true;
	nElements = nelements;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, nelements*sizeof(int), ele, GL_DYNAMIC_DRAW);
}


void Shape::applyTexture(float * uvs, unsigned char * pixels, int w, int h){
	useTexture = true;
	glUseProgram(program);
	
	glBindBuffer(GL_ARRAY_BUFFER, tbo);
	glBufferData(GL_ARRAY_BUFFER, 2*nVertices*sizeof(float), uvs, GL_DYNAMIC_DRAW);
	CHECK_GL_ERROR();

//	glBindTexture(GL_TEXTURE_2D, 0);
//	glDeleteTextures(1, &tex);
//	CHECK_GL_ERROR();

//	glGenTextures(1, &tex);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);
	glUniform1i(glGetUniformLocation(program, "tex"), 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels); // After reading one row of texels, pointer advances to next 4 byte boundary. Therefore ALWAYS use 4byte colour types. 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glGenerateMipmap(GL_TEXTURE_2D);
	CHECK_GL_ERROR();

}


void Shape::autoExtent(){
	glm::vec3 size = bbox1 - bbox0;
	glm::vec3 centroid = (bbox0+bbox1)/2.f;
	float scale = 2/fmax(fmax(size.x, size.y), size.z);

	world = glm::mat4(1.f);
	model = glm::mat4(1.f);
	model = glm::scale(model, glm::vec3(scale, scale, scale));
	model = glm::translate(model, -centroid);

}

void Shape::setShaderVariable(string s, glm::mat4 f){
	GLuint loc = glGetUniformLocation(program, s.c_str());
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(f));
}

void Shape::setPointSize(float psize){
	GLuint loc = glGetUniformLocation(program, "pointsize");
	glUniform1f(loc, psize);
	CHECK_GL_ERROR();
}


void Shape::render(){
	
	if (activeCamera == NULL) {
		cout << "WARNING: No camera is active. Cannot draw." << endl;
		return;
	}
	
	glUseProgram(program);
	setShaderVariable("transform", activeCamera->matrix()*world*model);

	GLint pos_loc = glGetAttribLocation(program, "in_pos");
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(pos_loc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(pos_loc);
	CHECK_GL_ERROR();

	GLint col_loc = glGetAttribLocation(program, "in_col");
	if (useColor){
		glBindBuffer(GL_ARRAY_BUFFER, cbo);
		glVertexAttribPointer(col_loc, 4, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(col_loc);
	}
	else{
		glVertexAttrib4f(col_loc, 1, 1, 1, 1);
	}
	CHECK_GL_ERROR();
	
	GLint uv_loc = glGetAttribLocation(program, "in_UV");
	if (useTexture){
		glBindBuffer(GL_ARRAY_BUFFER, tbo);
		glVertexAttribPointer(uv_loc, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(uv_loc);
	}
	else{
		glVertexAttrib2f(uv_loc, 0, 0);
	}

	glBindTexture(GL_TEXTURE_2D, tex);

	CHECK_GL_ERROR();
	
	if (useElements) glDrawElements(renderMode, nElements, GL_UNSIGNED_INT, (void *)0);
	else             glDrawArrays(renderMode, 0, nVertices);

	if (useTexture) glDisableVertexAttribArray(uv_loc);
	if (useColor)   glDisableVertexAttribArray(col_loc);
	glDisableVertexAttribArray(pos_loc);
	CHECK_GL_ERROR();

}


//void Shape::deleteTexture(){
//	if (useTexture){
//	}
//}



