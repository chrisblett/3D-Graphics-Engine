#include "Mesh.h"
#include "Model.h"
#include <GL\glew.h>
#include <cassert>

Model::Model(Mesh* pMesh)
	: m_pMesh{ pMesh }
{
	assert(m_pMesh && "Mesh is NULL");
}

void Model::Render()
{
	m_pMesh->Render(GL_TRIANGLES);
}