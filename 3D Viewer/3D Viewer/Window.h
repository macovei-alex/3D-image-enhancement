#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <glfw3.h>

#include <iostream>
#include <string>

#include "Camera.h"
#include "Utils.h"
#include "ShaderProgram.h"
#include "Model.h"
#include "LightSource.h"

class Window
{
public:
	Window(std::unique_ptr<ShaderProgram>&& modelShader, std::unique_ptr<ShaderProgram>&& lightShader, std::unique_ptr<Model>&& model, std::unique_ptr<LightSource>&& lightSource);
	void RenderFrame() const;

private:
	void InitializeGraphics();
	void InitializeWindow();

public:
	static const unsigned int SCREEN_WIDTH = 800;
	static const unsigned int SCREEN_HEIGHT = 600;

private:
	std::unique_ptr<GLFWwindow> window;
	std::unique_ptr<ShaderProgram> modelShaders;
	std::unique_ptr<ShaderProgram> lightingShaders;
	std::unique_ptr<Model> model;
	std::unique_ptr<LightSource> lightSource;
	std::unique_ptr<Camera> camera;
};

