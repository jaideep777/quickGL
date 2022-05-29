#include <iostream>
#include <vector>
#include <cmath>

#include "../include/quickgl.h"
using namespace std;

// To compile:
// g++ -I../include -o 1 tree.cpp -L../lib -lquickgl -lglfw

struct Vertex
{
    Vertex(const glm::vec3& position, const glm::vec3& normal)
        : position(position)
        , normal(normal) {
    }
    glm::vec3 position;
    glm::vec3 normal;
};


// spin the pts array around the Z axis.
// pts.x will become the radius, and pts.y will become the height
// pts should be sorted by y-coordinate
class RotatedShape : public Shape{
	public:
	vector<glm::vec2> curve_pts; 
	unsigned int segments = 32;
    vector<glm::vec2> circlePts;

	public:
	using Shape::Shape; // inherit constructor from base Shape
	
	void createPoints(const vector<glm::vec2>& pts, unsigned int segments = 32){
		curve_pts = pts;
		
		vector <float> points;
		// precalculate circle points
		for(unsigned int i = 0; i <= segments; ++i){
		    float angle = ( i / (float)segments ) * 3.14159f * 2.0f;
		    circlePts.push_back(glm::vec2(cos(angle), sin(angle)));
		}

		// fill each layer
		typedef vector<glm::vec3> Layer;
		typedef vector<Layer> Layers;
		Layers layers(pts.size(), Layer(circlePts.size()));
		for(size_t i = 0; i < pts.size(); ++i){
		    for(unsigned int j = 0; j < circlePts.size(); ++j){
		        layers[i][j] = glm::vec3(circlePts[j] * pts[i].x, pts[i].y);
		    }
		}

		// move through layers generating triangles
		//vector<Vertex> verts;
		vector<float> verts;
		vector<float> normals;
		for(size_t i = 1; i < layers.size(); ++i){
		    const Layer& prvLayer = layers[i-1];
		    const Layer& curLayer = layers[i-0];
		    
		    for(size_t j = 1; j < circlePts.size(); ++j){
		        //    upper = cur layer
		        //        UL -- UR  
		        // left   | 0 /  |  right 
		        // = j-1  |  / 1 |  = j-0
		        //        LL -- LR  
		        //    lower = prv layer
		        const glm::vec3& LL = prvLayer[j-1]; // lower-left
		        const glm::vec3& LR = prvLayer[j-0]; // lower-right
		        const glm::vec3& UL = curLayer[j-1]; // upper-left
		        const glm::vec3& UR = curLayer[j-0]; // upper-right

		        // triangle0: LL -> UR -> UL
		        const glm::vec3 normal0 = glm::normalize(glm::cross(UR - LL, UL - LL));
		        verts.push_back(LL.x); verts.push_back(LL.y); verts.push_back(LL.z);
		        verts.push_back(UR.x); verts.push_back(UR.y); verts.push_back(UR.z);
		        verts.push_back(UL.x); verts.push_back(UL.y); verts.push_back(UL.z);
		        normals.push_back(normal0.x); normals.push_back(normal0.y); normals.push_back(normal0.z);
		        normals.push_back(normal0.x); normals.push_back(normal0.y); normals.push_back(normal0.z);
		        normals.push_back(normal0.x); normals.push_back(normal0.y); normals.push_back(normal0.z);
//		        verts.push_back(Vertex(LL, normal0));
//		        verts.push_back(Vertex(UR, normal0));
//		        verts.push_back(Vertex(UL, normal0));

		        // triangle1: LL -> LR -> UR
		        const glm::vec3 normal1 = glm::normalize(glm::cross(LR - LL, UL - LL));
		        verts.push_back(LL.x); verts.push_back(LL.y); verts.push_back(LL.z);
		        verts.push_back(LR.x); verts.push_back(LR.y); verts.push_back(LR.z);
		        verts.push_back(UR.x); verts.push_back(UR.y); verts.push_back(UR.z);
		        normals.push_back(normal1.x); normals.push_back(normal1.y); normals.push_back(normal1.z);
		        normals.push_back(normal1.x); normals.push_back(normal1.y); normals.push_back(normal1.z);
		        normals.push_back(normal1.x); normals.push_back(normal1.y); normals.push_back(normal1.z);
//		        verts.push_back(Vertex(LL, normal1));
//		        verts.push_back(Vertex(LR, normal1));
//		        verts.push_back(Vertex(UR, normal1));
		    }
		}
		
		setVertices(verts.data());
		setNormals(normals.data());
	}
};



int main(int argc, char** argv){

	// Initialize QuickGL (OpenGL initializations)
	initQuickGL(argc, argv);
	
	// Create a Controller to rotate/pan/zoom the view with mouse/touch
	CameraController c;
//	Tool t;
	
	// Create a Camera
	glm::vec3 pos(2.0f, 2.0f, 2.0f);
	glm::vec3 lookat(0.5f, 0.5f, 0.0f);

	Camera cam(pos, lookat, glm::vec3(0.0f, 0.0f, 1.0f));
	cam.activate();

	// Create a rainbow colour palette of 1000 colours
	Palette palette(1000);
	palette.createRainbow(); 


	// Point-cloud Analysis goes here
	

	vector<glm::vec2> curve = {glm::vec2(1,0), glm::vec2(0.5,0.5), glm::vec2(1,1)};
	RotatedShape trunk(32*3*4, GL_TRIANGLES);
	trunk.createPoints(curve);
	trunk.color = glm::vec4(.4,.2,0,1);
//	ptcld.setColors(cols_pts.data());
	trunk.autoExtent();	// Centers the point-cloud at origin

	
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
	

	Shape axis(6, GL_LINES);
	axis.setVertices(pos3);
	axis.setColors(col3);
//	axis.autoExtent();
//	axis.applyTexture(UVs, pixels2, 2,2);

	
	
//	glm::vec3 los = pos;
	float pos_los[] = {pos.x, pos.y, pos.z, lookat.x, lookat.y, lookat.z};
	Shape lineOfSight(2, GL_LINES);
	lineOfSight.setVertices(pos_los);



    while (windowIsOpen())
    {
		onDisplay();
//		usleep(10000);

		glfwWaitEvents();
	}

	
	closeQuickGL();

  return 0;
}



