#ifndef RAYMARCH
#define RAYMARCH

#include "Camera.h"
#include "voxelBuffer.h"
#include "fileReader.h"
#include <thread>

class raymarch {
public:
	float kValue;
	
	Camera* cam;
	//voxelBuffer* vb;
	raymarch(char* filename);
	~raymarch();
	void calculateValues(int startingplace);
	float computeLightValue(glm::vec3* currentVoxel);
	float* redOut;
	float* greenOut;
	float* blueOut;
	void endMultiRayMarch(int frame);
	BMP output;
	int IMAGEWIDTH;

	void clearBMP();

	void setUpVoxels(int t);
	//void multiThreadNoise(int start);
	//void multiThreadPyro(int start);
};

#endif