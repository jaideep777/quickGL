#include "../include/shape.h"

#include <fstream>
#include <string>
#include <iostream>
#include <algorithm>
#include <numeric>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_access.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/string_cast.hpp>

#include "../include/camera.h"
#include "../include/glinit.h"


using namespace std;


GLuint loadShader(string filename, GLenum shader_type){
	cout << "Loading shader from " << filename << endl;
	ifstream fin(filename.c_str());
	
	// FIXME: assert(fin);
	
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
	
	vertexShader = loadShader("shaders/shader_vertex.glsl", GL_VERTEX_SHADER);
	fragmentShader = loadShader("shaders/shader_fragment.glsl", GL_FRAGMENT_SHADER);
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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glGenerateMipmap(GL_TEXTURE_2D);
	CHECK_GL_ERROR();

}

void Shape::setTextureFilters(GLint minFilter, GLint magFilter){
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter); //GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter); //GL_NEAREST);
	CHECK_GL_ERROR();

}



// FIXME: use the PVM matrices to set extent
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
	
	if (!activeCamera) return;
	
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



