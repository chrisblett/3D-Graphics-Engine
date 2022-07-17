#ifndef DEBUG_H
#define DEBUG_H

#include <glm\glm.hpp>
#include <vector>

namespace Debug
{
	namespace Mesh
	{
		std::vector<glm::vec3> GenerateNormalLines(const std::vector<glm::vec3>& positions, const std::vector<glm::vec3>& endpoints);
		void CreateNormals(const std::vector<glm::vec3>& positions, const std::vector<glm::vec3>& normals);
	}
}

#endif
