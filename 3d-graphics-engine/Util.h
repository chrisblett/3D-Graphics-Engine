#ifndef UTIL_H
#define UTIL_H

struct GLFWwindow;

namespace Util
{
	void CenterWindow(GLFWwindow* pWindow);
	void CalculateCenteredWindowPos(int* newX, int* newY);
}

#endif