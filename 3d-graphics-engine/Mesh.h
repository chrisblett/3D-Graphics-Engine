#ifndef MESH_H
#define MESH_H

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <vector>
#include <array>

class Mesh
{
	static constexpr int FLOATS_PER_POSITION = 3;
	static constexpr int FLOATS_PER_NORMAL = 3;
	static constexpr int FLOATS_PER_TEXCOORD = 2;

	enum BufferType
	{
		INDEX_BUFFER,
		POSITION_BUFFER,
		NORMAL_BUFFER,
		TEXCOORD_BUFFER,

		MAX_BUFFERS
	};

	enum VertexAttrib
	{
		POSITION_ATTRIB,
		NORMAL_ATTRIB,
		TEXCOORD_ATTRIB
	};

	std::array<GLuint, MAX_BUFFERS> m_buffers;
	GLuint m_vaoID;
	GLsizei m_numIndices;

public:
	Mesh(const std::vector<glm::vec3>& positions,
		const std::vector<GLuint>& indices);

	Mesh(const std::vector<glm::vec3>& positions, const std::vector<glm::vec2>& texCoords,
		const std::vector<GLuint>& indices);

	Mesh(const std::vector<glm::vec3>& positions, const std::vector<glm::vec2>& texCoords,
		const std::vector<glm::vec3>& normals, const std::vector<GLuint>& indices);

	Mesh(const std::vector<glm::vec3>& positions, const std::vector<glm::vec3>& normals,
		const std::vector<GLuint>& indices);

	~Mesh();

	void Render(GLenum primitiveMode = GL_TRIANGLES) const;
private:
	template <typename T>
	void SendVertexData(const std::vector<T>& data, BufferType bufferType,
		VertexAttrib vertexAttrib, const int numFloats) const;

	void SendIndices(const std::vector<GLuint>& indices) const;
};

template <typename T>
void Mesh::SendVertexData(const std::vector<T>& data, BufferType bufferType,
	VertexAttrib vertexAttrib, const int numFloats) const
{
	glBindVertexArray(m_vaoID);

	glBindBuffer(GL_ARRAY_BUFFER, m_buffers[bufferType]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(data[0]) * data.size(),
		&data[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(vertexAttrib);
	glVertexAttribPointer(vertexAttrib, numFloats,
		GL_FLOAT, GL_FALSE, 0, nullptr);
}

#endif