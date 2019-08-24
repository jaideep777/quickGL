
# QuickGL

QuickGL lets you instantly add graphical visualizations to your project with just a few lines of code. It hides away all the nitty-gritties of OpenGL and lets you focus on the main idea of your project.  

## Getting started

To create a basic visualization, follow the simple steps in the code below. These steps will automagically:
1) Create a window and draw all created shapes in it.
2) Create a camera that you can zoom with scroll wheel, rotate with left click, and pan with middle or right click. On touch screens, touch to rotate and pinch to zoom. 
3) Perform all OpenGL initializations and allocations behind the scenes 

````C++
// 1. Initialize QuickGL (OpenGL initializations)
initQuickGL(argc, argv);

// 3. Create a Camera located at (2,2,2) looking at (0.5, 0.5, 0)
Camera cam(glm::vec3(2.f,2.f,2.f), glm::vec3(0.5f, 0.5f, 0.f), glm::vec3(0.0f, 0.0f, 1.0f));
cam.activate();

// 3. (Optional) Create a Controller to rotate/pan/zoom the view with mouse/touch
CameraController c;

// 4. Create an object (Shape) to be drawn, and specify its contents (vertices, vertex colours, element indices, texture coordinates, and texture image)
Shape s(n, GL_TRIANGLES); // n points, drawn as TRIANGLES
s.setVertices(vertices); // set vertices {x1,y1,z1,...,xn,yn,zn}
s.setColors(cols);	 // set vertex colours {r1,g1,b1,a1, ... , rn,gn,bn,an} 
s.setElements(indices, 6); // (Optional) set elements
s.applyTexture(tex_coords, image, image_width, image_height); // (Optional) Apply a texture. tex_coords = {u1,v1,...,un,vn}, image = an RGBA image in unsigned byte format

// 5. Create more shapes if desired
// Shape s1(...)

// 6. Start drawing!
glutMainLoop();

// Clean up
closeQuickGL();


````



## Compilation

Include the contents of the QuickGL `src` and `include` folders in your respective project folders. Compile each file into an object file and link with the objects of your project.   

## Extending QuickGL for more functionality

All classes (`Shape`, `Camera`, and `Tool`) are extendable. Custom functions for Mouse/Keyboard input can be set by extending the `Tool` class and defining the `onClick()` and `onMouseMove()` functions.

## Shape Examples

Here are a few examples of some common shapes that can be created:

1\) To create a Point-Cloud:
````C++
// Create a random 10000-point Point-Cloud
vector <float> points(3*100000);
for (int i=0; i<10000; ++i){
	points[3*i+0] = float(rand())/RAND_MAX;
	points[3*i+1] = float(rand())/RAND_MAX;
	points[3*i+2] = float(rand())/RAND_MAX;
}

// create a 1000 point colour palette
Palette palette(1000);
palette.createRainbow(); 
vector <float> cols_pts = palette.mapValues(points.data(), 100000, 3, 2); // map the z-coordinates to colours

// Create a Shape for point-cloud
Shape ptcld(100000, GL_POINTS);
ptcld.setVertices(points.data());
ptcld.setColors(cols_pts.data());  // colour by altitude
ptcld.autoExtent();	// Center the point-cloud at origin
````

2\) To create a photo frame

````C++
float pts[] = {1,1, 0,1, 0,0, 1,0}
float UVs[] = {1,1, 0,1, 0,0, 1,0}
int elements[] = {0,1,2,2,3,0}
unsigned char image[] = {0, 0,255,255, 	
			 0, 255, 0,255,
			 255,0,0,255,    
			 255,255,255,255}; // a sample 2x2 image of 4 pixels, but this could be a real image
			 
Shape s(4, GL_TRIANGLES);
s.setVertices(pts);
s.setElements(elements, 6);
s.applyTexture(UVs, image, 2,2);
 
````

3\) Create an Axis

````C++
float pos3[] = {0,0,0, 10,0,0, 0,0,0, 0,10,0, 0,0,0, 0,0,10}; // Lines from 0-10 on all 3 axes 
float col3[] = {1,0,0, 0.5,
		1,0,0, 0.5,
		0,1,0, 0.5,
		0,1,0, 0.5,
		0,0,1, 0.5,
		0,0,1, 0.5 };

Shape axis(6, GL_LINES);
axis.setVertices(pos3);
axis.setColors(col3);

````

