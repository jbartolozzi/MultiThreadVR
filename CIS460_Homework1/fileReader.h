#ifndef FILEREADER
#define FILEREADER
#include "../glm/glm.hpp"
#include "Camera.h"
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>


class fileReader {
public:
	float DELT;		// Size of voxel in floating point
	float STEP;		// the step size
	glm::vec3 XYZC; // number of voxels wide, tall, deep
	glm::vec3 BRGB; // background color in floating point
	glm::vec3 MRGB; // material RGB value in floating point
	char FILE[100];	// output file name
	glm::vec2 RESO; // two integers representing width and height of raytrace area
	glm::vec3 EYEP; // the position of eye in world space
	glm::vec3 VDIR; // viewing direction of center of rendering
	glm::vec3 UVEC; // the up vector
	float FOVY;		// half angle of view in Y direction 
	glm::vec3 LPOS;	// position of light in world space
	glm::vec3 LCOL;	// color of point light in floating point
	glm::vec3 ORIG; // the origin of the voxel buffer

	int voxelDensityIndex;
	bool readAllAttributes;
	float* voxelDensities;

	fileReader(char* fileName);
	void readAttributes(char* line);
	void readVoxelDensity(char* line);
	Camera* getCameraFromFile();

	char* readNextCharToken();
	int readNextIntToken();
	float readNextFloatToken();
	glm::vec3 readNextVecToken();
};
#endif