#pragma once

#include <glm/mat4x4.hpp>

struct ModelData {
	int VAO;
	int ArraySize;
	unsigned int TextureID = 0;
};

struct Model {
	ModelData Data;
	glm::mat4 Transformation;
};