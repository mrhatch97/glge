#pragma once

#include <glge/Common.h>
#include <glge/Util/Util.h>

#include <optional>
#include <random>
#include <tuple>

namespace eng
{
	namespace procgen
	{
		using Engine = std::default_random_engine;
		using Distribution = std::uniform_real_distribution<float>;
		using RNG = std::function<Distribution::result_type()>;
		using grid_pt = std::tuple<size_t, size_t>;
		using step_pts = std::tuple<std::optional<float>, std::optional<float>,
			std::optional<float>, std::optional<float>>;
		using namespace util;

		void diamond_square(std::seed_seq & seed, Matrix<float> & grid, float variation);

		void diamond_step(const RNG & rng, Matrix<float> & grid, size_t radius);
		void square_step(const RNG & rng, Matrix<float> & grid, size_t radius);

		RNG::result_type aggregate(const std::function<step_pts(grid_pt)> & pts,
			const RNG & rng, grid_pt center);

		step_pts get_diamond(const Matrix<float> & grid, size_t radius, grid_pt center);
		step_pts get_square(const Matrix<float> & grid, size_t radius, grid_pt center);
		
		// Computes the average of the values in a tuple of std::optional, completely excluding
		// nullopt from the calculation
		template<typename... Args>
		[[nodiscard]] constexpr auto average(const std::tuple<Args...> vals) noexcept
		{
			auto ct = std::tuple_size<decltype(vals)>::value;
			auto extract = [&ct](auto next) 
			{
				// Return 0 for Nothing, value for Just, decrement ct if Nothing
				return (next.has_value() ? ct : ct--, next.value_or(0));
			};
			auto sum = [](auto... xs) { return (xs + ...); };

			return std::apply([&](auto... v) { return sum(extract(v)...); }, vals) / ct;
		}

		[[nodiscard]] std::optional<float> grid_get_safe(const Matrix<float> & grid, size_t x, size_t y);
	}
}