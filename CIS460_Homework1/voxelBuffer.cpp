#include "voxelBuffer.h"
using namespace std;

voxelBuffer::voxelBuffer(float* input, int size, int _width, int _height, int _depth, float _voxelsize){
	voxelArray = new voxel[size];
	width = _width;
	height = _height;
	depth = _depth;
	voxelSize = _voxelsize;
	for(int i = 0; i < size; i++) {
		voxelArray[i].density = input[i];
		voxelArray[i].lightValue = -1;
	}
	displacement = (depth * voxelSize);
}

voxelBuffer::voxelBuffer(int size, int _width, int _height, int _depth, float _voxelsize){
	voxelArray = new voxel[size];
	width = _width;
	height = _height;
	depth = _depth;
	voxelSize = _voxelsize;
	displacement = (depth * voxelSize);
}



// get voxel at index
voxel* voxelBuffer::at(int x, int y, int z) {
	int index = (width * y) + x + (z*(width * height));
	return &voxelArray[index];
}

// get closest voxel to float values
voxel* voxelBuffer::get(glm::vec3 ray){
	float x = ray.x - origin.x;
	float y = ray.y - origin.y;
	float z = ray.z + displacement - origin.z;
	if (x < 0 || y < 0 || z < 0 || x >= (width * voxelSize) || y >= (height * voxelSize) || z >= (depth * voxelSize)) {
		return NULL;
	}
	else {
		x = glm::floor(x/voxelSize);
		y = glm::floor(y/voxelSize);
		z = glm::floor(z/voxelSize);
		return at(x,y,z);
	}
}

glm::vec3* voxelBuffer::locationOfVoxel(glm::vec3 ray) {
	float x = ray.x - origin.x;
	float y = ray.y - origin.y;
	float z = ray.z + displacement - origin.z;
	if (x < 0 || y < 0 || z < 0 || x >= (width * voxelSize) || y >= (height * voxelSize) || z >= (depth * voxelSize)) {
		return NULL;
	}
	else {
		x = glm::floor(x/voxelSize) + origin.x;
		y = glm::floor(y/voxelSize) + origin.y;
		z = glm::floor(z/voxelSize) - displacement + origin.z;
		glm::vec3* out = new glm::vec3(x,y,z);
		return out;
	}
}

void voxelBuffer::generateSphere(int startingpoint, glm::vec3 origin, float radius, float voxelSize) {
	for(int x = startingpoint; x < width;) {
		for(int y = 0; y < height; y++) {
			for (int z = 0; z < depth; z++) {
				glm::vec3 check(x,y,z);
				float d = glm::distance(check,origin);
				if (d <= radius) {
					if (voxelArray[(width * y) + x + (z*(width * height))].density < 1) {
						voxelArray[(width * y) + x + (z*(width * height))].density += 1.f - (d/radius);
					}
					voxelArray[(width * y) + x + (z*(width * height))].lightValue = -1;
				}
			}
		}
		x+=4;
	}
}

// fbm(P) - (1-|P/radius|)
void voxelBuffer::generateNoise(int startingpoint, float t, float radius, glm::vec3 origin, float voxelSize) {
	Perlin p3 = Perlin(10,0.1,1.1,t);

	for(int x = startingpoint; x < width;) {
		if (startingpoint == 0 && x%10 == 0) {
			cout << ((float)x/(float)width)*100.f << "%, ";
		}
		for(int y = 1; y < height; y++) {
			for (int z = 1; z < depth; z++) {
				int newX = (x*voxelSize + voxelSize/2) - origin.x;
				int newY = (y*voxelSize + voxelSize/2) - origin.y;
				int newZ = (z*voxelSize - voxelSize/2) - origin.z;
				float newDensity = p3.Get(newX,newY,newZ) + (1 - glm::length(glm::vec3(newX,newY,newZ)/radius));
				voxelArray[(width * y) + x + (z*(width * height))].density += glm::max(glm::min(newDensity,1.f),0.f);
				voxelArray[(width * y) + x + (z*(width * height))].lightValue = -1;
			}
		}
		x+= 4;
	}
}

// max((radius - |P/radiu|+abs(fbm(P))),0)
void voxelBuffer::generatePyro(int startingpoint, float t, float radius, glm::vec3 origin, float voxelSize) {
	Perlin p3 = Perlin(5,0.1,1.1,t);
	for(int x = startingpoint; x < width;) {
		if (startingpoint == 0 && x%10 == 0) {
			cout << ((float)x/(float)width)*100.f << "%, ";
		}
		for(int y = 1; y < height; y++) {
			for (int z = 1; z < depth; z++) {
				int newX = (x*voxelSize + voxelSize/2) - origin.x;
				int newY = (y*voxelSize + voxelSize/2) - origin.y;
				int newZ = (z*voxelSize - voxelSize/2) - origin.z;
				
				glm::vec3 ray(newX,newY,newZ);
				float newDensity =  glm::max((radius - glm::length(ray/radius) + glm::abs(p3.Get(newX,newY,newZ))),0.f);
				voxelArray[(width * y) + x + (z*(width * height))].density += glm::max(glm::min(newDensity,1.f),0.f);
				voxelArray[(width * y) + x + (z*(width * height))].lightValue = -1;
			}
		}
		x+= 4;
	}
}

void voxelBuffer::clearBuffer() {
	for(int x = 0; x < width; x++) {
		for(int y = 0; y < height; y++) {
			for (int z = 0; z < depth; z++) {
				voxelArray[(width * y) + x + (z*(width * height))].density = 0.f;
			}
		}
	}
}