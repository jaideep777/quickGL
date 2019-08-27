#ifndef QUICKGL_PALETTE_H_
#define QUICKGL_PALETTE_H_

#include <glm/glm.hpp>
#include <vector>

class Palette{
	public:
	int n;
	std::vector <glm::vec4> colors;

	Palette(int n);
	void createRainbow(float start=0, float end=0.75);
	void createRandom(float start=0, float end=1);
	void createGrayscale(float start=0, float end=1);
	void createRamp(glm::vec4 start, glm::vec4 end);	
	void print();
	std::vector <float> mapValues(float * v, int nval, int stride = 1, int offset = 0, float vmin = 1e20, float vmax = 1e20);
	std::vector <unsigned char> mapValues_byte(float * v, int nval, int stride = 1, int offset = 0, float vmin = 1e20, float vmax = 1e20);
};



#endif 


