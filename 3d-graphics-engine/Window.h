#ifndef WINDOW_H
#define WINDOW_H

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <string>

class GraphicsEngine;

class Window
{
	GLFWwindow* m_window;
	const char* m_title;
	int m_width;
	int m_height;

	// Returns true if the cursor is within the window region
	bool m_cursorInWindow;

	// Returns true if the cursor is over an ImGui window
	bool m_imguiHighlighted;
	bool m_firstMouseMove;
	glm::vec2 m_prevMousePosition;

	GraphicsEngine* m_engine;

public:
	Window(const char* title, int width, int height);
	~Window();

	bool Initialise();
	void AttachEngine(GraphicsEngine* pEngine) { m_engine = pEngine; }

	// Calculates the mouse delta and stores in out params x and y
	void CalculateMouseOffset(double xpos, double ypos, float* x, float* y);
	void SwapBuffers() const;

	void OnUserRequestedCursor();

	bool ShouldClose() const;
	bool IsKeyPressed(int key) const;

	// Returns true if the application still has control of the user's cursor
	bool IsUserFocused() const 
	{ 
		return glfwGetInputMode(m_window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED;
	}

	void ImGuiHighlighted(bool highlighted)
	{
		m_imguiHighlighted = highlighted;
	}
	
	int GetWidth() const  { return m_width; }
	int GetHeight() const { return m_height; }
	GraphicsEngine* GetGraphicsEngine() { return m_engine; }

private:
	static void OnCursorEntered(GLFWwindow* window, int entered);
	static void OnWindowResized(GLFWwindow* window, int width, int height);
	static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
};

#endif
