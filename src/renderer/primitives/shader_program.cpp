#include "glge/renderer/primitives/shader_program.h"

#include <glge/common.h>

namespace glge::renderer::primitive
{
	ShaderManager::ShaderManager() = default;

	ShaderManager::~ShaderManager() = default;

	ShaderInstanceBase::ShaderInstanceBase(ShaderBase & shader) : shader(shader)
	{}
}   // namespace glge::renderer::primitive
