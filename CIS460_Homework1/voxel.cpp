#include "voxel.h"

voxel::voxel(float _density, float _lightvalue) {
	this->density = _density;
	this->lightValue = _lightvalue;
}

voxel::voxel(){
	this->density = 0.f;
	this->lightValue = 0.f;
}