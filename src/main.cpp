#include <iostream>
#include <vector>

#include "../include/quickgl.h"
using namespace std;


	

int main(int argc, char** argv){

	// Initialize QuickGL (OpenGL initializations)
	initQuickGL(argc, argv);
	
	// Create a Controller to rotate/pan/zoom the view with mouse/touch
	CameraController c;

	// Create a Camera
	glm::vec3 pos(2.0f, 2.0f, 2.0f);
	glm::vec3 lookat(0.5f, 0.5f, 0.0f);

	Camera cam(pos, lookat, glm::vec3(0.0f, 0.0f, 1.0f));
	cam.activate();

	// Create a rainbow colour palette of 1000 colours
	Palette palette(1000);
	palette.createRainbow(); 


	// Point-cloud Analysis goes here
	

	// Create a (Demo) Point-Cloud
	vector <float> points(3*100000);
	for (int i=0; i<10000; ++i){
		points[3*i+0] = float(rand())/RAND_MAX;
		points[3*i+1] = float(rand())/RAND_MAX;
		points[3*i+2] = float(rand())/RAND_MAX;
	}
	vector <float> cols_pts = palette.mapValues(points.data(), 100000, 3, 2);
	
	// Create a Shape for point-cloud
	Shape ptcld(100000, GL_POINTS);
	ptcld.setVertices(points.data());
	ptcld.setColors(cols_pts.data());
	ptcld.autoExtent();	// Centers the point-cloud at origin

	
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//       OTHER QUICKGL TESTS
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

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
//	axis.autoExtent();
//	axis.applyTexture(UVs, pixels2, 2,2);


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



	
	
//	glm::vec3 los = pos;
	float pos_los[] = {pos.x, pos.y, pos.z, lookat.x, lookat.y, lookat.z};
	Shape lineOfSight(2, GL_LINES);
	lineOfSight.setVertices(pos_los);


//	for (int i=0; i<10000; ++i){  
	glutMainLoop();
//	usleep(100);
//	}

//	s.deleteTexture();
//	delete s;
	
	closeQuickGL();

  return 0;
}



