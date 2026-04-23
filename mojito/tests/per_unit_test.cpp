#include "mojito/per_unit.hpp"

#include <gtest/gtest.h>

#include "mojito/reference_frame.hpp"

namespace {

using namespace mojito;
using namespace mojito;

TEST(PerUnitTest, ConvertSiToPu)
{
    double value_si = 100.0;
    double value_base = 200.0;

    double pu_value = convert_si_to_pu(value_si, value_base);
    EXPECT_DOUBLE_EQ(pu_value, 0.5);
}

TEST(PerUnitTest, ConvertPuToSi)
{
    double pu_value = 0.5;
    double value_base = 200.0;

    double value_si = convert_pu_to_si(pu_value, value_base);
    EXPECT_DOUBLE_EQ(value_si, 100.0);
}

// For AlphaBeta
TEST(PerUnitTest, ConvertAlphaBetaSiToPu)
{
    alphabeta<double> value_si(1.0, -0.5);
    double value_base(2.0);

    alphabeta<double> pu_value = convert_si_to_pu(value_si, value_base);
    EXPECT_DOUBLE_EQ(pu_value.alpha(), 0.5);
    EXPECT_DOUBLE_EQ(pu_value.beta(), -0.25);
}

TEST(PerUnitTest, ConvertAlphaBetaPuToSi)
{
    alphabeta<double> pu_value(0.5, -0.25);
    double value_base(2.0);

    alphabeta<double> value_si = convert_pu_to_si(pu_value, value_base);
    EXPECT_DOUBLE_EQ(value_si.alpha(), 1.0);
    EXPECT_DOUBLE_EQ(value_si.beta(), -0.5);
}

// For ABC
TEST(PerUnitTest, ConvertAbcSiToPu)
{
    abc<double> value_si(1.0, -0.5, 0.5);
    double value_base(2.0);

    abc<double> pu_value = convert_si_to_pu(value_si, value_base);
    EXPECT_DOUBLE_EQ(pu_value.a(), 0.5);
    EXPECT_DOUBLE_EQ(pu_value.b(), -0.25);
    EXPECT_DOUBLE_EQ(pu_value.c(), 0.25);
}

TEST(PerUnitTest, ConvertAbcPuToSi)
{
    abc<double> pu_value(0.5, -0.25, 0.25);
    double value_base(2.0);

    abc<double> value_si = convert_pu_to_si(pu_value, value_base);
    EXPECT_DOUBLE_EQ(value_si.a(), 1.0);
    EXPECT_DOUBLE_EQ(value_si.b(), -0.5);
    EXPECT_DOUBLE_EQ(value_si.c(), 0.5);
}

// For DQ
TEST(PerUnitTest, ConvertDqSiToPu)
{
    dq<double> value_si(1.0, -0.5);
    double value_base(2.0);

    dq<double> pu_value = convert_si_to_pu(value_si, value_base);
    EXPECT_DOUBLE_EQ(pu_value.d(), 0.5);
    EXPECT_DOUBLE_EQ(pu_value.q(), -0.25);
}

TEST(PerUnitTest, ConvertDqPuToSi)
{
    dq<double> pu_value(0.5, -0.25);
    double value_base(2.0);

    dq<double> value_si = convert_pu_to_si(pu_value, value_base);
    EXPECT_DOUBLE_EQ(value_si.d(), 1.0);
    EXPECT_DOUBLE_EQ(value_si.q(), -0.5);
}

}  // namespace