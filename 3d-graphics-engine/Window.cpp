#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Window.h"
#include "GraphicsEngine.h"
#include "Util.h"
#include <iostream>
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <functional>

void Window::OnCursorEntered(GLFWwindow* window, int entered)
{
	Window* pWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	// Only if the user is not in the process of exiting
	if(!pWindow->m_cursorInWindow && entered)
	{
		pWindow->m_cursorInWindow = true;
		//printf("Cursor is now in the window\n");
	}
	else
	{
		pWindow->m_cursorInWindow = false;
		//printf("Cursor has left the window\n");
	}
}

void Window::OnWindowResized(GLFWwindow* window, int width, int height)
{
	Window* pWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	// Update the window size
	pWindow->m_width = width;
	pWindow->m_height = height;

	printf("Resized window, new size: %dx%d\n", width, height);

	// Perform the new calculations within the GraphicsEngine 
	pWindow->m_engine->OnWindowResized(width, height);
}

void Window::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	Window* pWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
	if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		// Get the cursor mode
		int cursorMode = glfwGetInputMode(window, GLFW_CURSOR);

		// If the cursor is over an ImGui window
		if(!pWindow->m_imguiHighlighted)
		{
			if(pWindow->m_cursorInWindow && cursorMode == GLFW_CURSOR_NORMAL)
			{
				//printf("Application now has control over cursor\n");
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			}	
		}
		else
		{
			//printf("Cursor is over an ImGui window, ignoring\n");
		}
	}
}

Window::Window(const char* title, int width, int height)
	: m_engine(nullptr)
	, m_window(nullptr)
	, m_title(title)
	, m_width(width)
	, m_height(height)
	, m_firstMouseMove(true)
	, m_cursorInWindow(false)
	, m_imguiHighlighted(false)
	, m_prevMousePosition(0.0f, 0.0f)
{
}

Window::~Window()
{
	std::cout << "Destroying window\n";
	glfwDestroyWindow(m_window);

	std::cout << "Terminating GLFW\n";
	glfwTerminate();
}

bool Window::Initialise()
{
	if(!glfwInit())
	{
		std::cerr << "GLFW failed to initialize!\n";
		return false;
	}

	// Specifies the minimum version of OpenGL that the machine 
	// must be compatible with (OpenGL 4.3)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

	// Enable MSAA, this must be specified before window creation!
	glfwWindowHint(GLFW_SAMPLES, Constants::MSAA_SAMPLES);

	m_window = glfwCreateWindow(m_width, m_height, m_title, nullptr, nullptr);

	// Handle window centering
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
	Util::CenterWindow(m_window);
	glfwShowWindow(m_window);

	glfwMakeContextCurrent(m_window);

	// Enable V-Sync, set the buffer swap interval
	glfwSwapInterval(1);

	glfwGetFramebufferSize(m_window, &m_width, &m_height);
	std::cout << "Framebuffer size is " << m_width << "x" << m_height << "\n";

	// Allows for access to an instance of this class if you have access to its GLFWwindow*
	glfwSetWindowUserPointer(m_window, (void*)this);

	// Setup callbacks
	glfwSetCursorPosCallback(m_window, GraphicsEngine::ProcessMouseMotion);
	glfwSetCursorEnterCallback(m_window, OnCursorEntered);
	glfwSetKeyCallback(m_window, GraphicsEngine::KeyCallback);
	glfwSetFramebufferSizeCallback(m_window, OnWindowResized);
	glfwSetMouseButtonCallback(m_window, MouseButtonCallback);
	
	// Initialize GLEW
	if(glewInit() != GLEW_OK)
	{
		std::cerr << "GLEW failed to initialize!\n";
		return false;
	} 

	glClearColor(0.025f, 0.025f, 0.025f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	// Set the depth comparison function. Choosing GL_LEQUAL helped
	// solve flickering issues around edges of polygons
	glDepthFunc(GL_LEQUAL);

	// Initialise Dear ImGui
	IMGUI_CHECKVERSION();

	// Create Dear ImGui Context
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	// Set Dear ImGui style
	ImGui::StyleColorsDark();
	
	// Set the platform/renderer backends
	ImGui_ImplGlfw_InitForOpenGL(m_window, true);
	ImGui_ImplOpenGL3_Init("#version 430");

	// Initialised the window successfully
	return true;
}

void Window::CalculateMouseOffset(double xpos, double ypos, float* x, float* y)
{
	/*
	 When the mouse callback is called the first time, the xpos and ypos
	 coordinates will be the coordinates of where the mouse is in display
	 coordinates relative to the top left position of the window.
	 This is often a large distance away from the center of the screen
	 and therefore causes a large jump in the camera's movement on startup.
	 To correct this, keep track of whether it is the first time the mouse has moved
	 and if so, set the previous mouse position to xpos and ypos
	*/
	if(m_firstMouseMove)
	{
		m_prevMousePosition.x = xpos;
		m_prevMousePosition.y = ypos;
		m_firstMouseMove = false;
	}

	// Calculate mouse position offset from last frame
	float offsetX = static_cast<float>(xpos) - m_prevMousePosition.x;
	float offsetY = static_cast<float>(ypos) - m_prevMousePosition.y;

	m_prevMousePosition.x = static_cast<float>(xpos);
	m_prevMousePosition.y = static_cast<float>(ypos);

	*x = offsetX;
	*y = offsetY;
}

void Window::SwapBuffers() const
{
	glfwSwapBuffers(m_window);
}

void Window::OnUserRequestedCursor()
{
	// Get the current cursor mode
	int cursorMode = glfwGetInputMode(m_window, GLFW_CURSOR);

	// Only process if WE currently have their cursor (their cursor is hidden).
	if(cursorMode != GLFW_CURSOR_NORMAL)
	{
		m_firstMouseMove = true;

		// Show the cursor
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}

bool Window::ShouldClose() const
{
	return glfwWindowShouldClose(m_window);
}

bool Window::IsKeyPressed(int key) const
{
	return glfwGetKey(m_window, key) == GLFW_PRESS;
}