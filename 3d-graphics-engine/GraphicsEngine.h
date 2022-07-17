#ifndef GRAPHICS_ENGINE_H
#define GRAPHICS_ENGINE_H

#include "Camera.h"
#include "Constants.h"
#include "MeshManager.h"
#include "PointLight.h"
#include "Renderer.h"
#include "ShaderManager.h"
#include "ShadowMap.h"
#include "TextureManager.h"
#include "Window.h"
#include "UIController.h"

#include <glm\glm.hpp>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <array>
#include <string>
#include <map>
#include <vector>

class Entity;
class Mesh;
class ShaderProgram;
class Skybox;

struct SystemData
{
	const GLubyte* renderer;
	const GLubyte* vendor;
	const GLubyte* version;
};

class GraphicsEngine
{
	Window* m_window;
	Camera    m_camera;
	Renderer  m_renderer;
	Skybox* m_skybox;
	std::array<ShadowMap, Constants::MAX_LIGHTS> m_shadowMaps;

	ShaderManager  m_shaderManager;
	MeshManager    m_meshManager;
	TextureManager m_textureManager;

	int m_directionalLightCount;
	int m_pointLightCount;
	int m_numLights;

	std::vector<Entity*>                      m_entities;
	glm::vec3                                 m_globalAmbientLight;
	std::array<Light*, Constants::MAX_LIGHTS> m_lights;
	std::vector<PointLight*>                  m_pointLights;

	// Stores rendering device info as strings
	SystemData m_systemData;

	UIController m_uiController;

public:
	bool m_enableBackfaceCulling;

public:
	GraphicsEngine(Window* window);
	~GraphicsEngine();

	void AddMesh(const std::string& filename);
	void AddTexture(const std::string& filename);
	void AddShader(const std::string& dirName);

	Entity* CreateEntity(const std::string& meshName);

	void SetGlobalAmbientLight(const glm::vec3& ambient);

	void SetSkybox(const std::string& cubemapDir);

	void CreatePointLight(const glm::vec3& position, const glm::vec3& color,
		float intensity = 1.0f);
	void CreateDirectionalLight(const glm::vec3& direction, const glm::vec3& color,
		float intensity = 1.0f);

	void Run();
	void ProcessInput(float dt);
	void Render() const;

	void OnWindowResized(int width, int height);
	void OnImGuiHighlighted(const ImGuiIO& io) const;

	Mesh*          GetMesh(const std::string& name) const;
	Texture*       GetTexture(const std::string& name);
	ShaderProgram* GetShader(const std::string& name);

	// Accessors
	Skybox*        GetSkybox()      { return m_skybox; }	
	const auto&    GetShadowMaps()  { return m_shadowMaps; }
	const auto&    GetPointLights() { return m_pointLights; }

	const auto&    GetEntities()           const { return m_entities; }
	const auto&    GetLights()             const { return m_lights; }
	glm::vec3      GetGlobalAmbientLight() const { return m_globalAmbientLight; }

	int GetLightCount()            const { return m_numLights; }
	int GetPointLightCount()       const { return m_pointLightCount; }
	int GetDirectionalLightCount() const { return m_directionalLightCount; }

	int GetWindowWidth()  const { return m_window->GetWidth(); }
	int GetWindowHeight() const { return m_window->GetHeight(); }

	const SystemData& GetSystemData() const { return m_systemData; }

private:
	void AddLight(Light* pLight);
	void AssignShaders();

public:
	static void ProcessMouseMotion(GLFWwindow* window, double xpos, double ypos);
	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
};

#endif
