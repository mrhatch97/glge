/// <summary>Procedural random terrain generation functions.</summary>
///
/// \file terrain.h

#pragma once

#include <glge/common.h>
#include <glge/util/util.h>

#include <cstdint>

namespace glge::proc_gen
{
  /// <summary>
  /// Generates a heightmap using the diamond-square algorithm.
  /// </summary>
  /// <param name="seed">
  /// 32-bit value to seed the random number generator.
  /// </param>
  /// <param name="n">
  /// Controls the size of the generated terrain. The generated heightmap will
  /// be (2^n)+1 points square.
  /// </param>
  /// <param name="variation">
  /// Controls the elevation variation of the map. Higher values result in more 
  /// varied elevations.
  /// </param>
  /// <param name="top_left">Value for top-left corner of the map.</param>
  /// <param name="top_right">Value for top-right corner of the map.</param>
  /// <param name="bottom_left">Value for bottom-left corner of the map.</param>
  /// <param name="bottom_right">
  /// Value for bottom-right corner of the map.
  /// </param>
  [[nodiscard]] util::Matrix<float> 
    diamond_square_gen(
      std::uint32_t seed,
      size_t n, 
      float variation, 
      float top_left = 0.0f, 
      float top_right = 0.0f, 
      float bottom_left = 0.0f, 
      float bottom_right = 0.0f);
}
