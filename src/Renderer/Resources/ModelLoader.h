#pragma once

#include <glge/Common.h>
#include <glge/Renderer/Resources/ResourceData.h>

namespace eng::render::res
{
	uptr<ModelData> load_model(const ModelFileInfo & fileinfo);
}