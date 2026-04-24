#pragma once

#include "types.hpp"

namespace mojito {

/**
 * @brief Mathematical constants for mojito library.
 * Using static constexpr with explicit real_t construction to ensure 
 * precision matches the precision of real_t (float or double).
 */
inline constexpr real_t pi = real_t{3.14159265358979323846};
inline constexpr real_t two_pi = real_t{2.0 * pi};
inline constexpr real_t half_pi = real_t{0.5 * pi};
inline constexpr real_t two_pi_thirds = real_t{2.0 * pi / 3.0};
inline constexpr real_t sqrt_2 = real_t{1.41421356237309504880};
inline constexpr real_t sqrt_3 = real_t{1.73205080756887729352};

}  // namespace mojito
