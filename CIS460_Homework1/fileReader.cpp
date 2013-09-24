#include "fileReader.h"
using namespace std;

Camera* fileReader::getCameraFromFile() {
	Camera* newCam = new Camera();
	newCam->eye = EYEP;
	newCam->up = UVEC;
	newCam->view = VDIR;
	newCam->fovY = glm::radians(FOVY);
	newCam->fovX = glm::atan((RESO.x/RESO.y)*glm::tan(glm::radians(FOVY)));
	newCam->camSet = false;
	newCam->imgWidth = RESO.x;
	newCam->imgHeight = RESO.y;
	return newCam;
}

fileReader::fileReader(char* fileName) {
	std::string line;
	ifstream file(fileName);
	readAllAttributes = false;
	voxelDensityIndex = 0;
	ORIG = glm::vec3(0.0,0.0,0.0);
	KVAL = 1;
	while (!file.good()) {
		cout << "Invalid File Name Please Try Again: ";
		char* filename = new char[25];
		cin >> filename;
		file.open(filename);
	}
	while(!file.eof()) {
		char line[100];
		file.getline(line,100);
		if (!readAllAttributes) {
			readAttributes(line);
		}
		else {
			readVoxelDensity(line);
		}
	}
}

void fileReader::readAttributes(char* line) {
	char* attribute = strtok(line, " ");

	if (strcmp(attribute, "DELT")==0) {
		DELT = readNextFloatToken();
	}
	else if (strcmp(attribute, "STEP")==0) {
		STEP = readNextFloatToken();
	}
	else if (strcmp(attribute, "XYZC")==0) {
		XYZC = readNextVecToken();
	}
	else if (strcmp(attribute, "BRGB")==0) {
		BRGB = readNextVecToken();
	}
	else if (strcmp(attribute, "MRGB")==0) {
		MRGB = readNextVecToken();
	}
	else if (strcmp(attribute, "FILE")==0) {
		strcpy(FILE, readNextCharToken());
	}
	else if (strcmp(attribute, "RESO")==0) {
		RESO.x = readNextIntToken();
		RESO.y = readNextIntToken();
	}
	else if (strcmp(attribute, "EYEP")==0) {
		EYEP = readNextVecToken();
	}
	else if (strcmp(attribute, "VDIR")==0) {
		VDIR = readNextVecToken();
	}
	else if (strcmp(attribute, "UVEC")==0) {
		UVEC = readNextVecToken();
	}
	else if (strcmp(attribute, "FOVY")==0) {
		FOVY = readNextFloatToken();
	}
	else if (strcmp(attribute, "LPOS")==0) {
		LPOS = readNextVecToken();
	}
	else if (strcmp(attribute, "LCOL")==0) {
		LCOL = readNextVecToken();
	}
	else if (strcmp(attribute, "ORIG")==0) {
		ORIG = readNextVecToken();
	}
	else if (strcmp(attribute, "KVAL")==0) {
		KVAL = readNextFloatToken();
	}
	else {
		int size = XYZC.x * XYZC.y * XYZC.z; 
		readAllAttributes = true;
		voxelDensities = new float[size];
		readVoxelDensity(attribute);
	}
}

void fileReader::readVoxelDensity(char* line) {
	if (voxelDensityIndex < (XYZC.x * XYZC.y * XYZC.z)) {
		voxelDensities[voxelDensityIndex] = (float) atof(line);
		voxelDensityIndex++;
	}
}

char* fileReader::readNextCharToken(){
	char* token = strtok(NULL, " ");
	return token;
}

int fileReader::readNextIntToken(){
	return atoi(readNextCharToken());
}

float fileReader::readNextFloatToken(){
	return (float) atof(readNextCharToken());
}

glm::vec3 fileReader::readNextVecToken() {
	glm::vec3 newVec;
	newVec.x = readNextFloatToken();
	newVec.y = readNextFloatToken();
	newVec.z = readNextFloatToken();
	return newVec;
}