#pragma once

#include <glm/mat4x4.hpp>

struct ModelData {
	int VAO;
	int ArraySize;
};

struct Model {
	ModelData Data;
	glm::mat4 Transformation;
};