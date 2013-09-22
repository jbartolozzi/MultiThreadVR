#ifndef VOXEL
#define VOXEL

#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/gtx/transform.hpp"
#include "../glm/glm.hpp"

class voxel {
public:
	voxel();
	voxel(float _density, float _lightvalue);
	float density;
	float lightValue;
};
#endif