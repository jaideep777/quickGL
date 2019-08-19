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




class Tool{
	protected:
	bool lClick, mClick, rClick;	// whether any mouse button was clicked
	int x0, y0;						// mouse coordinates when any button was clicked
	
	public:
	Tool(){
		activeTools.push_front(this);
	}

	virtual ~Tool(){
		try{
			list<Tool*>::iterator it = find(activeTools.begin(), activeTools.end(), this);
			if (it == activeTools.end()) throw string("Tool destructor could not find tool in active list!");
			activeTools.erase(it);
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

class CameraTransformer : public Tool{
	private:
	Camera * affectedCam;

	public:
	CameraTransformer(Camera * C) : Tool(){
		affectedCam = C;
	}

//	virtual void onClick(int button, int state, int x, int y){
//		cout << "Clicked by Tool::CameraTransformer at: " << x << " " << y << endl;
//		captureClick(button, state, x,y);		
//	}
	
	virtual void onMouseMove(int x, int y){
		float h = glutGet(GLUT_WINDOW_HEIGHT);
		float w = glutGet(GLUT_WINDOW_WIDTH);
		if (lClick == 1){
			affectedCam->rx += 0.2*(y - y0);
			affectedCam->ry += 0.2*(x - x0);
		}
		if (rClick == 1){
			float r = (y - y0)/h;
			affectedCam->sc *= 1+r;
		}
		if (mClick == 1){
			affectedCam->ty -= (h/2.5)*(y - y0)/h;	// -= because y is measured from top
			affectedCam->tx += (w/2.5)*(x - x0)/w;
		}
		y0 = y;
		x0 = x;
		
		glm::mat4 view = affectedCam->view0;
		if (affectedCam->worldUp == glm::vec3(0.f, 1.f, 0.f)){
		  	view = glm::translate(view, glm::vec3(affectedCam->tx, affectedCam->ty, 0));
		  	view = glm::scale(view, glm::vec3(affectedCam->sc, affectedCam->sc, affectedCam->sc));
		  	view = glm::rotate(view, affectedCam->rx*0.1f, glm::vec3(1.f, 0.f, 0.f));
		  	view = glm::rotate(view, affectedCam->ry*0.1f, glm::vec3(0.f, 1.f, 0.f));
		}
		else if (affectedCam->worldUp == glm::vec3(0.f, 0.f, 1.f)){
		  	view = glm::translate(view, glm::vec3(affectedCam->tx, 0, affectedCam->ty));
		  	view = glm::scale(view, glm::vec3(affectedCam->sc, affectedCam->sc, affectedCam->sc));
		  	view = glm::rotate(view, affectedCam->rx*0.1f, glm::vec3(1.f, 0.f, 0.f));
		  	view = glm::rotate(view, affectedCam->ry*0.1f, glm::vec3(0.f, 0.f, 1.f));	// FIXME: second rotation should be around an axis in the screen plane, so that worldUp remains vertical. 
		}
		affectedCam->view = view;
//		cout << affectedCam->rx << endl;
		
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

  glewExperimental = GL_TRUE;
  glewInit();


  // BUFFERS etc

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	
	float pos3[] = {0,0,0, 100,0,0, 0,0,0, 0,100,0, 0,0,0, 0,0,100};
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


	Shape front(4, GL_TRIANGLES);
	front.setVertices(vertices_f);
	front.setColors(cols);
	front.setElements(indices, 6);
	front.applyTexture(UVs, pixels3, 3,2);

	Shape back(4, GL_TRIANGLES);
	back.setVertices(vertices_b);
	back.setColors(cols_b);
	back.setElements(indices, 6);
//	back.autoExtent();
	glm::vec3 b0 = back.getTransformedBBox0();
	glm::vec3 b1 = back.getTransformedBBox1();
//	back.applyTexture(UVs, pixels2, 2,2);
	

	cout << "Trans bounding box: [" << b0.x << " " << b0.y << " " << b0.z << "] ["  
		  						    << b1.x << " " << b1.y << " " << b1.z << "]" << endl;




//	

//	cam.projection = glm::perspective(glm::radians(90.0f), float(width) / height, 0.1f, 1000.0f);
	Camera cam(glm::vec3(0.0f, -2.0f, 0.0f),  //glm::vec3(1.0f, 0.5f, 2.0f), 
				glm::vec3(0.0f, 0.0f, 0.0f), 
				glm::vec3(0.0f, 0.0f, 1.0f));
	Shape::activeCamera = &cam;
	CameraTransformer camTrans(&cam);

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



