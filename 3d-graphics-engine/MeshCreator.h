#ifndef MESH_CREATOR_H
#define MESH_CREATOR_H

class Mesh;

class MeshCreator
{
public:
	static Mesh* CreateCube();
	static Mesh* CreatePyramid();
	static Mesh* CreatePlane(int textureSquared = 1);

	// The number of triangles in this plane depends on the size.
	// Additionally, this plane does not have texture coordinate data
	static Mesh* CreateUntexturablePlane(int size);
private:
	MeshCreator() = delete;
};

#endif