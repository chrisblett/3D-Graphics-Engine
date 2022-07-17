#ifndef RENDERER_H
#define RENDERER_H

#include "Camera.h"
#include "Constants.h"
#include "DebugQuad.h"
#include "Light.h"
#include <glm\glm.hpp>
#include <unordered_map>

class Entity;
class GraphicsEngine;
class Model;
class Mesh;
class ShaderProgram;
class ShadowData;
class ShadowMap;
class Skybox;

typedef std::array<glm::mat4, Constants::MAX_LIGHTS> tLightMatrixArray;

class Renderer
{
	// Pointers to the two default shaders that entities
	// use, one for entities that use textures and one for
	// those that don't.
	ShaderProgram* m_defaultShader;
	ShaderProgram* m_defaultShaderTextureless;

	// A reference to the camera
	const Camera& m_camera;

	const glm::vec3& m_globalAmbientLight;
	const std::array<Light*, Constants::MAX_LIGHTS>& m_lights;

	// A pointer to the skybox
	Skybox* m_skybox;

	// A pointer to the graphics engine
	GraphicsEngine* m_pGraphicsEngine;

	DebugQuad m_debugQuad;

	bool m_renderPointLights;
	bool m_renderShadowMap;
	bool m_wireframeModeEnabled;

	// Keeps track of the index of the current shadow map being viewed
	int m_shadowMapViewIndex;

	// The colour of all meshes when they are drawn in wireframe mode
	static constexpr glm::vec3 WIREFRAME_COLOUR{ 1.0f, 1.0f, 1.0f };
	static constexpr glm::vec3 WIREFRAME_CLEAR_COLOUR{ 0.0f, 0.0f, 0.0f };

	// Stores the default clear colour
	glm::vec4 m_defaultClearColour;

public:
	Renderer(GraphicsEngine* pEngine, const Camera& cam,
		const std::array<Light*, Constants::MAX_LIGHTS>& lights,
		const glm::vec3& globalAmbientLight);

	//void AddToRenderList(Entity* pEntity, ShaderProgram* pShader);
	void SetSkybox(Skybox* pSkybox) { m_skybox = pSkybox; }

	// Updates the shader uniforms with the current ambient light values
	void UpdateAmbientLight() const;

	// Handles any processing that needs to be done before anything is rendered for the first time
	void BeforeFirstRender();
	void Render() const;

	// Toggle the shadow map view 
	void ToggleRenderShadowMapView();
	// Toggle wireframe rendering mode
	void ToggleWireframeRendering();

	void NextShadowMapView();

	const glm::vec3& GetGlobalAmbientLight()        const { return m_globalAmbientLight; }
	const Camera& GetCamera()                       const { return m_camera; }
	const glm::mat4& GetProjectionMatrix()          const { return m_camera.GetProjectionMatrix(); }
	const glm::mat4& GetViewMatrix()                const { return m_camera.GetViewMatrix(); }

private:
	void ShadowPass(const glm::mat4& lightMatrix) const;
	void RenderPass(const glm::mat4& projection, const glm::mat4& view,
		const tLightMatrixArray& lightMatrices) const;

	void RenderPointLight(Mesh* pModel, ShaderProgram* pShader,
		const glm::vec3& position, const glm::vec3& scale, const glm::vec3& colour) const;
	void RenderPointLights() const;
	void RenderWireframeMode(const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix) const;
	void RenderShadowMapView(const ShadowMap& shadowMap, const ShadowData& shadowData) const;
};

#endif
