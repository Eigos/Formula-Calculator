#pragma once
#include "glad.h"
#include "GLFW/glfw3.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "formul.h"
#include "shader.h"
#include "shape.h"
#include "camera.h"

#include <vector>
#include <iostream>
#include <functional>


class RenderOpenGL {

private:

	const unsigned int SCR_WIDTH = 800;
	const unsigned int SCR_HEIGHT = 600;
	int shaderProgram = 0;
	GLFWwindow* window;
	Shader* shader;
	std::vector<glm::vec3> NormalPoints;

	void InitWindow();
	void MainLoop();
	void PlaneNormal();


public:
	RenderOpenGL();
	RenderOpenGL(std::vector<float>x);
	RenderOpenGL(std::vector<float>x, std::vector<float>y);
	RenderOpenGL(std::vector<float>x, std::vector<float>y, std::vector<float>z);
	RenderOpenGL(Formul& formul);

};
