#include "mojito/reference_frame.hpp"

#include <gtest/gtest.h>

#include "mojito/angle_wrapped.hpp"
// #include "mojito/constants.hpp"
// #include "mojito/three_phase_waveform_generator.hpp"
#include "mojito/types.hpp"
// #include "mojito_testing/simulator_gtest.hpp"

namespace {

using namespace mojito;

static constexpr real_t k_epsilon = 1e-6;

// Helper functions to compare values within epsilon
void expect_near(real_t a, real_t b) { ASSERT_NEAR(a, b, k_epsilon); }

template <size_t n>
void expect_array_near(const std::array<real_t, n>& a, const std::array<real_t, n>& b)
{
    for (size_t i = 0; i < n; ++i) {
        ASSERT_NEAR(a[i], b[i], k_epsilon) << "Arrays differ at index " << i;
    }
}

// Common test values
const std::array<real_t, 3> balanced_abc = {1.0, -0.5, -0.5};
const std::array<real_t, 2> expected_alphabeta = {1.0, 0.0};
const std::array<real_t, 2> expected_dq = {1.0, 0.0};

// ABC Tests
TEST(ReferenceFrameTest, AbcDefaultConstructor)
{
    abc<real_t> coords;
    expect_array_near(coords.array(), {0.0, 0.0, 0.0});
}

TEST(ReferenceFrameTest, AbcArrayConstructor)
{
    abc<real_t> coords(balanced_abc);
    expect_array_near(coords.array(), balanced_abc);
}

TEST(ReferenceFrameTest, AbcValueConstructor)
{
    abc<real_t> coords(1.0, -0.5, -0.5);
    expect_array_near(coords.array(), balanced_abc);
}

TEST(ReferenceFrameTest, AbcArrayAccess)
{
    abc<real_t> coords(balanced_abc);
    expect_near(coords[0], 1.0);
    expect_near(coords[1], -0.5);
    expect_near(coords[2], -0.5);
}

TEST(ReferenceFrameTest, AbcToAlphabeta)
{
    abc<real_t> abc_coords(balanced_abc);
    auto ab_coords = abc_coords.to_alphabeta();
    expect_array_near(ab_coords.array(), expected_alphabeta);
}

TEST(ReferenceFrameTest, AbcAdditionAndSubtractionOperators)
{
    abc<real_t> a{1.0, 2.0, 3.0};
    abc<real_t> b{4.0, 5.0, 6.0};

    abc<real_t> sum = a + b;
    expect_near(sum[0], 5.0);
    expect_near(sum[1], 7.0);
    expect_near(sum[2], 9.0);

    abc<real_t> diff = b - a;
    expect_near(diff[0], 3.0);
    expect_near(diff[1], 3.0);
    expect_near(diff[2], 3.0);
}

TEST(ReferenceFrameTest, AbcUnaryNegationOperator)
{
    abc<real_t> abc_coords{1.0, -0.5, -0.5};
    abc<real_t> neg_abc = -abc_coords;
    expect_near(neg_abc[0], -1.0);
    expect_near(neg_abc[1], 0.5);
    expect_near(neg_abc[2], 0.5);
}

TEST(ReferenceFrameTest, AbcCompoundOperators)
{
    abc<real_t> a(1.0, 2.0, 3.0);
    abc<real_t> b(4.0, 5.0, 6.0);

    a += b;
    expect_array_near(a.array(), std::array<real_t, 3>{5.0, 7.0, 9.0});

    a -= b;
    expect_array_near(a.array(), std::array<real_t, 3>{1.0, 2.0, 3.0});

    a *= 2.0;
    expect_array_near(a.array(), std::array<real_t, 3>{2.0, 4.0, 6.0});

    a /= 2.0;
    expect_array_near(a.array(), std::array<real_t, 3>{1.0, 2.0, 3.0});
}

TEST(ReferenceFrameTest, AbcScalarMultiplicationAndDivision)
{
    abc<real_t> a(1.0, 2.0, 3.0);

    abc<real_t> b = a * 3.0;
    expect_array_near(b.array(), std::array<real_t, 3>{3.0, 6.0, 9.0});

    abc<real_t> c = a / 2.0;
    expect_array_near(c.array(), std::array<real_t, 3>{0.5, 1.0, 1.5});
}

TEST(ReferenceFrameTest, AbcIteratorAndSize)
{
    abc<real_t> coords{1.0, 2.0, 3.0};

    // Test size
    EXPECT_EQ(coords.size(), 3);

    // Test non-const iterator
    std::vector<real_t> values;
    for (auto& value : coords) {
        values.push_back(value);
    }
    EXPECT_EQ(values.size(), 3);
    EXPECT_NEAR(values[0], 1.0, k_epsilon);
    EXPECT_NEAR(values[1], 2.0, k_epsilon);
    EXPECT_NEAR(values[2], 3.0, k_epsilon);

    // Test const iterator
    const abc<real_t> const_coords{1.0, 2.0, 3.0};
    values.clear();
    for (const auto& value : const_coords) {
        values.push_back(value);
    }
    EXPECT_EQ(values.size(), 3);
    EXPECT_NEAR(values[0], 1.0, k_epsilon);
    EXPECT_NEAR(values[1], 2.0, k_epsilon);
    EXPECT_NEAR(values[2], 3.0, k_epsilon);
}

// Alphabeta Tests
TEST(ReferenceFrameTest, AlphabetaDefaultConstructor)
{
    alphabeta<real_t> coords;
    expect_array_near(coords.array(), {0.0, 0.0});
}

TEST(ReferenceFrameTest, AlphabetaArrayConstructor)
{
    alphabeta<real_t> coords(expected_alphabeta);
    expect_array_near(coords.array(), expected_alphabeta);
}

TEST(ReferenceFrameTest, AlphabetaValueConstructor)
{
    alphabeta<real_t> coords(1.0, 0.0);
    expect_array_near(coords.array(), expected_alphabeta);
}

TEST(ReferenceFrameTest, ConversionAlphaBetaToAbc)
{
    alphabeta<real_t> ab_coords(expected_alphabeta);
    abc<real_t> abc_coords = ab_coords.to_abc();
    expect_array_near(abc_coords.array(), balanced_abc);
}

TEST(ReferenceFrameTest, ConversionAlphaBetaToDq)
{
    alphabeta<real_t> ab_coords(expected_alphabeta);
    auto dq_coords = ab_coords.to_dq(angle_wrapped(0.0));
    expect_array_near(dq_coords.array(), expected_dq);
}

TEST(ReferenceFrameTest, AlphabetaMagnitudeAndPhase)
{
    alphabeta<real_t> coords(1.0, 1.0);
    expect_near(coords.magnitude(), std::sqrt(2.0));
    expect_near(coords.phase().get_radians(), pi / 4);
}

TEST(ReferenceFrameTest, DqMagnitudeAndPHase)
{
    dq<real_t> coords(1.0, 1.0);
    expect_near(coords.magnitude(), std::sqrt(2.0));
    expect_near(coords.phase(angle_wrapped(0.0)).get_radians(), pi / 4);
}

TEST(ReferenceFrameTest, AlphabetaAdditionAndSubtractionOperators)
{
    alphabeta<real_t> a{1.0, 2.0};
    alphabeta<real_t> b{3.0, 4.0};

    alphabeta<real_t> sum = a + b;
    expect_near(sum[0], 4.0);
    expect_near(sum[1], 6.0);

    alphabeta<real_t> diff = b - a;
    expect_near(diff[0], 2.0);
    expect_near(diff[1], 2.0);
}

TEST(ReferenceFrameTest, AlphabetaUnaryNegationOperator)
{
    alphabeta<real_t> ab_coords{1.0, -0.5};
    alphabeta<real_t> neg_ab = -ab_coords;
    expect_near(neg_ab[0], -1.0);
    expect_near(neg_ab[1], 0.5);
}

TEST(ReferenceFrameTest, AlphabetaCompoundOperators)
{
    alphabeta<real_t> a(1.0, 2.0);
    alphabeta<real_t> b(3.0, 4.0);

    a += b;
    expect_array_near(a.array(), std::array<real_t, 2>{4.0, 6.0});

    a -= b;
    expect_array_near(a.array(), std::array<real_t, 2>{1.0, 2.0});

    a *= 2.0;
    expect_array_near(a.array(), std::array<real_t, 2>{2.0, 4.0});

    a /= 2.0;
    expect_array_near(a.array(), std::array<real_t, 2>{1.0, 2.0});
}

TEST(ReferenceFrameTest, AlphabetaScalarMultiplicationAndDivision)
{
    alphabeta<real_t> a(1.0, 2.0);

    alphabeta<real_t> b = a * 3.0;
    expect_array_near(b.array(), std::array<real_t, 2>{3.0, 6.0});

    alphabeta<real_t> c = a / 2.0;
    expect_array_near(c.array(), std::array<real_t, 2>{0.5, 1.0});
}

TEST(ReferenceFrameTest, AlphabetaIteratorAndSize)
{
    alphabeta<real_t> coords{1.0, 2.0};

    // Test size
    EXPECT_EQ(coords.size(), 2);

    // Test non-const iterator
    std::vector<real_t> values;
    for (auto& value : coords) {
        values.push_back(value);
    }
    EXPECT_EQ(values.size(), 2);
    EXPECT_NEAR(values[0], 1.0, k_epsilon);
    EXPECT_NEAR(values[1], 2.0, k_epsilon);

    // Test const iterator
    const alphabeta<real_t> const_coords{1.0, 2.0};
    values.clear();
    for (const auto& value : const_coords) {
        values.push_back(value);
    }
    EXPECT_EQ(values.size(), 2);
    EXPECT_NEAR(values[0], 1.0, k_epsilon);
    EXPECT_NEAR(values[1], 2.0, k_epsilon);
}

// DQ Tests
TEST(ReferenceFrameTest, DqDefaultConstructor)
{
    dq<real_t> coords;
    expect_array_near(coords.array(), {0.0, 0.0});
}

TEST(ReferenceFrameTest, DqArrayConstructor)
{
    dq<real_t> coords(expected_dq);
    expect_array_near(coords.array(), expected_dq);
}

TEST(ReferenceFrameTest, DqValueConstructor)
{
    dq<real_t> coords(1.0, 0.0);
    expect_array_near(coords.array(), expected_dq);
}

TEST(ReferenceFrameTest, DqToAlphabeta)
{
    dq<real_t> dq_coords(expected_dq);
    auto ab_coords = dq_coords.to_alphabeta(angle_wrapped(0.0));
    expect_array_near(ab_coords.array(), expected_alphabeta);
}

TEST(ReferenceFrameTest, DqMagnitude)
{
    dq<real_t> coords(3.0, 4.0);
    expect_near(coords.magnitude(), 5.0);
}

TEST(ReferenceFrameTest, DqAdditionAndSubtractionOperators)
{
    dq<real_t> a{1.0, 2.0};
    dq<real_t> b{3.0, 4.0};

    dq<real_t> sum = a + b;
    expect_near(sum[0], 4.0);
    expect_near(sum[1], 6.0);

    dq<real_t> diff = b - a;
    expect_near(diff[0], 2.0);
    expect_near(diff[1], 2.0);
}

TEST(ReferenceFrameTest, DqUnaryNegationOperator)
{
    dq<real_t> dq_coords{1.0, -0.5};
    dq<real_t> neg_dq = -dq_coords;
    expect_near(neg_dq[0], -1.0);
    expect_near(neg_dq[1], 0.5);
}

TEST(ReferenceFrameTest, DqCompoundOperators)
{
    dq<real_t> a(1.0, 2.0);
    dq<real_t> b(3.0, 4.0);

    a += b;
    expect_array_near(a.array(), std::array<real_t, 2>{4.0, 6.0});

    a -= b;
    expect_array_near(a.array(), std::array<real_t, 2>{1.0, 2.0});

    a *= 2.0;
    expect_array_near(a.array(), std::array<real_t, 2>{2.0, 4.0});

    a /= 2.0;
    expect_array_near(a.array(), std::array<real_t, 2>{1.0, 2.0});
}

TEST(ReferenceFrameTest, DqScalarMultiplicationAndDivision)
{
    dq<real_t> a(1.0, 2.0);

    dq<real_t> b = a * 3.0;
    expect_array_near(b.array(), std::array<real_t, 2>{3.0, 6.0});

    dq<real_t> c = a / 2.0;
    expect_array_near(c.array(), std::array<real_t, 2>{0.5, 1.0});
}

TEST(ReferenceFrameTest, DqIteratorAndSize)
{
    dq<real_t> coords{1.0, 2.0};

    // Test size
    EXPECT_EQ(coords.size(), 2);

    // Test non-const iterator
    std::vector<real_t> values;
    for (auto& value : coords) {
        values.push_back(value);
    }
    EXPECT_EQ(values.size(), 2);
    EXPECT_NEAR(values[0], 1.0, k_epsilon);
    EXPECT_NEAR(values[1], 2.0, k_epsilon);

    // Test const iterator
    const dq<real_t> const_coords{1.0, 2.0};
    values.clear();
    for (const auto& value : const_coords) {
        values.push_back(value);
    }
    EXPECT_EQ(values.size(), 2);
    EXPECT_NEAR(values[0], 1.0, k_epsilon);
    EXPECT_NEAR(values[1], 2.0, k_epsilon);
}

// Make functions tests
TEST(ReferenceFrameTest, MakeAbc)
{
    real_t magnitude = 1.0;
    auto coords = make_abc<real_t>(magnitude, angle_wrapped(0.0));

    // At 0 degrees, phase A should be at magnitude, B and C at -0.5*magnitude
    expect_near(coords[0], magnitude);
    expect_near(coords[1], -0.5 * magnitude);
    expect_near(coords[2], -0.5 * magnitude);
}

TEST(ReferenceFrameTest, MakeAlphabeta)
{
    real_t magnitude = 1.0;
    auto coords = make_alphabeta<real_t>(magnitude, angle_wrapped(0.0));

    // At 0 degrees, alpha should be magnitude, beta should be 0
    expect_near(coords[0], magnitude);
    expect_near(coords[1], 0.0);
}

// Full conversion cycle tests
TEST(ReferenceFrameTest, FullConversionCycle)
{
    // Start with abc
    abc<real_t> start_abc(balanced_abc);

    // Convert through all frames and back
    auto ab = start_abc.to_alphabeta();
    auto dq = ab.to_dq(angle_wrapped(0.0));
    auto back_ab = dq.to_alphabeta(angle_wrapped(0.0));
    abc<real_t> back_abc = back_ab.to_abc();
    ::dq<real_t> back_dq = back_abc.to_dq(angle_wrapped(0.0));

    // Should get back to original values
    expect_array_near(back_abc.array(), start_abc.array());
}

TEST(ReferenceFrameTest, NonMemberScalarMultiplication)
{
    abc<real_t> abc_coords(1.0, 2.0, 3.0);
    abc<real_t> abc_scaled = 2.0 * abc_coords;
    expect_array_near(abc_scaled.array(), std::array<real_t, 3>{2.0, 4.0, 6.0});

    alphabeta<real_t> ab_coords(1.0, 2.0);
    alphabeta<real_t> ab_scaled = 2.0 * ab_coords;
    expect_array_near(ab_scaled.array(), std::array<real_t, 2>{2.0, 4.0});

    dq<real_t> dq_coords(1.0, 2.0);
    dq<real_t> dq_scaled = 2.0f * dq_coords;
    expect_array_near(dq_scaled.array(), std::array<real_t, 2>{2.0, 4.0});
}

TEST(ReferenceFrameTest, AlphaBetaRotateCounterClockwise)
{
    alphabeta<real_t> ab = {1.0, 0.0};

    ab = ab.rotate_counter_clockwise(angle_wrapped(pi / 2));
    // Result is alpha = 0, beta = 1
    expect_near(ab[0], 0.0);
    expect_near(ab[1], 1.0);
    expect_near(ab.magnitude(), 1.0);

    ab = ab.rotate_counter_clockwise(angle_wrapped(pi / 2));
    // Result is alpha = -1, beta = 0
    expect_near(ab[0], -1.0);
    expect_near(ab[1], 0.0);
    expect_near(ab.magnitude(), 1.0);

    ab = ab.rotate_counter_clockwise(angle_wrapped(pi / 2));
    // Result is alpha = 0, beta = -1
    expect_near(ab[0], 0.0);
    expect_near(ab[1], -1.0);
    expect_near(ab.magnitude(), 1.0);

    ab = ab.rotate_counter_clockwise(angle_wrapped(pi / 2));
    // Result is alpha = 1, beta = 0
    expect_near(ab[0], 1.0);
    expect_near(ab[1], 0.0);
    expect_near(ab.magnitude(), 1.0);
}

TEST(ReferenceFrameTest, AlphaBetaRotateClockwise)
{
    alphabeta<real_t> ab = {1.0, 0.0};

    ab = ab.rotate_clockwise(angle_wrapped(pi / 2));
    // Result is alpha = 0, beta = -1
    expect_near(ab[0], 0.0);
    expect_near(ab[1], -1.0);
    expect_near(ab.magnitude(), 1.0);

    ab = ab.rotate_clockwise(angle_wrapped(pi / 2));
    // Result is alpha = -1, beta = 0
    expect_near(ab[0], -1.0);
    expect_near(ab[1], 0.0);
    expect_near(ab.magnitude(), 1.0);

    ab = ab.rotate_clockwise(angle_wrapped(pi / 2));
    // Result is alpha = 0, beta = 1
    expect_near(ab[0], 0.0);
    expect_near(ab[1], 1.0);
    expect_near(ab.magnitude(), 1.0);

    ab = ab.rotate_clockwise(angle_wrapped(pi / 2));
    // Result is alpha = 1, beta = 0
    expect_near(ab[0], 1.0);
    expect_near(ab[1], 0.0);
    expect_near(ab.magnitude(), 1.0);
}

TEST(ReferenceFrameTest, DqRotateCounterClockwise)
{
    dq<real_t> dq_coords{1.0, 0.0};

    // Rotate counter-clockwise by 90 degrees (pi/2)
    dq<real_t> rotated = dq_coords.rotate_counter_clockwise(angle_wrapped(pi / 2));
    expect_near(rotated[0], 0.0);
    expect_near(rotated[1], 1.0);
    expect_near(rotated.magnitude(), 1.0);

    // Another 90° rotation (total 180°)
    rotated = rotated.rotate_counter_clockwise(angle_wrapped(pi / 2));
    expect_near(rotated[0], -1.0);
    expect_near(rotated[1], 0.0);

    // Another 90° rotation (total 270°)
    rotated = rotated.rotate_counter_clockwise(angle_wrapped(pi / 2));
    expect_near(rotated[0], 0.0);
    expect_near(rotated[1], -1.0);

    // Final 90° rotation should return to the original orientation
    rotated = rotated.rotate_counter_clockwise(angle_wrapped(pi / 2));
    expect_near(rotated[0], 1.0);
    expect_near(rotated[1], 0.0);
}

TEST(ReferenceFrameTest, DqRotateClockwise)
{
    dq<real_t> dq_coords{1.0, 0.0};

    // Rotate clockwise by 90 degrees (pi/2)
    dq<real_t> rotated = dq_coords.rotate_clockwise(angle_wrapped(pi / 2));
    expect_near(rotated[0], 0.0);
    expect_near(rotated[1], -1.0);
    expect_near(rotated.magnitude(), 1.0);

    // Another 90° rotation (total 180°)
    rotated = rotated.rotate_clockwise(angle_wrapped(pi / 2));
    expect_near(rotated[0], -1.0);
    expect_near(rotated[1], 0.0);

    // Another 90° rotation (total 270°)
    rotated = rotated.rotate_clockwise(angle_wrapped(pi / 2));
    expect_near(rotated[0], 0.0);
    expect_near(rotated[1], 1.0);

    // Final 90° rotation returns to original orientation
    rotated = rotated.rotate_clockwise(angle_wrapped(pi / 2));
    expect_near(rotated[0], 1.0);
    expect_near(rotated[1], 0.0);
}

TEST(ReferenceFrameTest, AlphaBetaToDQ)
{
    alphabeta<real_t> ab{1.0, 0.0};
    dq<real_t> dq = ab.to_dq(angle_wrapped(0.0));
    expect_near(dq[0], 1.0);
    expect_near(dq[1], 0.0);

    dq = ab.to_dq(angle_wrapped(pi / 2.0));
    expect_near(dq[0], 0.0);
    expect_near(dq[1], -1.0);

    dq = ab.to_dq(angle_wrapped(pi));
    expect_near(dq[0], -1.0);
    expect_near(dq[1], 0.0);

    dq = ab.to_dq(angle_wrapped(3 * pi / 2));
    expect_near(dq[0], 0.0);
    expect_near(dq[1], 1.0);
}

TEST(ReferenceFrameTest, DQToAlphaBeta)
{
    dq<real_t> dq{1.0, 0.0};
    alphabeta<real_t> ab = dq.to_alphabeta(angle_wrapped(0.0));
    expect_near(ab[0], 1.0);
    expect_near(ab[1], 0.0);

    ab = dq.to_alphabeta(angle_wrapped(pi / 2.0f));
    expect_near(ab[0], 0.0);
    expect_near(ab[1], 1.0);

    ab = dq.to_alphabeta(angle_wrapped(pi));
    expect_near(ab[0], -1.0);
    expect_near(ab[1], 0.0);

    ab = dq.to_alphabeta(angle_wrapped(3 * pi / 2));
    expect_near(ab[0], 0.0);
    expect_near(ab[1], -1.0);
}

TEST(ReferenceFrameTest, FullTransformation)
{
    abc<real_t> abc{1.0, -0.5, -0.5};
    alphabeta<real_t> ab = abc.to_alphabeta();
    dq<real_t> dq = ab.to_dq(angle_wrapped(0.0));
    alphabeta<real_t> back_ab = dq.to_alphabeta(angle_wrapped(0.0));
    ::abc<real_t> back_abc = back_ab.to_abc();
    ::dq<real_t> back_dq = back_abc.to_dq(angle_wrapped(0.0));

    expect_near(back_abc[0], abc[0]);
    expect_near(back_abc[1], abc[1]);
    expect_near(back_abc[2], abc[2]);

    expect_near(back_ab[0], ab[0]);
    expect_near(back_ab[1], ab[1]);

    expect_near(back_dq[0], dq[0]);
    expect_near(back_dq[1], dq[1]);
}

TEST(ReferenceFrameTest, ABCToAlphaBeta)
{
    // Original balanced example
    abc<real_t> abc_coords{1.0, -0.5, -0.5};
    alphabeta<real_t> ab_coords = abc_coords.to_alphabeta();
    expect_near(ab_coords[0], 1.0);
    expect_near(ab_coords[1], 0.0);

    // Test 45°: use make_abc to generate balanced abc with magnitude 1 and phase 45°
    {
        abc<real_t> abc45 = make_abc<real_t>(1.0, angle_wrapped(45.0 * pi / 180.0));
        alphabeta<real_t> ab45 = abc45.to_alphabeta();
        EXPECT_NEAR(ab45[0], std::sqrt(2.0) / 2.0, 1e-6);
        EXPECT_NEAR(ab45[1], std::sqrt(2.0) / 2.0, 1e-6);
        EXPECT_NEAR(ab45.magnitude(), 1.0, 1e-6);
    }

    // Test 90°: balanced abc corresponding to 90° (expected: alpha=0, beta=1)
    {
        abc<real_t> abc90 = make_abc<real_t>(1.0, angle_wrapped(90.0 * pi / 180.0));
        alphabeta<real_t> ab90 = abc90.to_alphabeta();
        EXPECT_NEAR(ab90[0], 0.0, 1e-6);
        EXPECT_NEAR(ab90[1], 1.0, 1e-6);
        EXPECT_NEAR(ab90.magnitude(), 1.0, 1e-6);
    }

    // Test 135°: balanced abc corresponding to 135° (expected: alpha ≈ -0.707107, beta ≈ 0.707107)
    {
        abc<real_t> abc135 = make_abc<real_t>(1.0, angle_wrapped(135.0 * pi / 180.0));
        alphabeta<real_t> ab135 = abc135.to_alphabeta();
        EXPECT_NEAR(ab135[0], -std::sqrt(2.0) / 2.0, 1e-6);
        EXPECT_NEAR(ab135[1], std::sqrt(2.0) / 2.0, 1e-6);
        EXPECT_NEAR(ab135.magnitude(), 1.0, 1e-6);
    }

    // Test 235°: generate balanced abc for 235° and verify both magnitude and phase.
    {
        abc<real_t> abc235 = make_abc<real_t>(1.0, angle_wrapped(235.0 * pi / 180.0));
        alphabeta<real_t> ab235 = abc235.to_alphabeta();
        EXPECT_NEAR(ab235.magnitude(), 1.0, 1e-6);
        // Convert phase (in radians) to degrees and adjust it into [0,360)
        real_t phase_deg = ab235.phase().get_radians() * 180.0 / pi;
        if (phase_deg < 0) phase_deg += 360.0;
        EXPECT_NEAR(phase_deg, 235.0, 1e-6);
    }
}

TEST(ReferenceFrameTest, AlphaBetaToABC)
{
    // Basic example: convert {1.0, 0.0} in αβ back to abc.
    {
        alphabeta<real_t> ab{1.0, 0.0};
        abc<real_t> abc_res = ab.to_abc();
        EXPECT_NEAR(abc_res[0], 1.0, 1e-6);
        EXPECT_NEAR(abc_res[1], -0.5, 1e-6);
        EXPECT_NEAR(abc_res[2], -0.5, 1e-6);
    }

    // Test 45°: Convert αβ (obtained from make_alphabeta) back to abc.
    {
        alphabeta<real_t> ab45 = make_alphabeta<real_t>(1.0, angle_wrapped(45.0 * pi / 180.0));
        abc<real_t> abc_from_ab = ab45.to_abc();
        // Expected abc obtained by make_abc should match the conversion.
        abc<real_t> expected_abc = make_abc<real_t>(1.0, angle_wrapped(45.0 * pi / 180.0));
        auto expected_arr = expected_abc.array();
        auto result_arr = abc_from_ab.array();
        for (size_t i = 0; i < 3; i++) {
            EXPECT_NEAR(result_arr[i], expected_arr[i], 1e-6);
        }
    }

    // Test 135°: Convert αβ from 135° back to abc.
    {
        alphabeta<real_t> ab135 = make_alphabeta<real_t>(1.0, angle_wrapped(135.0 * pi / 180.0));
        abc<real_t> abc_from_ab = ab135.to_abc();
        abc<real_t> expected_abc = make_abc<real_t>(1.0, angle_wrapped(135.0 * pi / 180.0));
        auto expected_arr = expected_abc.array();
        auto result_arr = abc_from_ab.array();
        for (size_t i = 0; i < 3; i++) {
            EXPECT_NEAR(result_arr[i], expected_arr[i], 1e-6);
        }
    }

    // Test 235°: Convert for 235°.
    {
        alphabeta<real_t> ab235 = make_alphabeta<real_t>(1.0, angle_wrapped(235.0 * pi / 180.0));
        abc<real_t> abc_from_ab = ab235.to_abc();
        abc<real_t> expected_abc = make_abc<real_t>(1.0, angle_wrapped(235.0 * pi / 180.0));
        auto expected_arr = expected_abc.array();
        auto result_arr = abc_from_ab.array();
        for (size_t i = 0; i < 3; i++) {
            EXPECT_NEAR(result_arr[i], expected_arr[i], 1e-6);
        }
    }
}

TEST(ReferenceFrameTest, IteratorModification)
{
    // Test abc modification through iterator
    abc<real_t> abc_coords{1.0, 2.0, 3.0};
    for (auto& value : abc_coords) {
        value *= 2.0;
    }
    EXPECT_NEAR(abc_coords[0], 2.0, k_epsilon);
    EXPECT_NEAR(abc_coords[1], 4.0, k_epsilon);
    EXPECT_NEAR(abc_coords[2], 6.0, k_epsilon);

    // Test alphabeta modification through iterator
    alphabeta<real_t> ab_coords{1.0, 2.0};
    for (auto& value : ab_coords) {
        value *= 2.0;
    }
    EXPECT_NEAR(ab_coords[0], 2.0, k_epsilon);
    EXPECT_NEAR(ab_coords[1], 4.0, k_epsilon);

    // Test dq modification through iterator
    dq<real_t> dq_coords{1.0, 2.0};
    for (auto& value : dq_coords) {
        value *= 2.0;
    }
    EXPECT_NEAR(dq_coords[0], 2.0, k_epsilon);
    EXPECT_NEAR(dq_coords[1], 4.0, k_epsilon);
}

// class ReferenceFrameSimulatorTest : public ::testing::Test {
// public:
//     static constexpr double sampling_time = 1e-4;  // 10 kHz sampling rate
//     static constexpr real_t signal_freq = 50.0;    // 50 Hz fundamental

//     void SetUp() override
//     {
//         // simulator = mojito::make_gtest_simulator();

//         // Configure signal generator
//         signal_gen->set_signal_frequency(signal_freq);
//         signal_gen->set_fundamental_positive_sequence_signal_amplitude(1.0);

//         // Register simulation updates
//         simulator.register_lambda_runner(sampling_time, [&]() {
//             // Update signal generator
//             signal_gen->update();

//             // Get ABC values
//             abc_values = signal_gen->get_fundamental_signal_abc();

//             // Transform to alpha-beta
//             alphabeta_values = abc_values.to_alphabeta();

//             // Calculate theta for dq transformation
//             theta = alphabeta_values.phase();

//             // Transform to dq
//             dq_values = alphabeta_values.to_dq(theta);
//         });

//         // Register data sampling
//         register_samplers();
//     }

//     bool simulate_for(double time) { return simulator.simulate_for(time); }

// protected:
//     mojito::simulator simulator = mojito::make_gtest_simulator();

//     std::unique_ptr<three_phase_waveform_generator> signal_gen =
//         std::make_unique<three_phase_waveform_generator>(sampling_time);

//     abc<real_t> abc_values{};
//     alphabeta<real_t> alphabeta_values{};
//     dq<real_t> dq_values{};
//     angle_wrapped theta{0.0};

// private:
//     void register_samplers()
//     {
//         // Sample abc values
//         simulator.register_sampler_entry(mojito::data_sampler_entry("abc", abc_values));
//         // Sample alpha-beta values
//         simulator.register_sampler_entry(mojito::data_sampler_entry("alphabeta", alphabeta_values));
//         // Sample dq values
//         simulator.register_sampler_entry(mojito::data_sampler_entry("dq", dq_values));
//         // Sample theta
//         simulator.register_sampler_entry(mojito::data_sampler_entry("theta", [this]() { return theta.get_pu(); }));
//     }
// };

// TEST_F(ReferenceFrameSimulatorTest, BalancedTransformationChain)
// {
//     // For balanced signals:
//     // - ABC should be 120° apart and equal magnitude
//     // - Alpha-beta should have constant magnitude
//     // - DQ should be constant (d ≈ 1, q ≈ 0)

//     simulator.register_checker(mojito::make_range_checker<real_t>(
//         "d_value", [this]() { return dq_values[0]; }, 0.95, 1.05,  // Expect d ≈ 1.0 ±5%
//         mojito::checker_activation_time_range{0.1, 0.2}            // Check after initial settling
//         ));

//     simulator.register_checker(mojito::make_range_checker<real_t>(
//         "q_value", [this]() { return dq_values[1]; }, -0.05, 0.05,  // Expect q ≈ 0.0 ±5%
//         mojito::checker_activation_time_range{0.1, 0.2}));

//     // Check alpha-beta magnitude is constant
//     simulator.register_checker(mojito::make_range_checker<real_t>(
//         "alphabeta_magnitude", [this]() { return alphabeta_values.magnitude(); }, 0.95,
//         1.05,  // Expect magnitude ≈ 1.0 ±5%
//         mojito::checker_activation_time_range{0.1, 0.2}));

//     EXPECT_TRUE(simulate_for(0.2));  // Run for 10 cycles at 50 Hz
// }

// TEST_F(ReferenceFrameSimulatorTest, FrequencyStepResponse)
// {
//     // Test transformation chain during frequency step
//     signal_gen->set_frequency_step(25.0, 0.1);  // Step to 75 Hz at t=0.1s

//     // Check that DQ values remain constant even after frequency step
//     simulator.register_checker(mojito::make_range_checker<real_t>(
//         "d_value_after_step", [this]() { return dq_values[0]; }, 0.95, 1.05,
//         mojito::checker_activation_time_range{0.15, 0.2}  // Check after step
//         ));

//     simulator.register_checker(mojito::make_range_checker<real_t>(
//         "q_value_after_step", [this]() { return dq_values[1]; }, -0.05, 0.05,
//         mojito::checker_activation_time_range{0.15, 0.2}));

//     EXPECT_TRUE(simulate_for(0.2));  // Run for 10 cycles at 50 Hz
// }

// TEST_F(ReferenceFrameSimulatorTest, UnbalancedTransformationChain)
// {
//     // Test with unbalanced ABC (positive + negative sequence)
//     signal_gen->set_fundamental_positive_sequence_signal_amplitude(0.9);  // 90% positive sequence
//     signal_gen->set_fundamental_negative_sequence_signal_amplitude(0.1);  // 10% negative sequence

//     // DQ should now have 100Hz oscillation due to negative sequence
//     simulator.register_checker(mojito::make_range_checker<real_t>(
//         "d_oscillation", [this]() { return dq_values[0]; }, 0.79, 1.01,  // Expect d to oscillate between these
//         values mojito::checker_activation_time_range{0.1, 0.2}));

//     simulator.register_checker(mojito::make_range_checker<real_t>(
//         "q_oscillation", [this]() { return dq_values[1]; }, -0.1, 0.1,  // Expect q to oscillate between these values
//         mojito::checker_activation_time_range{0.1, 0.2}));

//     EXPECT_TRUE(simulate_for(0.2));  // Run for 10 cycles at 50 Hz
// }

}  // namespace