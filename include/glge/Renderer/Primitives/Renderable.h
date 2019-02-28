#pragma once

namespace eng::render::primitive
{
	class Renderable
	{
	public:
		virtual void render() const = 0;

		virtual ~Renderable() = default;
	};
}