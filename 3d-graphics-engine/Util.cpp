#include "Constants.h"
#include "Util.h"
#include <GLFW\glfw3.h>

namespace Util
{
	void CalculateCenteredWindowPos(int* newX, int* newY)
	{
		// Get the primary monitor dimensions
		GLFWmonitor* pPrimaryMonitor = glfwGetPrimaryMonitor();

		int monitorX;
		int monitorY;
		glfwGetMonitorPos(pPrimaryMonitor, &monitorX, &monitorY);

		//std::cout << "Monitor x: " << monitorX << " y: " << monitorY << '\n';

		const GLFWvidmode* pVideoMode = glfwGetVideoMode(pPrimaryMonitor);

		/*
		std::cout << "Monitor width: " << pVideoMode->width <<
			" height: " << pVideoMode->height << '\n';
			*/

		*newX = monitorX + (pVideoMode->width - Constants::DEFAULT_WINDOW_WIDTH) / 2;
		*newY = monitorY + (pVideoMode->height - Constants::DEFAULT_WINDOW_HEIGHT) / 2;
	}

	void CenterWindow(GLFWwindow* pWindow)
	{
		int windowPosX;
		int windowPosY;
		CalculateCenteredWindowPos(&windowPosX, &windowPosY);

		glfwSetWindowPos(pWindow, windowPosX, windowPosY);
	}
}
