#include <iostream>
#include <string>
#include <fstream>
#include <list>
#include <unistd.h>
#include <functional>
#include <algorithm>
#include "../include/glinit.h"
#include "../include/shape.h"
#include "../include/camera.h"
#include "../include/tool.h"
using namespace std;

//extern list <Shape*> allShapes;
//extern glm::mat4 projection;
//extern glm::mat4 view; 




	

int main(int argc, char** argv)
{

	initSimpleGL(argc, argv);
	
	float pos3[] = {0,0,0, 10,0,0, 0,0,0, 0,10,0, 0,0,0, 0,0,10};
	float col3[] = {1,0,0, 0.5,
				    1,0,0, 0.5,
				    0,1,0, 0.5,
				    0,1,0, 0.5,
				    0.0,0.8,1, 0.5,
				    0.0,0.8,1, 0.5
				   };
	


	GLfloat vertices[] = {
	   0.5f,  0.5f,  0.0f,
	  -0.5f,  0.5f,  0.0f,
	  -0.5f, -0.5f,  0.0f,
	   0.5f, -0.5f,  0.0f
	};

	// texture setup
	GLfloat UVs[] = {
	   1.0f, 1.0f,
	   0.0f, 1.0f,
	   0.0f, 0.0f,
	   1.0f, 0.0f
	};

	float cols[] = {1,1,1,0.5, 1,0,0,0.5, 0,0,1,0.5, 0,1,0,0.5 };
	
	int indices[] = { 0, 1, 2, 2,3,0 };
	unsigned char pixels2[] = {
	  0, 0,255,255, 	0, 255, 0,255,
	  255,0,0,255,    255,255,255,255
	};

	int nVertices = 4;
	int nelements = 6;

	Shape s(4, GL_TRIANGLES);
	s.setVertices(vertices);
	s.setColors(cols);
	s.setElements(indices, 6);
	s.applyTexture(UVs, pixels2, 2,2);

	Shape axis(6, GL_LINES);
	axis.setVertices(pos3);
	axis.setColors(col3);
//	axis.applyTexture(UVs, pixels2, 2,2);



	
	
//	registerCallbacks(b);

//	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);


	GLfloat vertices_f[] = {
	   0.25f,  0.25f,  0.5f,
	  -0.75f,  0.25f,  0.5f,
	  -0.75f, -0.75f,  0.5f,
	   0.25f, -0.75f,  0.5f
	};
	float cols_r[] = {1,0,0,0.5, 1,0,0,0.5, 1,0,0,0.5, 1,0,0,0.5 };

	GLfloat vertices_b[] = {
	   0.75f,  0.75f,  -0.5f,
	  -0.25f,  0.75f,  -0.5f,
	  -0.25f, -0.25f,  -0.5f,
	   0.75f, -0.25f,  -0.5f
	};
	float cols_b[] = {0,0,1,0.5, 0,0,1,0.5, 0,0,1,0.5, 0,0,1,0.5 };
	float cols_g[] = {0,1,0,0.5, 0,1,0,0.5, 0,1,0,0.5, 0,1,0,0.5 };

	unsigned char pixels3[] = {
	  255, 0,255,255, 	0, 255, 255,255, 255, 0, 255,255,
	  128,0,0,255,    255,255,0,255,     0, 255, 255,255
	};

	float cols_po[] = {0,0,1,0.5, 1,0,0,0.5, 0,1,0,0.5, 1,0,1,0.5 };

	Shape front(4, GL_TRIANGLES);
	front.setVertices(vertices_f);
	front.setColors(cols);
	front.setElements(indices, 6);
	front.applyTexture(UVs, pixels3, 3,2);

	Shape back(4, GL_TRIANGLES);
	back.setVertices(vertices_b);
	back.setColors(cols_po);
	back.setElements(indices, 6);
	back.setPointSize(2);
//	back.autoExtent();
	glm::vec3 b0 = back.getTransformedBBox0();
	glm::vec3 b1 = back.getTransformedBBox1();
//	back.applyTexture(UVs, pixels2, 2,2);
	

	cout << "Trans bounding box: [" << b0.x << " " << b0.y << " " << b0.z << "] ["  
		  						    << b1.x << " " << b1.y << " " << b1.z << "]" << endl;


	glm::vec3 pos(2.0f, 2.0f, 2.0f);
	glm::vec3 lookat(0.5f, 0.5f, 0.0f);

	
	Camera cam(pos,  // glm::vec3(1.0f, 0.5f, 2.0f), // 
				lookat,
				glm::vec3(0.0f, 0.0f, 1.0f));
	cam.activate();

	// FIXME: Creating a new unactivated Camera after disables the first camera as it occupies the head position!   
	Camera cam2(pos,  // glm::vec3(1.0f, 0.5f, 2.0f), // 
				lookat,
				glm::vec3(0.0f, 1.0f, 0.0f));
//	cam2.activate();

	InteractiveCameraTool camMover;

	cam2.activate();
	InteractiveCameraTool camMover2;
	
	cam.activate();
	
//	glm::vec3 los = pos;
	float pos_los[] = {pos.x, pos.y, pos.z, lookat.x, lookat.y, lookat.z};
	Shape lineOfSight(2, GL_LINES);
	lineOfSight.setVertices(pos_los);


//	for (int i=0; i<10000; ++i){  
//	glutMainLoop();
//	usleep(100);
//	}

//	s.deleteTexture();
//	delete s;
	
	closeSimpleGL();

  return 0;
}



