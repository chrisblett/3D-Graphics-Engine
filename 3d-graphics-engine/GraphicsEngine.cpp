#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Constants.h"
#include "DirectionalLight.h"
#include "Entity.h"
#include "GraphicsEngine.h"
#include "Light.h"
#include "MeshCreator.h"
#include "MeshLoader.h"
#include "PointLight.h"
#include "Skybox.h"
#include "Texture.h"
#include "Window.h"
#include <glm\glm.hpp>
#include <iostream>
#include <cassert>

GraphicsEngine::GraphicsEngine(Window* window)
	: m_window(window)
	, m_camera(window->GetWidth(), window->GetHeight())
	, m_numLights(0)
	, m_directionalLightCount(0)
	, m_pointLightCount(0)
	, m_renderer(this, m_camera, m_lights, m_globalAmbientLight)
	, m_uiController(this)
	, m_skybox(nullptr)
	, m_enableBackfaceCulling(true)
{
	assert(m_window && "Window is NULL");

	// Allows for the possibility to retrieve this instance from the window
	m_window->AttachEngine(this);

	// Retrieve and store system data
	m_systemData.vendor   = glGetString(GL_VENDOR);
	m_systemData.renderer = glGetString(GL_RENDERER);
	m_systemData.version  = glGetString(GL_VERSION);

	// Enable multisampling 
	glEnable(GL_MULTISAMPLE);

	// Create internal meshes (primitives)
	m_meshManager.CreateMesh("plane", MeshCreator::CreatePlane(4));
	m_meshManager.CreateMesh("cube",  MeshCreator::CreateCube());
	AddMesh("sphere.obj");
}

GraphicsEngine::~GraphicsEngine()
{
	std::cout << "Destroying engine\n";

	std::cout << "Freeing all entities\n";
	for(const auto& entity : m_entities)
	{
		delete entity;
	}

	std::cout << "Freeing all lights\n";
	for(int i = 0; i < m_numLights; ++i)
	{
		delete m_lights[i];
	}

	// Delete skybox
	if(m_skybox)
	{
		delete m_skybox;
	}
}

void GraphicsEngine::AddMesh(const std::string& filename)
{
	m_meshManager.CreateMesh(filename);
}

void GraphicsEngine::AddShader(const std::string& dirName)
{
	const std::string vertPath = Constants::SHADER_PATH + dirName + "/vert.glsl";
	const std::string fragPath = Constants::SHADER_PATH + dirName + "/frag.glsl";

	m_shaderManager.RegisterShader(vertPath, fragPath, dirName.c_str());
}

void GraphicsEngine::AddTexture(const std::string& filename)
{
	m_textureManager.CreateTexture(filename);
}

void GraphicsEngine::AddLight(Light* pLight)
{
	if(m_numLights >= Constants::MAX_LIGHTS)
	{
		// Assumes that the pointer is to memory on the heap
		delete pLight;
		assert(false && "Maximum light capacity reached");
	}
	else
	{
		m_lights[m_numLights] = pLight;
		m_numLights++;

		std::cout << "Created " << m_numLights << "/" << Constants::MAX_LIGHTS << " lights\n";
	}
}

void GraphicsEngine::CreatePointLight(const glm::vec3& position, const glm::vec3& color,
	float intensity)
{
	// Keep track of each light by name
	//m_lightFreq.Add("PointLight");
	m_uiController.AddSceneObject("PointLight");

	PointLight* pPointLight = new PointLight(position, color, intensity);

	// Allow for shadow mapping with point lights
	pPointLight->CreateShadowData(GetWindowWidth() / GetWindowHeight());

	AddLight(pPointLight);
	m_pointLightCount++;

	// Store point light data separately for easier point light drawing
	m_pointLights.push_back(pPointLight);
}

void GraphicsEngine::CreateDirectionalLight(const glm::vec3& direction, const glm::vec3& color, float intensity)
{
	//m_lightFreq.Add("DirectionalLight");
	m_uiController.AddSceneObject("DirectionalLight");

	AddLight(new DirectionalLight(glm::normalize(direction), color, intensity));
	m_directionalLightCount++;
}

Entity* GraphicsEngine::CreateEntity(const std::string& meshName)
{
	Mesh* pMesh = GetMesh(meshName);
	assert(pMesh);

	// Dynamically allocate entities rather than create them on the stack. 
	// This is done because if the vector ever needs to resize, the stack
	// allocated entites will need to be copied to new memory, this will
	// invalidate their pointers that may have been stored elsewhere.
	m_entities.push_back(new Entity(pMesh));
	Entity* pEntity = m_entities.back();
	
	//m_entFreq.Add(meshName);
	m_uiController.AddSceneObject(meshName);

	return pEntity;
}

void GraphicsEngine::SetGlobalAmbientLight(const glm::vec3& ambient)
{
	if(!Light::IsValidColour(ambient))
	{
		assert(false && "Invalid global ambient light colour provided!");
	}

	m_globalAmbientLight = ambient;
	m_renderer.UpdateAmbientLight();
}

void GraphicsEngine::SetSkybox(const std::string& cubemapDir)
{
	ShaderProgram* skyboxShader = nullptr;
	const Mesh* cubeMesh = nullptr;

	if(m_skybox)
	{
		// Replace the old skybox with a new one. 
		// The new skybox will reuse the same shader and mesh data
		skyboxShader = &m_skybox->GetShader();
		cubeMesh = &m_skybox->GetMesh();

		// Free the memory used by the old skybox
		std::cout << "Creating new Skybox, freeing old skybox\n";
		delete m_skybox;

		Skybox* newSkybox = new Skybox(cubemapDir, *cubeMesh, *skyboxShader);
		m_renderer.SetSkybox(newSkybox);

		m_skybox = newSkybox;
	}
	else
	{
		// A skybox hasn't ever been created, obtain the shader and mesh it will use
		skyboxShader = m_shaderManager.GetShaderByName("skybox");
		assert(skyboxShader && "Skybox shader is NULL");

		cubeMesh = GetMesh("cube");

		m_skybox = new Skybox(cubemapDir, *cubeMesh, *skyboxShader);
		m_renderer.SetSkybox(m_skybox);

		// Enable interpolation across cubemap textures to prevent seams
		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	}
}

Mesh* GraphicsEngine::GetMesh(const std::string& name) const
{
	return m_meshManager.GetMesh(name);
}

Texture* GraphicsEngine::GetTexture(const std::string& name)
{
	return m_textureManager.GetTexture(name);
}

ShaderProgram* GraphicsEngine::GetShader(const std::string& name)
{
	return m_shaderManager.GetShaderByName(name);
}

void GraphicsEngine::Render() const
{
	// Update any state that may have changed
	if(m_enableBackfaceCulling)
	{
		glEnable(GL_CULL_FACE);
	}
	else
	{
		glDisable(GL_CULL_FACE);
	}

	m_renderer.Render();
}

void GraphicsEngine::OnWindowResized(int width, int height)
{
	// Readjust the viewport
	glViewport(0, 0, width, height);

	m_camera.CreateProjectionMatrix(width, height);
}

void GraphicsEngine::ProcessMouseMotion(GLFWwindow* window, double xpos, double ypos)
{
	Window* pWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if(pWindow->IsUserFocused())
	{
		GraphicsEngine* pEngine = pWindow->GetGraphicsEngine();

		// Store the distance the mouse travelled last update in these variables
		float offsetX, offsetY;

		pWindow->CalculateMouseOffset(xpos, ypos, &offsetX, &offsetY);
		pEngine->m_camera.UpdateOrientation(offsetX, offsetY);
	}
}

void GraphicsEngine::ProcessInput(float dt)
{
	if(m_window->IsKeyPressed(GLFW_KEY_W))
	{
		m_camera.CalculateVelocity(FORWARD, dt);
	}

	if(m_window->IsKeyPressed(GLFW_KEY_A))
	{
		m_camera.CalculateVelocity(LEFT, dt);
	}

	if(m_window->IsKeyPressed(GLFW_KEY_S))
	{
		m_camera.CalculateVelocity(BACKWARD, dt);
	}

	if(m_window->IsKeyPressed(GLFW_KEY_D))
	{
		m_camera.CalculateVelocity(RIGHT, dt);
	}

	if(m_window->IsKeyPressed(GLFW_KEY_Q))
	{
		m_camera.CalculateVelocity(UP, dt);
	}

	if(m_window->IsKeyPressed(GLFW_KEY_E))
	{
		m_camera.CalculateVelocity(DOWN, dt);
	}

	m_camera.UpdatePosition(dt);
}

void GraphicsEngine::KeyCallback(GLFWwindow* window, int key, int scancode,
	int action, int mods)
{
	Window* pWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
	GraphicsEngine* pEngine = pWindow->GetGraphicsEngine();

	// User requested control of their cursor
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		pWindow->OnUserRequestedCursor();
	}

	// User requested wireframe rendering mode
	if(key == GLFW_KEY_F && action == GLFW_PRESS)
	{
		pEngine->m_renderer.ToggleWireframeRendering();
	}

	// Toggle the shadow map debug view
	if(key == GLFW_KEY_TAB && action == GLFW_PRESS)
	{
		pEngine->m_renderer.ToggleRenderShadowMapView();
		std::cout << "Toggling shadow map rendering\n";
	}

	// Move through different shadow map views
	if(key == GLFW_KEY_C && action == GLFW_PRESS)
	{
		pEngine->m_renderer.NextShadowMapView();
	}
}

void GraphicsEngine::AssignShaders()
{
	for(Entity* entity : m_entities)
	{
		// Assign the appropriate shader depending on if the material
		// uses textures
		Material* mat = entity->GetMaterial();

		if(mat->HasTexture())
		{
			entity->SetShader(GetShader(Constants::DEFAULT_SHADER_NAME_TEXTURE));
		}
		else
		{
			entity->SetShader(GetShader(Constants::DEFAULT_SHADER_NAME_TEXTURELESS));
		}
	}
}

void GraphicsEngine::Run()
{
	// Set up entity shaders, this cannot be done on their creation
	// since the material is not known at that time.
	AssignShaders();

	float previousTime = 0.0f;

	// Set before first render call begins
	m_renderer.BeforeFirstRender();
	m_uiController.BeforeFirstRender(m_entities.size(), m_numLights);

	// Main loop
	while(!m_window->ShouldClose())
	{
		// Calculate the time passed since the last frame so everything 
		// updates relative to how long the frame took to complete.
		// This ensures that the program will not run faster or slower 
		// on machines of varying performance.
		float currentTime = static_cast<float>(glfwGetTime());
		float deltaTime = currentTime - previousTime;
		previousTime = currentTime;

		ProcessInput(deltaTime);

		Render();

		// Draw the GUI
		m_uiController.DrawUI(this);

		m_window->SwapBuffers();
		glfwPollEvents();
	}

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void GraphicsEngine::OnImGuiHighlighted(const ImGuiIO& io) const
{
	m_window->ImGuiHighlighted(io.WantCaptureMouse);
}