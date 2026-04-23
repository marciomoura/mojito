#include <gtest/gtest.h>

#include "mojito/angle_wrapped.hpp"
#include "mojito/constants.hpp"
#include "mojito/reference_frame.hpp"
#include "mojito/types.hpp"

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

TEST(ReferenceFrameTest, AbcDefaultConstructor)
{
    abc<real_t> coords;
    expect_array_near_abc(coords, {real_t{0.0}, real_t{0.0}, real_t{0.0}});
}

TEST(ReferenceFrameTest, AbcArrayConstructor)
{
    abc<real_t> coords(balanced_abc);
    expect_array_near_abc(coords, balanced_abc);
}

TEST(ReferenceFrameTest, AbcValueConstructor)
{
    abc<real_t> coords(real_t{1.0}, real_t{-0.5}, real_t{-0.5});
    expect_array_near_abc(coords, balanced_abc);
}

TEST(ReferenceFrameTest, AbcToAlphabeta)
{
    abc<real_t> abc_coords(balanced_abc);
    auto ab_coords = abc_coords.to_alphabeta();
    expect_array_near_ab(ab_coords, expected_alphabeta);
}

TEST(ReferenceFrameTest, AbcAdditionAndSubtraction)
{
    abc<real_t> a{real_t{1.0}, real_t{2.0}, real_t{3.0}};
    abc<real_t> b{real_t{4.0}, real_t{5.0}, real_t{6.0}};

    abc<real_t> sum = a + b;
    EXPECT_NEAR(sum[0], real_t{5.0}, k_epsilon);
    EXPECT_NEAR(sum[1], real_t{7.0}, k_epsilon);
    EXPECT_NEAR(sum[2], real_t{9.0}, k_epsilon);

    abc<real_t> diff = b - a;
    EXPECT_NEAR(diff[0], real_t{3.0}, k_epsilon);
    EXPECT_NEAR(diff[1], real_t{3.0}, k_epsilon);
    EXPECT_NEAR(diff[2], real_t{3.0}, k_epsilon);
}

TEST(ReferenceFrameTest, AbcUnaryNegation)
{
    abc<real_t> abc_coords{real_t{1.0}, real_t{-0.5}, real_t{-0.5}};
    abc<real_t> neg_abc = -abc_coords;
    EXPECT_NEAR(neg_abc[0], real_t{-1.0}, k_epsilon);
    EXPECT_NEAR(neg_abc[1], real_t{0.5}, k_epsilon);
    EXPECT_NEAR(neg_abc[2], real_t{0.5}, k_epsilon);
}

TEST(ReferenceFrameTest, AbcScalarOperations)
{
    abc<real_t> a(real_t{1.0}, real_t{2.0}, real_t{3.0});
    abc<real_t> b = a * real_t{3.0};
    expect_array_near_abc(b, {real_t{3.0}, real_t{6.0}, real_t{9.0}});

    abc<real_t> c = a / real_t{2.0};
    expect_array_near_abc(c, {real_t{0.5}, real_t{1.0}, real_t{1.5}});
}

TEST(ReferenceFrameTest, ConversionAlphaBetaToAbc)
{
    alphabeta<real_t> ab_coords(expected_alphabeta);
    abc<real_t> abc_coords = ab_coords.to_abc();
    expect_array_near_abc(abc_coords, balanced_abc);
}

TEST(ReferenceFrameTest, ConversionAlphaBetaToDq)
{
    alphabeta<real_t> ab_coords(expected_alphabeta);
    auto dq_coords = ab_coords.to_dq(angle_wrapped(real_t{0.0}));
    expect_array_near_dq(dq_coords, expected_dq);
}

TEST(ReferenceFrameTest, MagnitudeAndPhase)
{
    alphabeta<real_t> ab(real_t{1.0}, real_t{1.0});
    EXPECT_NEAR(ab.magnitude(), std::sqrt(real_t{2.0}), k_epsilon);
    EXPECT_NEAR(ab.phase().get_radians().value(), half_pi / real_t{2.0}, k_epsilon);

    dq<real_t> dq_coords(real_t{1.0}, real_t{1.0});
    EXPECT_NEAR(dq_coords.magnitude(), std::sqrt(real_t{2.0}), k_epsilon);
    EXPECT_NEAR(dq_coords.phase(angle_wrapped(real_t{0.0})).get_radians().value(), half_pi / real_t{2.0}, k_epsilon);
}

TEST(ReferenceFrameTest, Rotation)
{
    alphabeta<real_t> ab = {real_t{1.0}, real_t{0.0}};
    ab = ab.rotate_counter_clockwise(angle_wrapped(half_pi));
    EXPECT_NEAR(ab[0], real_t{0.0}, k_epsilon);
    EXPECT_NEAR(ab[1], real_t{1.0}, k_epsilon);

    ab = ab.rotate_clockwise(angle_wrapped(half_pi));
    EXPECT_NEAR(ab[0], real_t{1.0}, k_epsilon);
    EXPECT_NEAR(ab[1], real_t{0.0}, k_epsilon);
}

TEST(ReferenceFrameTest, FullConversionCycle)
{
    abc<real_t> start_abc(balanced_abc);
    auto ab = start_abc.to_alphabeta();
    auto dq = ab.to_dq(angle_wrapped(real_t{0.0}));
    auto back_ab = dq.to_alphabeta(angle_wrapped(real_t{0.0}));
    abc<real_t> back_abc = back_ab.to_abc();

    expect_array_near_abc(back_abc, start_abc.array());
}

}  // namespace
