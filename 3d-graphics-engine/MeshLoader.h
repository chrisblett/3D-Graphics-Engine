#ifndef MESH_LOADER_H
#define MESH_LOADER_H

#include <string>

class Mesh;

Mesh* LoadMesh(const std::string& filepath);

#endif