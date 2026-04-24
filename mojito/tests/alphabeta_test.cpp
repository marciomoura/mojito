#include <gtest/gtest.h>
#include "mojito/mojito.hpp"

namespace {

using namespace mojito;

static constexpr real_t k_epsilon = real_t{1e-6};

TEST(AlphabetaTest, MagnitudeAndPhase)
{
    alphabeta<real_t> ab(real_t{1.0}, real_t{1.0});
    EXPECT_NEAR(ab.magnitude(), std::sqrt(real_t{2.0}), k_epsilon);
    EXPECT_NEAR(ab.phase().get_radians().value(), half_pi / real_t{2.0}, k_epsilon);
}

TEST(AlphabetaTest, Rotation)
{
    alphabeta<real_t> ab = {real_t{1.0}, real_t{0.0}};
    ab = ab.rotate_counter_clockwise(angle_wrapped(half_pi));
    EXPECT_NEAR(ab[0], real_t{0.0}, k_epsilon);
    EXPECT_NEAR(ab[1], real_t{1.0}, k_epsilon);

    ab = ab.rotate_clockwise(angle_wrapped(half_pi));
    EXPECT_NEAR(ab[0], real_t{1.0}, k_epsilon);
    EXPECT_NEAR(ab[1], real_t{0.0}, k_epsilon);
}

}  // namespace
