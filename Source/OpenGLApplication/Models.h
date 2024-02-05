#pragma once

#include <GL/glew.h>
#include <string>

class Model {
private:
	int VAO;

public:
	Model() {}

	bool SetModel(std::string InputPath);
	int GetVAO();
};