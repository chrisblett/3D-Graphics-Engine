#include "MeshManager.h"
#include "MeshLoader.h"
#include "Mesh.h"
#include "Constants.h"
#include <cassert>
#include <iostream>
#include <string>

MeshManager::~MeshManager()
{
	// Free all meshes
	std::cout << "Freeing all meshes\n";

	for(const auto& elem : m_meshes)
	{
		delete elem.second;
	}
}

void MeshManager::CreateMesh(const std::string& filename)
{
	CreateMesh(filename, LoadMesh(Constants::MODEL_PATH + filename));
}

void MeshManager::CreateMesh(const std::string& name, Mesh* pMesh)
{
	printf("Creating mesh '%s'\n", name.c_str());
	auto it = m_meshes.find(name);
	if(it != m_meshes.end())
	{
		std::cerr << "A mesh already exists with the name '" << name << "'\n";
		assert(false);
	}
	else
	{
		m_meshes.insert({ name, pMesh });
	}
}

Mesh* MeshManager::GetMesh(const std::string& name) const
{
	Mesh* pMesh = nullptr;

	auto it = m_meshes.find(name);
	if(it != m_meshes.end())
	{
		pMesh = (*it).second;
	}
	else
	{
		std::cerr << "Failed to retrieve mesh with name '" << name << "'\n";
	}

	assert(pMesh && "Mesh is NULL");

	return pMesh;
}