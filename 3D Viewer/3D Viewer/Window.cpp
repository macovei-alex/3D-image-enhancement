#include "Window.h"

Window::Window(std::unique_ptr<ShaderProgram>&& modelShader,
	std::unique_ptr<ShaderProgram>&& lightShader,
	std::unique_ptr<Model>&& model,
	std::unique_ptr<LightSource>&& lightSource)
	: modelShaders(std::move(modelShader)), 
	lightingShaders(std::move(lightShader)),
	model(std::move(model)), 
	lightSource(std::move(lightSource))
{

}

void Window::InitializeGraphics()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // culoarea de fond a ecranului
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glDisable(GL_LIGHTING);

	// Eliminam cullingul deoarece modelele vor avea doar fata exterioara
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
}

void Window::InitializeWindow()
{
	// glfw: initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw window creation
	window = std::make_unique<GLFWwindow>(glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "3D Model Viewer", NULL, NULL));
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		throw std::runtime_error("Failed to create GLFW window");
	}

	glfwMakeContextCurrent(window.get());
	glfwSetKeyCallback(window.get(), KeyCallback);
	glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
	glfwSetCursorPosCallback(window, MouseCallback);
	glfwSetScrollCallback(window, ScrollCallback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);

	glewInit();
	
	this->window = std::make_unique<GLFWwindow>(*window);
}
