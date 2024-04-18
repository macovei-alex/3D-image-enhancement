#define GLEW_STATIC
#include <GL/glew.h>

#include <glfw3.h>

#include "Camera.h"
#include "Utils.h"
#include "ShaderProgram.h"
#include "Model.h"
#include "LightSource.h"

#include <iostream>
#include <string>

#pragma comment (lib, "glfw3.lib")
#pragma comment (lib, "glew32s.lib")
#pragma comment (lib, "OpenGL32.lib")

constexpr unsigned int SCREEN_WIDTH = 800;
constexpr unsigned int SCREEN_HEIGHT = 600;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

unsigned int VaoId, VboId, IboId, ColorBufferId;

ShaderProgram* modelShaders, * lightingShaders;
Camera* camera;
Model* model;
LightSource* lightSource;

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

void PerformKeysActions(GLFWwindow* window)
{
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
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
	{
		int width, height;
		glfwGetWindowSize(window, &width, &height);
		camera->Set(width, height);
	}

	else if (key == GLFW_KEY_Z && action == GLFW_PRESS)
		lightSource->SetAmbientStrength(lightSource->GetAmbientStrength() + 0.1f);
	else if (key == GLFW_KEY_X && action == GLFW_PRESS)
		lightSource->SetAmbientStrength(lightSource->GetAmbientStrength() - 0.1f);
	else if (key == GLFW_KEY_C && action == GLFW_PRESS)
		lightSource->SetSpecularStrength(lightSource->GetSpecularStrength() + 0.1f);
	else if (key == GLFW_KEY_V && action == GLFW_PRESS)
		lightSource->SetSpecularStrength(lightSource->GetSpecularStrength() - 0.1f);
	else if (key == GLFW_KEY_B && action == GLFW_PRESS)
		lightSource->SetDiffuseStrength(lightSource->GetDiffuseStrength() + 0.1f);
	else if (key == GLFW_KEY_N && action == GLFW_PRESS)
		lightSource->SetDiffuseStrength(lightSource->GetDiffuseStrength() - 0.1f);
	else if (key == GLFW_KEY_M && action == GLFW_PRESS)
		lightSource->SetSpecularExponent(lightSource->GetSpecularExponent() * 2);
	else if (key == GLFW_KEY_COMMA && action == GLFW_PRESS)
		lightSource->SetSpecularExponent(lightSource->GetSpecularExponent() / 2);
}

void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	camera->Set(width, height);
}

void MouseCallback(GLFWwindow* window, double deltaX, double deltaY)
{
	camera->HandlMouseMovement(static_cast<float>(deltaX), static_cast<float>(deltaY));
}

void ScrollCallback(GLFWwindow* window, double xoffset, double yOffset)
{
	camera->HandleMouseScroll(static_cast<float>(yOffset));
}

void InitializeGraphics()
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

GLFWwindow* InitializeWindow()
{
	// glfw: initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw window creation
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "3D Model Viewer", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return nullptr;
	}

	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
	glfwSetCursorPosCallback(window, MouseCallback);
	glfwSetScrollCallback(window, ScrollCallback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);

	glewInit();
	return window;
}

void Clean()
{
	delete modelShaders, lightingShaders;
	delete camera;
	delete model;
	delete lightSource;

	glfwTerminate();
}

void RenderFrame()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	lightingShaders->Use();

	lightingShaders->SetVec3("LightColor", lightSource->GetColor());
	lightingShaders->SetVec3("LightPosition", lightSource->GetModel().GetPosition());
	lightingShaders->SetVec3("ViewPosition", camera->GetPosition());

	lightingShaders->SetFloat("AmbientStrength", lightSource->GetAmbientStrength());
	lightingShaders->SetFloat("DiffuseStrength", lightSource->GetDiffuseStrength());
	lightingShaders->SetFloat("SpecularStrength", lightSource->GetSpecularStrength());
	lightingShaders->SetInt("SpecularExponent", lightSource->GetSpecularExponent());

	lightingShaders->SetMat4("ModelMatrix", model->GetModelMatrix());
	lightingShaders->SetMat4("ViewMatrix", camera->GetViewMatrix());
	lightingShaders->SetMat4("ProjectionMatrix", camera->GetProjectionMatrix());

	model->Render();

	modelShaders->Use();

	modelShaders->SetMat4("ModelMatrix", lightSource->GetModel().GetModelMatrix());
	modelShaders->SetMat4("ViewMatrix", camera->GetViewMatrix());
	modelShaders->SetMat4("ProjectionMatrix", camera->GetProjectionMatrix());

	lightSource->GetModel().Render();
}

int main(int argc, const char* argv[])
{
	GLFWwindow* window = InitializeWindow();
	if (window == nullptr)
	{
		return -1;
	}
	InitializeGraphics();

	modelShaders = new ShaderProgram("modelVS.glsl", "modelFS.glsl");
	lightingShaders = new ShaderProgram("lightingVS.glsl", "lightingFS.glsl");
	camera = new Camera(SCREEN_WIDTH, SCREEN_HEIGHT);

	model = new Model("model.txt", true);
	lightSource = new LightSource(std::move(Model("lightModel.txt", true)));
	lightSource->GetModel().SetPosition(camera->GetPosition() + glm::vec3(0.0f, 1.0f, 0.0f));

	while (!glfwWindowShouldClose(window))
	{
		double currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		model->Rotate(glm::vec3(0.0f, deltaTime, 0.0f));

		DisplayFPS(currentFrame);
		PerformKeysActions(window);
		RenderFrame();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	Clean();
	return 0;
}