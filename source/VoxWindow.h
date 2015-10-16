#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string>
using namespace std;

#include <GLFW/glfw3.h>


class VoxWindow
{
public:
	/* Public methods */
	VoxWindow();
	~VoxWindow();

	void Create();
	void Destroy();
	void Update(float dt);
	void Render();

	void PollEvents();
	int ShouldCloseWindow();

protected:
	/* Protected methods */

private:
	/* Private methods */

public:
	/* Public members */

protected:
	/* Protected members */

private:
	/* Private members */
	GLFWwindow* window;
};