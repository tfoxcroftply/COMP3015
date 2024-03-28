#pragma once

#include <glm/mat4x4.hpp>

struct ModelData {
	int VAO;
	int ArraySize;
	unsigned int TextureID = -1;
};

struct Model {
	ModelData Data;
	glm::mat4 Transformation = glm::mat4(1.0f);
};