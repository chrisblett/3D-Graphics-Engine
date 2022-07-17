#include "Camera.h"
#include "Constants.h"
#include "GraphicsEngine.h"
#include "Entity.h"
#include "Light.h"
#include "Mesh.h"
#include "MeshCreator.h"
#include "Model.h"
#include "Renderer.h"
#include "ShaderUtil.h"
#include "Skybox.h"
#include "Texture.h"
#include <GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <array>

namespace
{
	// Transforms a vector from the range [-1, 1] to [0, 1]
	const glm::mat4 BIAS_MATRIX = glm::mat4(0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f);
}

Renderer::Renderer(GraphicsEngine* pEngine, const Camera& cam,
	const std::array<Light*, Constants::MAX_LIGHTS>& lights,
	const glm::vec3& globalAmbientLight)
	: m_pGraphicsEngine(pEngine)
	, m_camera(cam)
	, m_lights(lights)
	, m_globalAmbientLight(globalAmbientLight)
	, m_skybox(nullptr)
	, m_defaultShader(nullptr)
	, m_defaultShaderTextureless(nullptr)
	, m_renderShadowMap(false)
	, m_renderPointLights(false)
	, m_wireframeModeEnabled(false)
	, m_shadowMapViewIndex(0)
{
	// Store the clear colour
	GLfloat clearCol[4];
	glGetFloatv(GL_COLOR_CLEAR_VALUE, clearCol);

	m_defaultClearColour.r = clearCol[0];
	m_defaultClearColour.g = clearCol[1];
	m_defaultClearColour.b = clearCol[2];
	m_defaultClearColour.a = clearCol[3];
}

void Renderer::UpdateAmbientLight() const
{
	if(m_defaultShader && m_defaultShaderTextureless)
	{
		m_defaultShader->SetUniform("globalAmbientLight", m_globalAmbientLight);
		m_defaultShaderTextureless->SetUniform("globalAmbientLight", m_globalAmbientLight);
	}
	else
	{
		printf("Cannot update ambient light! Default shaders have not been assigned yet!\n");
	}
}

void Renderer::ToggleRenderShadowMapView()
{
	m_renderShadowMap = !m_renderShadowMap;

}

void Renderer::ToggleWireframeRendering()
{
	m_wireframeModeEnabled = !m_wireframeModeEnabled;

	// Set the polygon mode
	if(m_wireframeModeEnabled)
	{
		printf("Wireframe rendering enabled\n");
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		const glm::vec3& wfCol = WIREFRAME_CLEAR_COLOUR;
		glClearColor(wfCol.r, wfCol.g, wfCol.b, 1.0f);
	}
	else
	{
		printf("Wireframe rendering disabled\n");
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		// Set the clear colour back
		glClearColor(m_defaultClearColour.r, m_defaultClearColour.g, m_defaultClearColour.b,
			m_defaultClearColour.a);
	}
}

void Renderer::BeforeFirstRender()
{
	// Only render point lights if there is atleast one in the scene
	m_renderPointLights = m_pGraphicsEngine->GetPointLights().size() > 0;
	if(m_renderPointLights)
	{
		std::cout << "Rendering point lights enabled\n";
	}
	else
	{
		std::cout << "Rendering point lights disabled (there are no point lights)\n";
	}

	m_debugQuad.Initialise(m_pGraphicsEngine->GetShader("debug-quad"));

	// Assign default shaders
	printf("Default shader name: %s\n",
		Constants::DEFAULT_SHADER_NAME_TEXTURE.c_str());
	printf("Default shader (textureless) name: %s\n",
		Constants::DEFAULT_SHADER_NAME_TEXTURELESS.c_str());
	m_defaultShader = m_pGraphicsEngine->GetShader(Constants::DEFAULT_SHADER_NAME_TEXTURE);
	m_defaultShaderTextureless
		= m_pGraphicsEngine->GetShader(Constants::DEFAULT_SHADER_NAME_TEXTURELESS);

	// Assign 'constant' uniforms to both default shaders
	m_defaultShader->SetUniform("globalAmbientLight", m_globalAmbientLight);
	m_defaultShaderTextureless->SetUniform("globalAmbientLight", m_globalAmbientLight);

	int pLights = m_pGraphicsEngine->GetPointLightCount();
	int dLights = m_pGraphicsEngine->GetDirectionalLightCount();
	m_defaultShader->SetUniform("dirLightCount", dLights);
	m_defaultShader->SetUniform("pointLightCount", pLights);
	m_defaultShaderTextureless->SetUniform("dirLightCount", dLights);
	m_defaultShaderTextureless->SetUniform("pointLightCount", pLights);

	// Print out the number of directional and point lights
	printf("dirLightCount: %d\n", m_defaultShader->GetUniformiValue("dirLightCount"));
	printf("pointLightCount: %d\n", m_defaultShader->GetUniformiValue("pointLightCount"));

	// Bind the shadow maps to the correct slots
	for(int i = 0; i < m_pGraphicsEngine->GetLightCount(); i++)
	{
		m_defaultShader->SetUniform("shadowMaps[" + std::to_string(i) + "]",
			i + 1);
		m_defaultShaderTextureless->SetUniform("shadowMaps[" + std::to_string(i) + "]",
			i + 1);
	}
}

void Renderer::NextShadowMapView()
{
	// Increment the index with cycling
	m_shadowMapViewIndex = (m_shadowMapViewIndex + 1) % m_pGraphicsEngine->GetLightCount();
	printf("m_shadowMapViewIndex: %d\n", m_shadowMapViewIndex);
}

void Renderer::ShadowPass(const glm::mat4& lightMatrix) const
{
	// Reduces the visiblity of shadow acne
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1.1f, 4.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	ShaderProgram* pCurShader = m_pGraphicsEngine->GetShader("shadow-map");
	pCurShader->Bind();

	const auto& entities = m_pGraphicsEngine->GetEntities();
	for(const auto& entity : entities)
	{
		glm::mat4 modelMatrix = entity->CalculateModelMatrix();

		pCurShader->SetUniform("lightMatrix", lightMatrix * modelMatrix);

		entity->Render();
	}
	glDisable(GL_POLYGON_OFFSET_FILL);
}

void Renderer::RenderPass(const glm::mat4& projection, const glm::mat4& view,
	const tLightMatrixArray& lightMatrices) const
{
	// Bind back to the default framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glViewport(0, 0, m_pGraphicsEngine->GetWindowWidth(), m_pGraphicsEngine->GetWindowHeight());

	// Clear its colour buffer and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Draw the skybox
	if(m_skybox)
	{
		m_skybox->PreRender(projection, view);
		m_skybox->Render();
	}

	// Get the entities
	const auto& entities = m_pGraphicsEngine->GetEntities();

	for(const auto& entity : entities)
	{
		ShaderProgram* pCurShader = entity->GetShader();
		pCurShader->Bind();

		entity->Bind(pCurShader);

		glm::mat4 modelMatrix = entity->CalculateModelMatrix();

		// Get the light count
		int lightCount = m_pGraphicsEngine->GetLightCount();
		for(int i = 0; i < lightCount; ++i)
		{
			ShadowData* shadowData = m_lights[i]->GetShadowData();
			pCurShader->SetUniform("shadowBiases[" + std::to_string(i) + "]",
				shadowData->GetShadowBias());
			pCurShader->SetUniform("lightMatrices[" + std::to_string(i) + "]",
				lightMatrices[i] * modelMatrix);
		}

		pCurShader->SetUniform("modelToWorldMatrix", modelMatrix);
		pCurShader->SetUniform("modelViewProjMatrix", projection * view * modelMatrix);

		entity->Render();
	}
}

void Renderer::Render() const
{
	const glm::mat4& viewMatrix = m_camera.GetViewMatrix();
	const glm::mat4& projectionMatrix = m_camera.GetProjectionMatrix();
	const glm::vec3& camPos = m_camera.GetPosition();

	// Update uniforms that change every frame
	m_defaultShader->SetUniform("eyePositionWorld", camPos);
	m_defaultShaderTextureless->SetUniform("eyePositionWorld", camPos);

	// Set light uniforms
	for(int i = 0, n = m_pGraphicsEngine->GetLightCount(); i < n; ++i)
	{
		m_lights[i]->SetUniforms(m_defaultShader, i);
		m_lights[i]->SetUniforms(m_defaultShaderTextureless, i);
	}

	if(m_wireframeModeEnabled)
	{
		// Render everything in wireframe mode
		RenderWireframeMode(projectionMatrix, viewMatrix);
		return;
	}

	// Stores each computed light matrix
	std::array<glm::mat4, Constants::MAX_LIGHTS> lightMatrices;
	
	const auto& shadowMaps = m_pGraphicsEngine->GetShadowMaps();

	// Get the number of active lights 
	const int lightCount = m_pGraphicsEngine->GetLightCount();
	for(int i = 0; i < lightCount; ++i)
	{
		ShadowData* shadowData = m_lights[i]->GetShadowData();

		// Can this light support shadows?
		if(shadowData)
		{
			glm::mat4 lightMatrix = shadowData->GetLightMatrix();
			const ShadowMap* pShadowMap = &shadowMaps[i];

			// Enable for writing
			pShadowMap->Write();

			// Do the shadow pass, updating the shadow map texture
			ShadowPass(lightMatrix);

			// Bind the map to the correct texture slot
			pShadowMap->Read(GL_TEXTURE0 + ShadowMap::START_TEXTURE_SLOT + i);

			lightMatrix = BIAS_MATRIX * lightMatrix;
			lightMatrices[i] = lightMatrix;
		}
	}

	// Do the render pass
	RenderPass(projectionMatrix, viewMatrix, lightMatrices);

	if(m_renderShadowMap)
	{
		// Retrieve the shadow map and shadow data for light that shadow map
		// is to be rendered
		const ShadowMap* pShadowMap = &shadowMaps[m_shadowMapViewIndex];
		const ShadowData* pShadowData = m_lights[m_shadowMapViewIndex]->GetShadowData();
		RenderShadowMapView(*pShadowMap, *pShadowData);
	}

	if(m_renderPointLights)
	{
		RenderPointLights();
	}
}

void Renderer::RenderWireframeMode(const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix) const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	ShaderProgram* pShader = m_pGraphicsEngine->GetShader("flat-colour");
	pShader->Bind();

	// NOTE: Can be made a constant
	// Set colour to white
	pShader->SetUniform("diffuseColour", WIREFRAME_COLOUR);

	const auto& entities = m_pGraphicsEngine->GetEntities();

	for(const auto& entity : entities)
	{
		glm::mat4 modelMatrix = entity->CalculateModelMatrix();
		pShader->SetUniform("modelViewProjMatrix", projectionMatrix * viewMatrix * modelMatrix);

		entity->Render();
	}

	// Draw point lights too
	auto pointLights = m_pGraphicsEngine->GetPointLights();
	Mesh* pLightMesh = m_pGraphicsEngine->GetMesh("sphere.obj");
	const glm::vec3 POINT_LIGHT_MESH_SCALE{ 0.1f, 0.1f, 0.1f };

	for(int i = 0, total = pointLights.size(); i < total; ++i)
	{
		RenderPointLight(pLightMesh, pShader, pointLights[i]->GetPosition(),
			POINT_LIGHT_MESH_SCALE, WIREFRAME_COLOUR);
	}
}

void Renderer::RenderShadowMapView(const ShadowMap& shadowMap, const ShadowData& shadowData) const
{
	// Draw to the default framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	m_debugQuad.Bind();
	bool isLightMatrixOrthographic = shadowData.IsOrthographic();

	// Send uniforms to the quad debug shader
	ShaderProgram* pShader = m_debugQuad.GetShader();
	pShader->SetUniform("isOrtho", isLightMatrixOrthographic);

	//std::cout << "isOrtho: " << pShader->GetUniformValue("isOrtho") << std::endl;

	// A perspective matrix needs to linearize depth for visualisation,
	// so we need to pass in its near and far plane values.
	if(!isLightMatrixOrthographic)
	{
		pShader->SetUniform("nearZ", shadowData.GetNearPlane());
		pShader->SetUniform("farZ", shadowData.GetFarPlane());
	}

	// Allow the red channel to be assigned the depth values
	shadowMap.SetTextureCompareMode(GL_NONE);
	shadowMap.Read(GL_TEXTURE0);

	m_debugQuad.Render(m_pGraphicsEngine->GetWindowWidth(), m_pGraphicsEngine->GetWindowHeight());

	shadowMap.SetTextureCompareMode(GL_COMPARE_REF_TO_TEXTURE);
}

void Renderer::RenderPointLight(Mesh* pMesh, ShaderProgram* pShader,
	const glm::vec3& position, const glm::vec3& scale, const glm::vec3& colour) const
{
	const auto& viewMatrix = m_camera.GetViewMatrix();
	const auto& projMatrix = m_camera.GetProjectionMatrix();

	// Create the model transformation matrix
	glm::mat4 translation = glm::translate(glm::mat4(1.0f), position);
	glm::mat4 model = glm::scale(translation, scale);

	// Send uniforms
	pShader->SetUniform("diffuseColour", colour);
	pShader->SetUniform("modelViewProjMatrix", projMatrix * viewMatrix * model);

	pMesh->Render();
}

// TODO: Make this more efficient
void Renderer::RenderPointLights() const
{
	auto pointLights = m_pGraphicsEngine->GetPointLights();

	// Render light meshes
	ShaderProgram* pFlatColourShader = m_pGraphicsEngine->GetShader("flat-colour");

	// Bind the shader
	pFlatColourShader->Bind();

	// Get the mesh that the lights use
	Mesh* pLightMesh = m_pGraphicsEngine->GetMesh("sphere.obj");

	const glm::vec3 POINT_LIGHT_MESH_SCALE{ 0.1f, 0.1f, 0.1f };

	for(int i = 0, total = pointLights.size(); i < total; ++i)
	{
		RenderPointLight(pLightMesh, pFlatColourShader, pointLights[i]->GetPosition(),
			POINT_LIGHT_MESH_SCALE, pointLights[i]->GetDiffuse());
	}
}