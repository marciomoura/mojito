#include <gtest/gtest.h>

#include "mojito/angle_wrapped.hpp"

namespace {

using namespace mojito;

TEST(AngleZeroToTwoPiTest, DefaultConstructor)
{
    angle_wrapped a;
    EXPECT_NEAR(a.get_radians(), 0.0, 1e-5);
    EXPECT_NEAR(a.get_degrees(), 0.0, 1e-5);
}

TEST(AngleZeroToTwoPiTest, Constructor)
{
    angle_wrapped a(angle_t{1.0});
    EXPECT_NEAR(a.get_radians(), 1.0, 1e-5);
    EXPECT_NEAR(a.get_degrees(), 180.0 / pi, 1e-5);
}

TEST(AngleZeroToTwoPiTest, SetValueWrapNegative)
{
    angle_wrapped a;
    a.set_value(angle_t{-1.0});
    EXPECT_NEAR(a.get_radians(), -1.0 + 2 * pi, 1e-5);
}

TEST(AngleZeroToTwoPiTest, SetValueWrapPositive)
{
    angle_wrapped a;
    a.set_value(angle_t{2 * pi + 0.5});
    EXPECT_NEAR(a.get_radians(), 0.5, 1e-5);
}

TEST(AngleZeroToTwoPiTest, SetValueExactlyTwoPi)
{
    angle_wrapped a;
    a.set_value(angle_t{2 * pi});

    // Setting exactly 2π should wrap to 0
    EXPECT_NEAR(a.get_radians(), 0.0f, 1e-5);

    // Setting near 2π should not wrap
    a.set_value(angle_t{2 * pi - 1e-5});
    EXPECT_NEAR(a.get_radians(), 2 * pi - 1e-5, 1e-5);
}

TEST(AngleZeroToTwoPiTest, SetValueExactlyZero)
{
    angle_wrapped a;
    a.set_value(angle_t{0.0});
    EXPECT_NEAR(a.get_radians(), 0.0, 1e-5);
}

TEST(AngleZeroToTwoPiTest, Addition)
{
    angle_wrapped a(angle_t{1.0});
    angle_wrapped b(angle_t{2.0});
    auto c = a + b;
    EXPECT_NEAR(c.get_radians(), 3.0, 1e-5);
}

TEST(AngleZeroToTwoPiTest, NegationOperator)
{
    angle_wrapped a;
    a.set_value(angle_t{1.0});
    angle_wrapped a_negated = -a;
    EXPECT_NEAR(a.get_radians(), 1.0, 1e-5);
    EXPECT_NEAR(a_negated.get_radians(), 2.0 * pi - 1.0, 1e-5);
}

TEST(AngleZeroToTwoPiTest, AdditionWithWrapping)
{
    angle_wrapped a(angle_t{5.0});
    angle_wrapped b(angle_t{2.0});
    auto c = a + b;
    EXPECT_NEAR(c.get_radians(), 7.0 - 2 * pi, 1e-5);
}

TEST(AngleZeroToTwoPiTest, SubtractionWithWrapping)
{
    angle_wrapped a(angle_t{1.0});
    angle_wrapped b(angle_t{2.0});
    auto c = a - b;  // Should wrap from -1.0 to -1.0 + 2pi
    EXPECT_NEAR(c.get_radians(), -1.0 + 2 * pi, 1e-5);
}

TEST(AngleZeroToTwoPiTest, AdditionAssignment)
{
    angle_wrapped a(angle_t{1.0});
    angle_wrapped b(angle_t{2.0});
    a += b;
    EXPECT_NEAR(a.get_radians(), 3.0, 1e-5);
}

TEST(AngleZeroToTwoPiTest, SubtractionAssignment)
{
    angle_wrapped a(angle_t{4.0});
    angle_wrapped b(angle_t{1.0});
    a -= b;
    EXPECT_NEAR(a.get_radians(), 3.0, 1e-5);
}

TEST(AngleZeroToTwoPiTest, ScalarMultiplication)
{
    angle_wrapped a(angle_t{1.0});
    auto b = real_t{2.0} * a;
    EXPECT_NEAR(b.get_radians(), 2.0, 1e-5);
}

TEST(AngleZeroToTwoPiTest, Equality)
{
    angle_wrapped a(angle_t{1.0});
    angle_wrapped b(angle_t{1.0});
    EXPECT_EQ(a, b);
}

TEST(AngleZeroToTwoPiTest, EqualityWithWrapping)
{
    angle_wrapped a(angle_t{0.0});
    angle_wrapped b(angle_t{2 * pi});
    EXPECT_EQ(a, b);
}

TEST(AngleZeroToTwoPiTest, Inequality)
{
    angle_wrapped a(angle_t{1.0});
    angle_wrapped b(angle_t{2.0});
    EXPECT_NE(a, b);
}

// Comparison operator tests for zero_to_two_pi

TEST(AngleZeroToTwoPiTest, LessThanBasic)
{
    angle_wrapped a(angle_t{1.0});
    angle_wrapped b(angle_t{2.0});
    EXPECT_TRUE(a < b);
    EXPECT_FALSE(b < a);
}

TEST(AngleZeroToTwoPiTest, LessThanEqual)
{
    angle_wrapped a(angle_t{1.0});
    angle_wrapped b(angle_t{1.0});
    EXPECT_FALSE(a < b);
    EXPECT_FALSE(b < a);
}

TEST(AngleZeroToTwoPiTest, LessThanWithWrapping)
{
    angle_wrapped a(angle_t{-0.1});  // Wraps to 2π - 0.1
    angle_wrapped b(angle_t{0.1});   // Near zero
    // After wrapping both to [0, 2π]: a ≈ 2π-0.1, b ≈ 0.1
    EXPECT_TRUE(b < a);
}

TEST(AngleZeroToTwoPiTest, LessThanOrEqualBasic)
{
    angle_wrapped a(angle_t{1.0});
    angle_wrapped b(angle_t{2.0});
    EXPECT_TRUE(a <= b);
    EXPECT_FALSE(b <= a);
}

TEST(AngleZeroToTwoPiTest, LessThanOrEqualEqual)
{
    angle_wrapped a(angle_t{1.0});
    angle_wrapped b(angle_t{1.0});
    EXPECT_TRUE(a <= b);
    EXPECT_TRUE(b <= a);
}

TEST(AngleZeroToTwoPiTest, LessThanOrEqualWithWrapping)
{
    angle_wrapped a(angle_t{0.0});     // At zero boundary
    angle_wrapped b(angle_t{2 * pi});  // Should wrap to 0.0 (equivalent)
    EXPECT_TRUE(a <= b);
    EXPECT_TRUE(b <= a);
}

TEST(AngleZeroToTwoPiTest, GreaterThanBasic)
{
    angle_wrapped a(angle_t{2.0});
    angle_wrapped b(angle_t{1.0});
    EXPECT_TRUE(a > b);
    EXPECT_FALSE(b > a);
}

TEST(AngleZeroToTwoPiTest, GreaterThanEqual)
{
    angle_wrapped a(angle_t{1.0});
    angle_wrapped b(angle_t{1.0});
    EXPECT_FALSE(a > b);
    EXPECT_FALSE(b > a);
}

TEST(AngleZeroToTwoPiTest, GreaterThanWithWrapping)
{
    angle_wrapped a(angle_t{0.1});   // Near zero
    angle_wrapped b(angle_t{-0.1});  // Wraps to 2π - 0.1
    // After wrapping both to [0, 2π]: a ≈ 0.1, b ≈ 2π-0.1
    EXPECT_TRUE(b > a);
}

TEST(AngleZeroToTwoPiTest, GreaterThanOrEqualBasic)
{
    angle_wrapped a(angle_t{2.0});
    angle_wrapped b(angle_t{1.0});
    EXPECT_TRUE(a >= b);
    EXPECT_FALSE(b >= a);
}

TEST(AngleZeroToTwoPiTest, GreaterThanOrEqualEqual)
{
    angle_wrapped a(angle_t{1.0});
    angle_wrapped b(angle_t{1.0});
    EXPECT_TRUE(a >= b);
    EXPECT_TRUE(b >= a);
}

TEST(AngleZeroToTwoPiTest, GreaterThanOrEqualWithWrapping)
{
    angle_wrapped a(angle_t{0.0});     // At zero boundary
    angle_wrapped b(angle_t{2 * pi});  // Should wrap to 0.0 (equivalent)
    EXPECT_TRUE(a >= b);
    EXPECT_TRUE(b >= a);
}

TEST(AngleZeroToTwoPiTest, ComparisonTransitivity)
{
    angle_wrapped a(angle_t{1.0});
    angle_wrapped b(angle_t{2.0});
    angle_wrapped c(angle_t{3.0});

    EXPECT_TRUE(a < b);
    EXPECT_TRUE(b < c);
    EXPECT_TRUE(a < c);  // Transitivity
}

TEST(AngleZeroToTwoPiTest, ComparisonBoundaryValues)
{
    angle_wrapped a(angle_t{0.0});           // At lower boundary
    angle_wrapped b(angle_t{pi});            // At middle
    angle_wrapped c(angle_t{2 * pi - 0.1});  // Near upper boundary

    EXPECT_TRUE(a < b);
    EXPECT_TRUE(b < c);
    EXPECT_TRUE(a < c);
}

}  // namespace