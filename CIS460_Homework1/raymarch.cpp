#include "raymarch.h"

using namespace std;

raymarch::raymarch(char* filename) {
	
	fr = new fileReader(filename);
	cam = fr->getCameraFromFile();
	int size = ((int)fr->XYZC.x*(int)fr->XYZC.y*(int)fr->XYZC.z);
	vb = new voxelBuffer(fr->voxelDensities,size,fr->XYZC.x,fr->XYZC.y,fr->XYZC.z,fr->DELT);
	vb->origin = fr->ORIG;
	kValue = -1.f * fr->KVAL;
	output.SetSize((int)fr->RESO.x,(int)fr->RESO.y);
	output.SetBitDepth(24);
	IMAGEWIDTH = fr->RESO.x;
}

// Ray = Eye + t (P-E)/|P-E|  glm::normalize(getDirectionFromCoordinate(i,j)-eye))
// r = xe + n*s
void raymarch::endMultiRayMarch() {
	char* filename = fr->FILE;
	output.WriteToFile(filename);
	cout << "Successfully wrote file named: " << filename << endl;
}
//transmittance 0 = opaque
void raymarch::calculateValues(int startingPlace) {
	/*int startIndex = 0;
	int endIndex = 0;
	if (startingPlace == 0) {
		
		startIndex = 0;
		endIndex = (fr->RESO.x)/4;
		cout << startIndex << " to " << endIndex << endl;
	}
	else if (startingPlace == 1) {
		
		startIndex = (fr->RESO.x)/4;
		endIndex = (fr->RESO.x)/2;
		cout << startIndex << " to " << endIndex << endl;
	}
	else if (startingPlace == 2) {
		
		startIndex = (fr->RESO.x)/2;
		endIndex = 3*((fr->RESO.x)/4);
		cout << startIndex << " to " << endIndex << endl;
	}
	else if (startingPlace == 3) {
		
		startIndex = 3*((fr->RESO.x)/4);
		endIndex = fr->RESO.x;
		cout << startIndex << " to " << endIndex << endl;
	}
	else {
		startIndex = 0;
		endIndex = fr->RESO.x;
		cout << startIndex << " to " << endIndex << endl;
	}
	
	// for each x */
	//for (int sx = startIndex; sx < endIndex; sx++) {
		// for each y
	float step = fr->STEP;
	int sx = startingPlace;
		for(int sy = 0; sy < fr->RESO.y; sy++) {
			glm::vec3 outColor(0.0,0.0,0.0);
			glm::vec3 direction = glm::normalize(cam->getDirectionFromCoordinate(sx,sy)-cam->eye);
			float T = 1.0;
			glm::vec3 Nrml(0,0,1);
			glm::vec3 Pnt(1,1,0);
			// for each step
			// camera z + depth of buffer / step size
			float a2 = -1 * glm::dot(direction,Nrml);
			float a1 = glm::dot(cam->eye,Nrml);
			int a3 = glm::floor(a1/a2);
			for(int t = a3; t < ((cam->eye.z + vb->depth) / (fr->STEP)); t++) {

				glm::vec3 ray = cam->eye + (direction * (t * step));
				voxel* currentV = vb->get(ray);
				if (currentV != NULL && currentV->density > 0) {
					float deltaT = exp(kValue * step * (currentV->density));
					T *= deltaT;
					if (currentV->lightValue == -1) {
						glm::vec3* loc = vb->locationOfVoxel(ray);
						currentV->lightValue = computeLightValue(loc);
					}
					outColor += (1-deltaT) * fr->MRGB * T * (currentV->lightValue * fr->LCOL);
				}
			}
			outColor += T*fr->BRGB;

			output(sx,(fr->RESO.y - 1) - sy)->Red = outColor.x * 255;
			output(sx,(fr->RESO.y - 1) - sy)->Green = outColor.y * 255;
			output(sx,(fr->RESO.y - 1) - sy)->Blue = outColor.z * 255;
		}
	//}
}

float raymarch::computeLightValue(glm::vec3* currentVoxel) {
	
	if (currentVoxel != NULL) {
		glm::vec3 voxelPosition(currentVoxel->x,currentVoxel->y,currentVoxel->z);
		glm::vec3 lightDir = (fr->LPOS - voxelPosition);
		glm::vec3 normalizedLightDir = glm::normalize(lightDir);
		int steps = glm::length(lightDir)/fr->STEP;
		float T = 1.0;
		for(int i = 0; i <= steps; i++) {
			glm::vec3 getVec = voxelPosition + (normalizedLightDir * (i * fr->STEP));
			voxel* voxelAtStep = vb->get(getVec);
			if (voxelAtStep != NULL && voxelAtStep->density > 0) {
				T *= exp(kValue * fr->STEP * (voxelAtStep->density));
			}
		}
		return T;
	}
	else {
		return -1.0;
	}
}