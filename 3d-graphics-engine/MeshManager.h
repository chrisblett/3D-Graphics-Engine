#ifndef MESH_MANAGER_H
#define MESH_MANAGER_H

class Mesh;

#include <map>
#include <string>

// This class is responsible for the management of meshes and freeing meshes when the 
// lifetime of an instance of this class ends. Users can retrieve a reference to a mesh by name
class MeshManager
{
	std::map<std::string, Mesh*> m_meshes;

public:
	~MeshManager();

	void CreateMesh(const std::string& filename);
	void CreateMesh(const std::string& name, Mesh* pMesh);

	Mesh* GetMesh(const std::string& name) const;
};
#endif
