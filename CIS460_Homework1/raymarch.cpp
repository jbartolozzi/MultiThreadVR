#include "raymarch.h"

using namespace std;
voxelBuffer* vb;
void multiThreadSphere(int start);
void multiThreadNoise(int start);
void multiThreadPyro(int start);
fileReader* fr;
int currentObjectNum;
float TIMER;

raymarch::raymarch(char* filename) {
	
	fr = new fileReader(filename);
	cam = fr->getCameraFromFile();
	int size = ((int)fr->XYZC.x*(int)fr->XYZC.y*(int)fr->XYZC.z);
	//vb = new voxelBuffer(fr->voxelDensities,size,fr->XYZC.x,fr->XYZC.y,fr->XYZC.z,fr->DELT);
	vb = new voxelBuffer(size,fr->XYZC.x,fr->XYZC.y,fr->XYZC.z,fr->DELT);
	vb->origin = fr->ORIG;
	kValue = -1.f * fr->KVAL;
	output.SetSize((int)fr->RESO.x,(int)fr->RESO.y);
	output.SetBitDepth(24);
	IMAGEWIDTH = fr->RESO.x;
}

void multiThreadNoise(int start) {
	vb->generateNoise(start,TIMER,fr->objRadius[currentObjectNum],fr->objCenter[currentObjectNum],fr->DELT);
}

void multiThreadSphere(int start) {
	vb->generateSphere(start,fr->objCenter[currentObjectNum],fr->objRadius[currentObjectNum],fr->DELT);
}

void multiThreadPyro(int start) {
	vb->generatePyro(start,TIMER,fr->objRadius[currentObjectNum],fr->objCenter[currentObjectNum],fr->DELT);
}

void raymarch::setUpVoxels(int t) {
	TIMER = (float) t;
	
	for (currentObjectNum = 0; currentObjectNum < fr->objNum; currentObjectNum++) {
		if (fr->objTypes[currentObjectNum] == fr->SPHERE) {
			cout << "Creating  Sphere: ";
			thread t1(multiThreadSphere,0);
			thread t2(multiThreadSphere,1);
			thread t3(multiThreadSphere,2);
			thread t4(multiThreadSphere,3);
			t1.join();
			t2.join();
			t3.join();
			t4.join();
			cout << "100%" << endl;
		}
		else if (fr->objTypes[currentObjectNum] == fr->CLOUD) {
			cout << "Creating  Cloud: ";
			thread t5(multiThreadNoise,0);
			thread t6(multiThreadNoise,1);
			thread t7(multiThreadNoise,2);
			thread t8(multiThreadNoise,3);
			t5.join();
			t6.join();
			t7.join();
			t8.join();
			cout << "100%" << endl;
		}
		else if (fr->objTypes[currentObjectNum] == fr->PYRO) {
			cout << "Creating  Pyroclastic: ";
			thread t9(multiThreadPyro,0);
			thread t10(multiThreadPyro,1);
			thread t11(multiThreadPyro,2);
			thread t12(multiThreadPyro,3);
			t9.join();
			t10.join();
			t11.join();
			t12.join();
			cout << "100%" << endl;
		}
	}
}

// Ray = Eye + t (P-E)/|P-E|  glm::normalize(getDirectionFromCoordinate(i,j)-eye))
// r = xe + n*s
void raymarch::endMultiRayMarch(int frame) {
	char* filename = fr->FILE;
	int size = strlen(filename)+4+frame;
	char* str = new char[size];
	char integer_string[32];
	sprintf(integer_string, "%d", frame);
	strcpy (str,filename);
	strcat (str,integer_string);
	strcat (str,".bmp");
	output.WriteToFile(str);
	cout << "Successfully wrote file named: " << str << endl;
}

void raymarch::clearBMP() {
	for (int sx = 0; sx < fr->RESO.x; sx++) {
		for (int sy = 0; sy < fr->RESO.y; sy++) {
			output(sx,(fr->RESO.y - 1) - sy)->Red = 0;
			output(sx,(fr->RESO.y - 1) - sy)->Green = 0;
			output(sx,(fr->RESO.y - 1) - sy)->Blue = 0;
		}
	}
	vb->clearBuffer();
}
//transmittance 0 = opaque
void raymarch::calculateValues(int startingPlace) {
	
	float step = fr->STEP;
	for (int sx = startingPlace; sx < fr->RESO.x;) {
		if (startingPlace == 0 && sx%200 == 0) {
			cout << 100*((float)sx / (float)fr->RESO.x) << "%, ";
		}
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
		sx+=8;
	}
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

raymarch::~raymarch() {
	delete fr;
	delete vb;
}