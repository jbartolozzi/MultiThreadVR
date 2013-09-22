#ifndef CAMERA
#define CAMERA

#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/gtx/transform.hpp"
#include "EasyBMP.h"

class Camera {
private:
	glm::vec3 M;
	glm::vec3 H;
	glm::vec3 V;
public:
	bool camSet;
	glm::vec3 up;
	glm::vec3 eye;
	glm::vec3 view;
	float fovX;
	float fovY;
	int imgWidth;
	int imgHeight;

	glm::vec3 getDirectionFromCoordinate(int sx, int sy);
	void setCameraState();
	void printImage(char* outputName);
	Camera();
};

#endif