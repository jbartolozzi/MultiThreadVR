#ifndef RAYMARCH
#define RAYMARCH

#include "Camera.h"
#include "voxelBuffer.h"
#include "fileReader.h"

class raymarch {
public:
	float kValue;
	fileReader* fr;
	Camera* cam;
	voxelBuffer* vb;
	raymarch(char* filename);
	void calculateValues(int startingplace);
	float computeLightValue(glm::vec3* currentVoxel);
	float* redOut;
	float* greenOut;
	float* blueOut;
	void endMultiRayMarch();
	BMP output;
	int IMAGEWIDTH;
	float calculateInterpolationWeights(glm::vec3 initial);
};

#endif