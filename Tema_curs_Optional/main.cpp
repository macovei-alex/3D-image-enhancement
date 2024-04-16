#define GLEW_STATIC
#include <GL/glew.h>

#include <gtc/type_ptr.hpp>

#include <glfw3.h>

#include "Camera.h"
#include "Utils.h"
#include "ShaderPair.h"
#include "Model.h"

#include <iostream>
#include <string>

#pragma comment (lib, "glfw3.lib")
#pragma comment (lib, "glew32s.lib")
#pragma comment (lib, "OpenGL32.lib")

constexpr unsigned int SCR_WIDTH = 800;
constexpr unsigned int SCR_HEIGHT = 600;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

unsigned int VaoId, VboId, IboId, ColorBufferId;

ShaderPair* shaders;
Camera* camera;
Model* model;

void DisplayFPS(double currentTime)
{
	static int frameCounter = 0;
	static int lastPrint = glfwGetTime();

	frameCounter++;

	if (currentTime - lastPrint >= 1)
	{
		std::cout << "FPS: " << frameCounter << std::endl;
		frameCounter = 0;
		lastPrint = currentTime;
	}
}

void RenderFrame()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(shaders->GetID());

	shaders->SetMat4("ModelMatrix", model->GetModelMatrix());
	shaders->SetMat4("ViewMatrix", camera->GetViewMatrix());
	shaders->SetMat4("ProjectionMatrix", camera->GetProjectionMatrix());

	model->Render();
}

void Cleanup()
{
	glfwTerminate();
}

void PerformKeysActions(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	float time = deltaTime;

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		time *= Camera::SPEED_BOOST_MULTIPLIER;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera->MoveForward(time);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera->MoveBackward(time);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera->MoveLeft(time);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera->MoveRight(time);
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		camera->MoveUp(time);
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		camera->MoveDown(time);

	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
	{
		int width, height;
		glfwGetWindowSize(window, &width, &height);
		camera->Set(width, height);
	}
}

void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	camera->Set(width, height);
}

void MouseCallback(GLFWwindow* window, double deltaX, double deltaY)
{
	camera->HandlMouseMovement((float)deltaX, (float)deltaY);
}

void ScrollCallback(GLFWwindow* window, double xoffset, double yOffset)
{
	camera->HandleMouseScroll((float)yOffset);
}

void InitializeGraphics()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // culoarea de fond a ecranului
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glDisable(GL_LIGHTING);

	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);

	model = new Model("model.txt");
}

GLFWwindow* InitializeWindow()
{
	// glfw: initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw window creation
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Lab 5", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return nullptr;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
	glfwSetCursorPosCallback(window, MouseCallback);
	glfwSetScrollCallback(window, ScrollCallback);

	// tell GLFW to capture our mouse
	// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);

	glewInit();
	return window;
}

int main(int argc, const char* argv[])
{
	GLFWwindow* window = InitializeWindow();
	if (window == nullptr)
	{
		return -1;
	}
	InitializeGraphics();

	shaders = new ShaderPair("VertexShader.glsl", "FragmentShader.glsl");
	shaders->Use();
	camera = new Camera(SCR_WIDTH, SCR_HEIGHT);

	while (!glfwWindowShouldClose(window))
	{
		double currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		DisplayFPS(currentFrame);
		PerformKeysActions(window);
		RenderFrame();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	delete shaders;
	delete camera;
	delete model;

	return 0;
}
