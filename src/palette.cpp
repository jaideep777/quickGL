#include "../include/palette.h"

#include <iostream>
using namespace std;

// ===========================================================
// class Palette 
// ===========================================================

glm::vec4 HSVtoRGB(float h, float s, float v ){
	glm::vec4 C;
	C.a = 1;
	
	if( s == 0 ) {
		// achromatic (grey)
		C.r = C.g = C.b = v;
		return C;
	}

	h = h*360.f/60.f;			// sector 0 to 5
	int i = int(h);
	float f = h - i;			// factorial part of h
	float p = v * ( 1 - s );
	float q = v * ( 1 - s * f );
	float t = v * ( 1 - s * ( 1 - f ) );

	switch( i ) {
		case 0:
			C.r = v;
			C.g = t;
			C.b = p;
			break;
		case 1:
			C.r = q;
			C.g = v;
			C.b = p;
			break;
		case 2:
			C.r = p;
			C.g = v;
			C.b = t;
			break;
		case 3:
			C.r = p;
			C.g = q;
			C.b = v;
			break;
		case 4:
			C.r = t;
			C.g = p;
			C.b = v;
			break;
		default:		// case 5:
			C.r = v;
			C.g = p;
			C.b = q;
			break;
	}

	return C;
}


Palette::Palette(int _n){
	n = _n;
	colors.resize(n);
}


void Palette::createRainbow(float start, float end){
	for (int i=0; i<n; ++i){
		float h = start + i*(end-start)/n; float s=1, v=1;
		colors[i] = HSVtoRGB(h,s,v);
	}
}


void Palette::createRandom(float start, float end){
	for (int i=0; i<n; ++i){
		colors[i].r = start + (end-start)*rand()/RAND_MAX;
		colors[i].g = start + (end-start)*rand()/RAND_MAX;
		colors[i].b = start + (end-start)*rand()/RAND_MAX;
		colors[i].a = 1;
	}
}


void Palette::createGrayscale(float start, float end){
	for (int i=0; i<n; ++i){
		colors[i].r = start + (end-start)*float(i)/(n-1);
		colors[i].g = start + (end-start)*float(i)/(n-1);
		colors[i].b = start + (end-start)*float(i)/(n-1);
		colors[i].a = 1;
	}
}


void Palette::createRamp(glm::vec4 start, glm::vec4 end){
	for (int i=0; i<n; ++i){
		colors[i].r = start.r + (end.r-start.r)*float(i)/(n-1);
		colors[i].g = start.g + (end.g-start.g)*float(i)/(n-1);
		colors[i].b = start.b + (end.b-start.b)*float(i)/(n-1);
		colors[i].a = 1;
	}
}


void Palette::print(){
	cout << "palette:\n";
	for (int i=0; i<n; ++i){
		glm::vec4 c = colors[i];
		cout << int(c.r*255) << "\t" << int(c.g*255) << "\t" << int(c.b*255) << "\n";
	}
	cout << '\n';
}


// WORKS for 2D and 3D
vector <float> Palette::mapValues(float* v, int nval, int stride, int offset, float vmin, float vmax){
	vector <float> cols(4*nval);
	
	float min_val = v[0], max_val = v[0];
//	double mean = v[0];
	for (int i=1; i < nval; ++i){
		min_val = min(min_val, v[stride*i+offset]);
		max_val = max(max_val, v[stride*i+offset]);
//		mean += v[stride*i+offset];
	}
//	mean /= nval;
	
	if (vmin != 1e20f) min_val = vmin; 
	if (vmax != 1e20f) max_val = vmax;

	cout << "minmax:" <<  min_val << " " << max_val << endl;
//	cout << "mean:" <<  mean << endl;
//	colMax = max(fabs(colMax), fabs(colMin));
//	cout << "nCol = " << nCol << ", colMax = " << colMax << ", colMin = " << colMin << '\n';
	for (int i=0; i < nval; ++i) {
		glm::vec4 c;
		int colID = (v[stride*i+offset] - min_val)/(max_val-min_val)*(n-1);
//		cout << v[i*stride+offset] << endl;
//		cout << "colid = " << colID << endl;
		if (colID < 0 || colID > n-1){
			cols[4*i+0] = 0;	// if color is out of range, return black
			cols[4*i+1] = 0;
			cols[4*i+2] = 0;
			cols[4*i+3] = 0;
		}
		else{
			cols[4*i+0] = colors[colID].r;	// else return color from palette
			cols[4*i+1] = colors[colID].g;
			cols[4*i+2] = colors[colID].b;
			cols[4*i+3] = colors[colID].a;
		}
	}
	return cols;
}

// WORKS for 2D and 3D
vector <unsigned char> Palette::mapValues_byte(float* v, int nval, int stride, int offset, float vmin, float vmax){
	vector <unsigned char> cols(4*nval);
	
	float min_val = v[0], max_val = v[0];
//	double mean = v[0];
	for (int i=1; i < nval; ++i){
		min_val = min(min_val, v[stride*i+offset]);
		max_val = max(max_val, v[stride*i+offset]);
//		mean += v[stride*i+offset];
	}
//	mean /= nval;
	
	if (vmin != 1e20f) min_val = vmin; 
	if (vmax != 1e20f) max_val = vmax;

	cout << "minmax:" <<  min_val << " " << max_val << endl;
//	cout << "mean:" <<  mean << endl;
//	colMax = max(fabs(colMax), fabs(colMin));
//	cout << "nCol = " << nCol << ", colMax = " << colMax << ", colMin = " << colMin << '\n';
	for (int i=0; i < nval; ++i) {
		glm::vec4 c;
		int colID = (v[stride*i+offset] - min_val)/(max_val-min_val)*(n-1);
//		cout << v[i*stride+offset] << endl;
//		cout << "colid = " << colID << endl;
		if (colID < 0 || colID > n-1){
			cols[4*i+0] = 0;	// if color is out of range, return black
			cols[4*i+1] = 0;
			cols[4*i+2] = 0;
			cols[4*i+3] = 0;
		}
		else{
			cols[4*i+0] = 255*colors[colID].r;	// else return color from palette
			cols[4*i+1] = 255*colors[colID].g;
			cols[4*i+2] = 255*colors[colID].b;
			cols[4*i+3] = 255*colors[colID].a;
		}
	}
	return cols;
}


