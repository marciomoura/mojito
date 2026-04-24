#include <gtest/gtest.h>
#include "mojito/mojito.hpp"

namespace {

using namespace mojito;

static constexpr real_t k_epsilon = real_t{1e-6};

TEST(DqTest, MagnitudeAndPhase)
{
    dq<real_t> dq_coords(real_t{1.0}, real_t{1.0});
    EXPECT_NEAR(dq_coords.magnitude(), std::sqrt(real_t{2.0}), k_epsilon);
    EXPECT_NEAR(dq_coords.phase(angle_wrapped(real_t{0.0})).get_radians().value(), half_pi / real_t{2.0}, k_epsilon);
}

TEST(DqTest, Rotation)
{
    dq<real_t> dq_val = {real_t{1.0}, real_t{0.0}};
    dq_val = dq_val.rotate_counter_clockwise(angle_wrapped(half_pi));
    EXPECT_NEAR(dq_val.d(), real_t{0.0}, k_epsilon);
    EXPECT_NEAR(dq_val.q(), real_t{1.0}, k_epsilon);

    dq_val = dq_val.rotate_clockwise(angle_wrapped(half_pi));
    EXPECT_NEAR(dq_val.d(), real_t{1.0}, k_epsilon);
    EXPECT_NEAR(dq_val.q(), real_t{0.0}, k_epsilon);
}

}  // namespace
