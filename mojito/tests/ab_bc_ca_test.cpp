#include <gtest/gtest.h>
#include <cmath>
#include "mojito/mojito.hpp"

namespace {

using namespace mojito;

static constexpr real_t k_epsilon = real_t{1e-6};

TEST(AbBcCaTest, ConstructionAndAccessors)
{
    ab_bc_ca<real_t> line(real_t{1.5}, real_t{0.0}, real_t{-1.5});
    EXPECT_NEAR(line.ab(), real_t{1.5}, k_epsilon);
    EXPECT_NEAR(line.bc(), real_t{0.0}, k_epsilon);
    EXPECT_NEAR(line.ca(), real_t{-1.5}, k_epsilon);
}

}  // namespace
