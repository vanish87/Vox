#include "../glew/include/GL/glew.h"

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

#pragma comment (lib, "opengl32")
#pragma comment (lib, "glu32")

#include "VoxWindow.h"

// Input callback functionality
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void MouseScrollCallback(GLFWwindow* window, double x, double y);


VoxWindow::VoxWindow()
{
}

VoxWindow::~VoxWindow()
{
}

void VoxWindow::Create()
{
	/* Initialize the window library */
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	/* Initialize any rendering params */
	int samples = 8;
	glfwWindowHint(GLFW_SAMPLES, samples);
	glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
	glGetIntegerv(GL_SAMPLES_ARB, &samples);

	/* Create a windowed mode window and it's OpenGL context */
	int windowWidth = 800;
	int windowHeight = 800;
	window = glfwCreateWindow(windowWidth, windowHeight, "Vox", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	/* Input callbacks */
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetMouseButtonCallback(window, MouseButtonCallback);
	glfwSetScrollCallback(window, MouseScrollCallback);

	/* Center on screen */
	int width;
	int height;
	const GLFWvidmode* vidmode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	glfwGetWindowSize(window, &width, &height);
	glfwSetWindowPos(window, (vidmode->width - width) / 2, (vidmode->height - height) / 2);

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glfwSwapInterval(0); // Disable v-sync

	/* Show the window */
	glfwShowWindow(window);
}

void VoxWindow::Destroy()
{
	glfwTerminate();
}

void VoxWindow::Update(float dt)
{
}

void VoxWindow::Render()
{
	/* Swap front and back buffers */
	glfwSwapBuffers(window);
}

void VoxWindow::PollEvents()
{
	/* Poll for and process events */
	glfwPollEvents();
}

int VoxWindow::ShouldCloseWindow()
{
	return glfwWindowShouldClose(window);
}