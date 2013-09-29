#ifndef VOXELBUFFER
#define VOXELBUFFER
#include "voxel.h"
#include <vector>
#include <stdio.h>
#include "perlin.h"
#include <iostream>

class voxelBuffer {
private:
	int displacement;

public:
	int depth;
	int width;
	int height;
	float voxelSize;
	glm::vec3 origin;
	
	voxelBuffer();
	voxelBuffer(float* input, int size, int _width, int _height, int _depth, float _voxelsize);
	voxelBuffer(int size, int _width, int _height, int _depth, float _voxelsize);
	voxel* at(int x, int y, int z);
	voxel* get(glm::vec3 v);
	voxel* voxelArray;
	glm::vec3* locationOfVoxel(glm::vec3 ray);

	void clearBuffer();
	void generateSphere(int startingpoint, glm::vec3 origin, float radius, float voxelSize);
	void generateNoise(int startingpoint, float t, float radius, glm::vec3 origin, float voxelSize,float pOctaves, float pFreq, float pAmp, float pSeed, int current);
	void generatePyro(int startingpoint, float t, float radius, glm::vec3 origin, float voxelSize,float pOctaves, float pFreq, float pAmp, float pSeed, int current);
};
#endif