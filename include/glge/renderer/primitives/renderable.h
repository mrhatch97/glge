#pragma once

namespace glge::renderer::primitive
{
	class Renderable
	{
	public:
		virtual void render() const = 0;

		virtual ~Renderable() = default;
	};
}