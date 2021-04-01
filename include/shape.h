#pragma once
#include "glad.h"
#include "glm/glm.hpp"
#include "shader.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>


class Shape {

private:

	unsigned int VBO, VAO, EBO;
	float* verticiePtr = nullptr;
	size_t verticieCount = 0;

	std::vector<glm::vec3> points;
	std::vector<unsigned int> elements;
	void DeleteExisting();

public:

	Shape();

	void Cube();
	void Sphere();

	void Draw();

};