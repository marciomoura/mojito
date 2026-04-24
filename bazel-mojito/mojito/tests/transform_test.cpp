#include <gtest/gtest.h>
#include "mojito/mojito.hpp"

namespace {

using namespace mojito;

static constexpr real_t k_epsilon = real_t{1e-6};

void expect_array_near_abc(const abc<real_t>& a, const std::array<real_t, 3>& b)
{
    for (size_t i = 0; i < 3; ++i) {
        EXPECT_NEAR(a[i], b[i], k_epsilon) << "Arrays differ at index " << i;
    }
}

void expect_array_near_ab(const alphabeta<real_t>& a, const std::array<real_t, 2>& b)
{
    for (size_t i = 0; i < 2; ++i) {
        EXPECT_NEAR(a[i], b[i], k_epsilon) << "Arrays differ at index " << i;
    }
}

void expect_array_near_dq(const dq<real_t>& a, const std::array<real_t, 2>& b)
{
    for (size_t i = 0; i < 2; ++i) {
        EXPECT_NEAR(a[i], b[i], k_epsilon) << "Arrays differ at index " << i;
    }
}

const std::array<real_t, 3> balanced_abc = {real_t{1.0}, real_t{-0.5}, real_t{-0.5}};
const std::array<real_t, 2> expected_alphabeta = {real_t{1.0}, real_t{0.0}};
const std::array<real_t, 2> expected_dq = {real_t{1.0}, real_t{0.0}};

TEST(TransformTest, AbcToAlphabeta)
{
    abc<real_t> abc_coords(balanced_abc);
    auto ab_coords = to_alphabeta(abc_coords);
    expect_array_near_ab(ab_coords, expected_alphabeta);
}

TEST(TransformTest, AlphaBetaToAbc)
{
    alphabeta<real_t> ab_coords(expected_alphabeta);
    abc<real_t> abc_coords = to_abc(ab_coords);
    expect_array_near_abc(abc_coords, balanced_abc);
}

TEST(TransformTest, AlphaBetaToDq)
{
    alphabeta<real_t> ab_coords(expected_alphabeta);
    auto dq_coords = to_dq(ab_coords, angle_wrapped(real_t{0.0}));
    expect_array_near_dq(dq_coords, expected_dq);
}

TEST(TransformTest, FullConversionCycle)
{
    abc<real_t> start_abc(balanced_abc);
    auto ab = to_alphabeta(start_abc);
    auto dq_val = to_dq(ab, angle_wrapped(real_t{0.0}));
    auto back_ab = to_alphabeta(dq_val, angle_wrapped(real_t{0.0}));
    abc<real_t> back_abc = to_abc(back_ab);

    expect_array_near_abc(back_abc, start_abc.array());
}

TEST(TransformTest, AbBcCa)
{
    abc<real_t> phase(real_t{1.0}, real_t{-0.5}, real_t{-0.5});
    ab_bc_ca<real_t> line = to_ab_bc_ca(phase);
    EXPECT_NEAR(line.ab(), real_t{1.5}, k_epsilon);
    EXPECT_NEAR(line.bc(), real_t{0.0}, k_epsilon);
    EXPECT_NEAR(line.ca(), real_t{-1.5}, k_epsilon);

    auto back = to_phase_balanced(line);
    expect_array_near_abc(back, phase.array());
}

}  // namespace
