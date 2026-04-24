#include <gtest/gtest.h>

#include "mojito/angle_wrapped.hpp"
#include "mojito/constants.hpp"

namespace {

using namespace mojito;

static constexpr real_t k_epsilon = real_t{1e-6};

TEST(AngleWrappedTest, DefaultConstructor)
{
    angle_wrapped a;
    EXPECT_NEAR(a.get_radians().value(), real_t{0.0}, k_epsilon);
    EXPECT_NEAR(a.get_degrees(), real_t{0.0}, k_epsilon);
}

TEST(AngleWrappedTest, Constructor)
{
    angle_wrapped a(angle_t{1.0});
    EXPECT_NEAR(a.get_radians().value(), real_t{1.0}, k_epsilon);
    EXPECT_NEAR(a.get_degrees(), real_t{180.0} / pi, k_epsilon);
}

TEST(AngleWrappedTest, SetValueWrapNegative)
{
    angle_wrapped a;
    a.set_value(angle_t{-1.0});
    EXPECT_NEAR(a.get_radians().value(), real_t{-1.0} + two_pi, k_epsilon);
}

TEST(AngleWrappedTest, SetValueWrapPositive)
{
    angle_wrapped a;
    a.set_value(angle_t{two_pi + real_t{0.5}});
    EXPECT_NEAR(a.get_radians().value(), real_t{0.5}, k_epsilon);
}

TEST(AngleWrappedTest, SetValueExactlyTwoPi)
{
    angle_wrapped a;
    a.set_value(angle_t{two_pi});
    EXPECT_NEAR(a.get_radians().value(), real_t{0.0}, k_epsilon);

    a.set_value(angle_t{two_pi - real_t{1e-5}});
    EXPECT_NEAR(a.get_radians().value(), two_pi - real_t{1e-5}, k_epsilon);
}

TEST(AngleWrappedTest, SetValueExactlyZero)
{
    angle_wrapped a;
    a.set_value(angle_t{0.0});
    EXPECT_NEAR(a.get_radians().value(), real_t{0.0}, k_epsilon);
}

TEST(AngleWrappedTest, Addition)
{
    angle_wrapped a(angle_t{1.0});
    angle_wrapped b(angle_t{2.0});
    auto c = a + b;
    EXPECT_NEAR(c.get_radians().value(), real_t{3.0}, k_epsilon);
}

TEST(AngleWrappedTest, NegationOperator)
{
    angle_wrapped a;
    a.set_value(angle_t{1.0});
    angle_wrapped a_negated = -a;
    EXPECT_NEAR(a.get_radians().value(), real_t{1.0}, k_epsilon);
    EXPECT_NEAR(a_negated.get_radians().value(), two_pi - real_t{1.0}, k_epsilon);
}

TEST(AngleWrappedTest, AdditionWithWrapping)
{
    angle_wrapped a(angle_t{5.0});
    angle_wrapped b(angle_t{2.0});
    auto c = a + b;
    EXPECT_NEAR(c.get_radians().value(), real_t{7.0} - two_pi, k_epsilon);
}

TEST(AngleWrappedTest, SubtractionWithWrapping)
{
    angle_wrapped a(angle_t{1.0});
    angle_wrapped b(angle_t{2.0});
    auto c = a - b;
    EXPECT_NEAR(c.get_radians().value(), real_t{-1.0} + two_pi, k_epsilon);
}

TEST(AngleWrappedTest, AdditionAssignment)
{
    angle_wrapped a(angle_t{1.0});
    angle_wrapped b(angle_t{2.0});
    a += b;
    EXPECT_NEAR(a.get_radians().value(), real_t{3.0}, k_epsilon);
}

TEST(AngleWrappedTest, SubtractionAssignment)
{
    angle_wrapped a(angle_t{4.0});
    angle_wrapped b(angle_t{1.0});
    a -= b;
    EXPECT_NEAR(a.get_radians().value(), real_t{3.0}, k_epsilon);
}

TEST(AngleWrappedTest, ScalarMultiplication)
{
    angle_wrapped a(angle_t{1.0});
    auto b = real_t{2.0} * a;
    EXPECT_NEAR(b.get_radians().value(), real_t{2.0}, k_epsilon);
}

TEST(AngleWrappedTest, Equality)
{
    angle_wrapped a(angle_t{1.0});
    angle_wrapped b(angle_t{1.0});
    EXPECT_EQ(a, b);
}

TEST(AngleWrappedTest, EqualityWithWrapping)
{
    angle_wrapped a(angle_t{0.0});
    angle_wrapped b(angle_t{two_pi});
    EXPECT_EQ(a, b);
}

TEST(AngleWrappedTest, LessThanBasic)
{
    angle_wrapped a(angle_t{1.0});
    angle_wrapped b(angle_t{2.0});
    EXPECT_TRUE(a < b);
    EXPECT_FALSE(b < a);
}

TEST(AngleWrappedTest, LessThanWithWrapping)
{
    angle_wrapped a(angle_t{-0.1});
    angle_wrapped b(angle_t{0.1});
    EXPECT_TRUE(b < a);
}

TEST(AngleWrappedTest, ComparisonTransitivity)
{
    angle_wrapped a(angle_t{1.0});
    angle_wrapped b(angle_t{2.0});
    angle_wrapped c(angle_t{3.0});
    EXPECT_TRUE(a < b);
    EXPECT_TRUE(b < c);
    EXPECT_TRUE(a < c);
}

}  // namespace
