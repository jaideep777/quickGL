#include <iostream>
#include <string>
#include <fstream>
#include <list>
#include <unistd.h>
#include <functional>
#include <algorithm>
#include "../include/shape.h"
using namespace std;

//extern list <Shape*> allShapes;
//extern glm::mat4 projection;
//extern glm::mat4 view; 

GLuint vao;

int width = 320;
int height = 240;


// SimpleGL follows the principle of creating all objects (including cameras) aligned to axes and then using the matrices to get then in position

class Tool{
	protected:
	bool lClick, mClick, rClick;	// whether any mouse button was clicked
	bool wheelUp, wheelDown;
	int x0, y0;						// mouse coordinates when any button was clicked
	
	public:
	Tool(){
		mClick = lClick = rClick = 0;
		activeTools.push_front(this);
		cout << "Constructing tool #" << activeTools.size() << ", " << activeTools.size() << " in list" << endl;
	}

	virtual ~Tool(){
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
	
	void captureClick(int button, int state, int x, int y){
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
	
	virtual void onClick(int button, int state, int x, int y){
		captureClick(button, state, x,y);		
	}
	
	virtual void onMouseMove(int x, int y){};
	
	static list <Tool*> activeTools;
};

list <Tool*> Tool::activeTools;

class InteractiveCamera : public Tool{
	public:
	Camera * cam;

	public:
	InteractiveCamera(glm::vec3 _position, glm::vec3 _lookingAt, glm::vec3 _worldUp) : Tool() {
		cout << "Constructing ICam" << endl;
		cam = new Camera(_position, _lookingAt, _worldUp);
	}

	virtual ~InteractiveCamera(){
		delete cam;
		cout << "Destroying ICam" << endl;
	}
	
	void activate(){
		cam->activate();
	}
	
	virtual void onClick(int button, int state, int x, int y){
//		cout << "Clicked by Tool::CameraTransformer at: " << x << " " << y << endl;
		captureClick(button, state, x,y);		

		if (button == 3 && state == GLUT_DOWN){	// wheel down / pinch out
			cam->sc *= 1+0.1;
		
		}
		if (button == 4 && state == GLUT_DOWN){	// wheel up / pinch in
			cam->sc *= 1-0.1;
		}

		cam->transform();
		
		glutPostRedisplay();
	}
	
		
	virtual void onMouseMove(int x, int y){
		float h = glutGet(GLUT_WINDOW_HEIGHT);
		float w = glutGet(GLUT_WINDOW_WIDTH);

		if (lClick){
			cam->rx += 0.02*(y - y0);
			cam->ry += 0.02*(x - x0);
		}
//		if (rClick){
//			float r = (y - y0)/h;
//			cam->sc *= 1+r;
//		}
		if (mClick || rClick){
			cam->ty -= (h/250.f)*(y - y0)/h;	// -= because y is measured from top
			cam->tx += (w/250.f)*(x - x0)/w;
		}
		y0 = y;
		x0 = x;
	
		cam->transform();
		
		glutPostRedisplay();
	}
	
};



	
void onDisplay(void)
{


  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (auto it : Shape::allShapes) it->render();	
	  
  glutSwapBuffers();
}

void onResize(int w, int h)
{
  width = w; height = h;
  glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}


void onClick(int button, int state, int x, int y){
	if (!Tool::activeTools.empty())	Tool::activeTools.front()->onClick(button, state, x, y);
}

void onMouseMove(int x, int y){
	if (!Tool::activeTools.empty())	Tool::activeTools.front()->onMouseMove(x, y);
}

int main(int argc, char** argv)
{

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);	
  glutInitWindowSize(width, height);

  glutCreateWindow("mini");

	
 	glEnable(GL_BLEND);
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
//	glBlendFunc( GL_ONE, GL_ONE );

	glEnable(GL_DEPTH_TEST);

    glEnable(GL_PROGRAM_POINT_SIZE);

  glewExperimental = GL_TRUE;
  glewInit();


  // BUFFERS etc

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	
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



  glutDisplayFunc(onDisplay);
  glutReshapeFunc(onResize);
	glutMouseFunc(onClick);
	glutMotionFunc(onMouseMove);
	
	
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
	glm::vec3 lookat(2.0f, 2.0f, 0.0f);

	
	InteractiveCamera cam(pos,  // glm::vec3(1.0f, 0.5f, 2.0f), // 
				lookat,
				glm::vec3(0.0f, 0.0f, 1.0f));
	cam.activate();

	InteractiveCamera cam2(pos,  // glm::vec3(1.0f, 0.5f, 2.0f), // 
				lookat,
				glm::vec3(0.0f, 1.0f, 0.0f));
	cam2.activate();


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
	

  glBindVertexArray(0);
  glDeleteVertexArrays(1, &vao);

  return 0;
}



