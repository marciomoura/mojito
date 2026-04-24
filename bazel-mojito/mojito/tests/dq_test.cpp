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

}  // namespace
