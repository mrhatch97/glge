#include "glge/common.h"

namespace glge
{
	std::mutex stdio_write_lock;
}

// Requires GLM 0.9.9.x - not always supported
#if 0
template struct glm::vec<2, float, glm::packed_highp>;
template struct glm::vec<3, float, glm::packed_highp>;
template struct glm::vec<4, float, glm::packed_highp>;
template struct glm::mat<4, 4, float, glm::packed_highp>;
template struct glm::qua<float, glm::packed_highp>;

template float glm::dot<>(const vec3 &, const vec3 &);
template float glm::radians<>(float);
#endif