#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "UIController.h"
#include "GraphicsEngine.h"
#include "Entity.h"
#include "Light.h"
#include "PointLight.h"
#include "DirectionalLight.h"
#include <string>
#include <cassert>

#define SHOW_IMGUI_DEMO 0

// This function allocates memory, therefore
// it is the caller's responsibility to free the memory
static const char* GetCString(const std::string& str)
{
	// Get the size of the string and add one for NULL terminator
	auto size = str.size() + 1;

	// Create a buffer to fit the string
	char* buff = new char[size];

	// Copy the memory
	memcpy(buff, str.c_str(), size);

	return buff;
}

static void ClampColour(float* col)
{
	col[0] = std::clamp(col[0], 0.0f, 1.0f);
	col[1] = std::clamp(col[1], 0.0f, 1.0f);
	col[2] = std::clamp(col[2], 0.0f, 1.0f);
}

UIController::UIController(GraphicsEngine* pEngine)
	: m_systemData(pEngine->GetSystemData())
{
	m_pEntities = &pEngine->GetEntities();
	m_pLights = &pEngine->GetLights();
}

UIController::~UIController()
{
	// Free the memory for the names
	for(auto c : m_entNames)
	{
		delete[] c;
	}

	for(auto c : m_lightNames)
	{
		delete[] c;
	}
}

void UIController::AddSceneObject(const std::string& name)
{
	// Add light source names to a specific frequency list
	if(name == "PointLight" || name == "DirectionalLight")
	{
		m_lightFreq.Add(name);
	}
	else
	{
		// Assume any other name is an entity
		m_entFreq.Add(name);
	}
}

void UIController::BeforeFirstRender(int numEntites, int numLights)
{
	std::vector<std::string> entNamesStr = m_entFreq.GetStrings();
	std::vector<std::string> lightNamesStr = m_lightFreq.GetStrings();

	// Make sure the sizes match
	assert(numEntites == entNamesStr.size() && "Number of entities in the scene and"
		"the number of entity names do not match!");

	assert(numLights == lightNamesStr.size() && "Number of lights in the scene and"
		"the number of light names do not match!");

	for(const auto& name : entNamesStr)
	{
		m_entNames.push_back(GetCString(name));
	}

	for(const auto& name : lightNamesStr)
	{
		m_lightNames.push_back(GetCString(name));
	}
}

void UIController::DrawUI(GraphicsEngine* pEngine) const
{
	// Start a new Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// Check if the cursor is over an ImGui element
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	pEngine->OnImGuiHighlighted(io);

#if(SHOW_IMGUI_DEMO)
	ImGui::ShowDemoWindow();
#endif

	// System window
	if(ImGui::Begin("System"))
	{
		ImGui::Text("Vendor: %s", m_systemData.vendor);
		ImGui::Text("Renderer: %s", m_systemData.renderer);
		ImGui::Text("Version: %s", m_systemData.version);
		ImGui::Separator();
		// Display average frame time and framerate
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
			1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
	ImGui::End();

	// Stores the index of the currently selected entity
	static int curEntIdx = 0;

	// Stores the index of the currently selected light
	static int curLightIdx = 0;

	assert(m_lightFreq.GetSize() == m_lightNames.size() && "Sizes do not match!");
	assert(m_entFreq.GetSize() == m_entNames.size() && "Sizes do not match!");

	// Scene properties window
	if(ImGui::Begin("Scene"))
	{
		if(ImGui::BeginTabBar("##tabbar"))
		{
			if(ImGui::BeginTabItem("Entities"))
			{
				// Only allow entities to be configured if there is at least
				// one entity in the scene
				if(!m_entNames.empty())
				{
					ImGui::ListBox("##entities", &curEntIdx, &m_entNames[0], m_entNames.size());
					ImGui::Checkbox("Backface Culling", &pEngine->m_enableBackfaceCulling);
				}
				ImGui::EndTabItem();
			}
			if(ImGui::BeginTabItem("Lighting"))
			{
				// Only allow lighting to be configured if there is at least
				// one light source in the scene
				if(!m_lightNames.empty())
				{
					// Display a list of the names of all light sources in the scene
					ImGui::ListBox("##lights", &curLightIdx,
						&m_lightNames[0], m_lightNames.size());

					glm::vec3 globalAmbient = pEngine->GetGlobalAmbientLight();
					// Modify ambient light					
					float newAmbient[3];
					newAmbient[0] = globalAmbient.r;
					newAmbient[1] = globalAmbient.g;
					newAmbient[2] = globalAmbient.b;
					ImGui::ColorEdit3("Ambient", newAmbient);

					ClampColour(newAmbient);
					pEngine->SetGlobalAmbientLight({ newAmbient[0], newAmbient[1], newAmbient[2] });
				}
				ImGui::EndTabItem();
			}
		}
	}	ImGui::EndTabBar();
	ImGui::End();

	// Inspector window
	if(ImGui::Begin("Inspector"))
	{
		if(ImGui::BeginTabBar("##tabbar"))
		{
			if(ImGui::BeginTabItem("Entity"))
			{
				if(!m_pEntities->empty())
				{
					// Get a reference to the current entity that was selected 
					// from the entity list
					Entity* selectedEntity = m_pEntities->at(curEntIdx);
					if(selectedEntity)
					{
						// Display its name
						ImGui::Text("Selected Entity: %s", m_entNames.at(curEntIdx));

						// Display transformation elements
						if(ImGui::CollapsingHeader("Translation"))
						{
							glm::vec3 currentPos = selectedEntity->GetPosition();

							// ImGui will ignore displaying anything after '##'
							// This allows slider names to appear the same without conflicts.
							ImGui::SliderFloat("X##translation", &currentPos.x, -10.0f, 10.0f);
							ImGui::SliderFloat("Y##translation", &currentPos.y, -10.0f, 10.0f);
							ImGui::SliderFloat("Z##translation", &currentPos.z, -10.0f, 10.0f);

							selectedEntity->SetPosition(currentPos);
						}
						if(ImGui::CollapsingHeader("Rotation"))
						{
							// Get the entity's rotation information
							glm::vec3 currentRot = selectedEntity->GetRotation();

							// Convert to radians
							float rotXRad = glm::radians(currentRot.x);
							float rotYRad = glm::radians(currentRot.y);
							float rotZRad = glm::radians(currentRot.z);

							ImGui::SliderAngle("X##rotation", &rotXRad, -180.0f, 180.0f);
							ImGui::SliderAngle("Y##rotation", &rotYRad, -180.0f, 180.0f);
							ImGui::SliderAngle("Z##rotation", &rotZRad, -180.0f, 180.0f);

							// Convert back to degrees and set the new rotation
							selectedEntity->SetRotation(glm::degrees(rotXRad), glm::degrees(rotYRad),
								glm::degrees(rotZRad));
						}
						if(ImGui::CollapsingHeader("Scale"))
						{
							float curScale = selectedEntity->GetScale().x;
							ImGui::SliderFloat("Factor", &curScale, 0.0f, 10.0f);
							selectedEntity->SetScale(curScale);
						}
					}
				}
				// End of tab
				ImGui::EndTabItem();
			}

			if(ImGui::BeginTabItem("Light"))
			{
				if(!m_pLights->empty())
				{
					// Get a pointer to the selected light
					Light* selectedLight = m_pLights->at(curLightIdx);
					if(selectedLight)
					{
						// Display the selected light's name
						ImGui::Text("Selected Light: %s", m_lightNames.at(curLightIdx));

						// Determine the type of light
						bool isPointLight = false;
						if(dynamic_cast<PointLight*>(selectedLight))
						{
							isPointLight = true;
						}

						if(isPointLight)
						{
							/* Point light specific options */

							// Cast to a PointLight
							PointLight* pointLight = dynamic_cast<PointLight*>(selectedLight);
							glm::vec3 lightPos = pointLight->GetPosition();

							// Create widgets
							ImGui::Text("Position");
							ImGui::SliderFloat("X##point_translation", &lightPos.x, -10.0f, 10.0f);
							ImGui::SliderFloat("Y##point_translation", &lightPos.y, -10.0f, 10.0f);
							ImGui::SliderFloat("Z##point_translation", &lightPos.z, -10.0f, 10.0f);

							// Update position
							pointLight->SetPosition(lightPos);
						}
						else
						{							
							// Directional light specific options

							// Cast to a DirectionalLight
							DirectionalLight* dirLight
								= dynamic_cast<DirectionalLight*>(selectedLight);
							glm::vec3 lightDir = dirLight->GetDirection();

							// Create widgets
							ImGui::Text("Direction");
							ImGui::SliderFloat("X##direction", &lightDir.x, -1.0f, 1.0f);
							ImGui::SliderFloat("Y##direction", &lightDir.y, -1.0f, 1.0f);
							ImGui::SliderFloat("Z##direction", &lightDir.z, -1.0f, 1.0f);
			
							// Update the direction
							dirLight->SetDirection(lightDir);							
						}

						/* Options common to both lights */

						// Modify colour
						float colour[3];

						// Get the light's current colour
						const glm::vec3 lightColour = selectedLight->GetColour();
						colour[0] = lightColour.r;
						colour[1] = lightColour.g;
						colour[2] = lightColour.b;

						// Create widget
						ImGui::ColorEdit3("Colour", colour);
						
						// Keep the colour value in the valid range
						ClampColour(colour);
						selectedLight->SetColour(glm::vec3(colour[0], colour[1], colour[2]));

						// Modify intensity
						float intensity = selectedLight->GetIntensity();
						ImGui::SliderFloat("Intensity", &intensity, 0.0f, 1.0f);
						
						// Clamp value and set						
						selectedLight->SetIntensity(std::clamp(intensity, 0.0f, 1.0f));
					}
				}
				// End of tab
				ImGui::EndTabItem();
			}
		}
		ImGui::EndTabBar();
	}
	ImGui::End();

	// ImGui Rendering
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}