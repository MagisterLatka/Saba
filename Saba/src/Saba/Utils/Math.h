#pragma once

namespace Saba::Math {

	bool Decompose(const glm::mat4& matrix, glm::vec3& translation, glm::vec3& rotation, glm::vec3& scale);
}
