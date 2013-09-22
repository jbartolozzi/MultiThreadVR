#include "Camera.h"

glm::vec3 Camera::getDirectionFromCoordinate(int sx, int sy) {
	if (!camSet) { 
		setCameraState(); 
	}
	glm::vec3 P = M + (2*((float)sx/(imgWidth-1)) - 1)*H + (2*((float)sy/(imgHeight-1)) - 1)*V;
	return P;
}

void Camera::setCameraState() {
	// A = C x U
	glm::vec3 A = glm::cross(view,up);
	// B = A x C
	glm::vec3 B = glm::cross(A,view);
	// M = E + C
	M = eye + view;
	// H = (A|C|tan(theta))/|A|
	H = (A * glm::length(view) * glm::tan(fovX))/(glm::length(A));
	// V = (B|C|tan(phi))/|B|
	V = (B * glm::length(view) * glm::tan(fovY))/(glm::length(B));
	camSet = true;
}

void Camera::printImage(char* outputName) {
	BMP output;
	output.SetSize(imgWidth, imgHeight);
	output.SetBitDepth(24);
	glm::vec3 outR;
	for (int i = 0; i < imgWidth; i++) {
		for (int j = 0; j < imgHeight; j++) {
			outR = glm::abs(glm::normalize(getDirectionFromCoordinate(i,j)-eye));
			output(i,j)->Red = outR.x * 255;
			output(i,j)->Green = outR.y * 255;
			output(i,j)->Blue = outR.z * 255;
		}
	}
	output.WriteToFile(outputName);
}

Camera::Camera() {
	camSet = false;
}