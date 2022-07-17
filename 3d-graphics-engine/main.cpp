#include "Entity.h"
#include "GraphicsEngine.h"
#include "Light.h"
#include "Mesh.h"
#include "MeshCreator.h"
#include "MeshManager.h"
#include "MeshUtil.h"
#include "Texture.h"
#include "TextureManager.h"
#include "TexturedModel.h"
#include <glm\glm.hpp>
#include <cstdlib>
#include <iostream>

extern GLuint g_normalsVaoID;

int main(int argc, char* argv[])
{
	Window window = Window("3D Graphics Engine",
		Constants::DEFAULT_WINDOW_WIDTH, Constants::DEFAULT_WINDOW_HEIGHT);

	if(!window.Initialise())
	{
		std::cerr << "Window failed to initialise!\n";
		return EXIT_FAILURE;
	}

	GraphicsEngine graphicsEngine(&window);

	// Create shaders
	graphicsEngine.AddShader("flat-colour");
	graphicsEngine.AddShader("phong");
	graphicsEngine.AddShader("phong-notexture");
	graphicsEngine.AddShader("skybox");
	graphicsEngine.AddShader("shadow-map");
	graphicsEngine.AddShader("debug-quad");

	// Create meshes
	graphicsEngine.AddMesh("teapot2.obj");

	// Create textures
	graphicsEngine.AddTexture("checker.png");
	graphicsEngine.AddTexture("Tiles095_1K_Color.jpg");

	// Create and configure entities

	// Ground plane
	Entity* plane = graphicsEngine.CreateEntity("plane");
	plane->SetScale(5);
	
	Material* planeMat = plane->GetMaterial();
	planeMat->SetDiffuseMap(graphicsEngine.GetTexture("Tiles095_1K_Color.jpg"));

	Entity* teapot = graphicsEngine.CreateEntity("teapot2.obj");
	Material* teapotMat = teapot->GetMaterial();
	teapotMat->SetDiffuseMap(graphicsEngine.GetTexture("checker.png"));

	// Configure environment and lighting
	//graphicsEngine.SetSkybox("cubemap");
	graphicsEngine.SetGlobalAmbientLight({ 0.1f, 0.1f, 0.1f });
	
	graphicsEngine.CreatePointLight(glm::vec3(-3.0f, 3.0f, 3.0f), { 0.0f, 1.0f, 0.0f }, 1.0f);	
	graphicsEngine.CreatePointLight(glm::vec3( 3.0f, 3.0f, 0.0f), { 0.0f, 0.0f, 1.0f }, 1.0f);
	graphicsEngine.CreateDirectionalLight(glm::vec3(0.0f, -1.0f, 0.0f),
		glm::vec3(1.0f, 1.0f, 1.0f), 0.5f);	
	
	// Run the engine
	graphicsEngine.Run();

	return EXIT_SUCCESS;
}
