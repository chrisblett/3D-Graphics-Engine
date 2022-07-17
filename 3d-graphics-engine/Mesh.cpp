#include "Debug.h"
#include "Mesh.h"
#include "MeshUtil.h"
#include <GL\glew.h>
#include <glm\glm.hpp>
#include <vector>

Mesh::Mesh(const std::vector<glm::vec3>& positions, const std::vector<GLuint>& indices)
	: m_numIndices{ static_cast<GLsizei>(indices.size()) }
{
	auto normals = MeshUtil::GenerateVertexNormals(positions, indices);

	glGenVertexArrays(1, &m_vaoID);
	glBindVertexArray(m_vaoID);

	glGenBuffers(MAX_BUFFERS, &m_buffers[0]);

	SendVertexData(positions, POSITION_BUFFER, POSITION_ATTRIB, FLOATS_PER_POSITION);
	SendVertexData(normals, NORMAL_BUFFER, NORMAL_ATTRIB, FLOATS_PER_NORMAL);
	SendIndices(indices);

	//Debug::Mesh::CreateNormals(positions, normals);
}

Mesh::Mesh(const std::vector<glm::vec3>& positions, const std::vector<glm::vec2>& texCoords,
	const std::vector<GLuint>& indices)
	: Mesh{ positions, indices }
{
	SendVertexData(texCoords, TEXCOORD_BUFFER, TEXCOORD_ATTRIB, FLOATS_PER_TEXCOORD);
}

Mesh::Mesh(const std::vector<glm::vec3>& positions, const std::vector<glm::vec2>& texCoords,
	const std::vector<glm::vec3>& normals, const std::vector<GLuint>& indices)
{
	m_numIndices = indices.size();
	
	assert(m_numIndices != 0);

	glGenVertexArrays(1, &m_vaoID);
	glBindVertexArray(m_vaoID);

	glGenBuffers(MAX_BUFFERS, &m_buffers[0]);

	SendVertexData(positions, POSITION_BUFFER, POSITION_ATTRIB, FLOATS_PER_POSITION);
	SendVertexData(texCoords, TEXCOORD_BUFFER, TEXCOORD_ATTRIB, FLOATS_PER_TEXCOORD);
	SendVertexData(normals, NORMAL_BUFFER, NORMAL_ATTRIB, FLOATS_PER_NORMAL);
	SendIndices(indices);
}

Mesh::Mesh(const std::vector<glm::vec3>& positions, const std::vector<glm::vec3>& normals, const std::vector<GLuint>& indices)
{
	m_numIndices = indices.size();

	assert(m_numIndices != 0);

	glGenVertexArrays(1, &m_vaoID);
	glBindVertexArray(m_vaoID);

	glGenBuffers(MAX_BUFFERS, &m_buffers[0]);

	SendVertexData(positions, POSITION_BUFFER, POSITION_ATTRIB, FLOATS_PER_POSITION);	
	SendVertexData(normals, NORMAL_BUFFER, NORMAL_ATTRIB, FLOATS_PER_NORMAL);
	SendIndices(indices);
}

Mesh::~Mesh()
{
	glDeleteBuffers(MAX_BUFFERS, &m_buffers[0]);
	glDeleteVertexArrays(1, &m_vaoID);
}

void Mesh::Render(GLenum primitiveMode) const
{
	glBindVertexArray(m_vaoID);

	glDrawElements(primitiveMode, m_numIndices, GL_UNSIGNED_INT, nullptr);
}

void Mesh::SendIndices(const std::vector<GLuint>& indices) const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffers[INDEX_BUFFER]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(),
		&indices[0], GL_STATIC_DRAW);
}