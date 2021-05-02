#include "graph_render.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// camera
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

bool firstMouse = true;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;


void RenderOpenGL::InitWindow() {
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);



	this->window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Graph", NULL, NULL);
	if (window == NULL)
	{
		//std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return;
	}


	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return;
	}


	int width = SCR_WIDTH;
	int height = SCR_HEIGHT;
	glfwGetFramebufferSize(window, &width, &height);

	//void(__thiscall RenderOpenGL:: * pFunc)(GLFWwindow*,int, int) = &RenderOpenGL::framebuffer_size_callback;
	//void* funcp = (void*&)pFunc;

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glEnable(GL_DEPTH_TEST);

}

void RenderOpenGL::MainLoop() {


	shader = new Shader("shaders\\testCameraShader.vert", "shaders\\testCameraShader.frag");
	//shader = new Shader("shaders\\testColoredShader.vert", "shaders\\testColoredShader.frag");

	shader->use();

	Shape shape;
	shape.Cube();

	//glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	//glPointSize(5.f);

	
	while (!glfwWindowShouldClose(window))
	{

		processInput(window);

		glClearColor(0.f, 0.f, 0.f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 model = glm::mat4(1.0f);



		shader->setMat4("projection", projection);
		shader->setMat4("view", view);
		shader->setMat4("model", model);

		shape.Draw();


		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
}

RenderOpenGL::RenderOpenGL()
{
	InitWindow();
	MainLoop();
}

void PlaneNormal() {

	float x = 0, y = 0, z = 0;

	


	for (float xx = 0; xx < 100; xx++) {
		for (float yy = 0; yy < 100; yy++) {
			//NormalPoints.push_back(glm::vec3(xx, yy, 0.0f));
			//NormalPoints.push_back(glm::vec3(xx, 100.0f, 0.0f));
		}

	}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	float cameraSpeed = 2.5 * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}