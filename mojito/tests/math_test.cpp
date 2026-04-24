#include <gtest/gtest.h>
#include "mojito/mojito.hpp"

namespace {

using namespace mojito;

static constexpr real_t k_epsilon = real_t{1e-6};

TEST(MathTest, CrossProduct)
{
    alphabeta<real_t> a{real_t{1.0}, real_t{0.0}};
    alphabeta<real_t> b{real_t{0.0}, real_t{1.0}};
    EXPECT_NEAR(cross_product(a, b), real_t{1.0}, k_epsilon);
    EXPECT_NEAR(cross_product(b, a), real_t{-1.0}, k_epsilon);

    dq<real_t> d{real_t{1.0}, real_t{0.0}};
    dq<real_t> q{real_t{0.0}, real_t{1.0}};
    EXPECT_NEAR(cross_product(d, q), real_t{1.0}, k_epsilon);
}

TEST(MathTest, Sum)
{
    abc<real_t> val{real_t{1.0}, real_t{2.0}, real_t{3.0}};
    EXPECT_NEAR(sum(val), real_t{6.0}, k_epsilon);
}

TEST(MathTest, Max)
{
    abc<real_t> val_abc{real_t{1.0}, real_t{3.0}, real_t{2.0}};
    EXPECT_NEAR(max(val_abc), real_t{3.0}, k_epsilon);

    alphabeta<real_t> val_ab{real_t{1.0}, real_t{4.0}};
    EXPECT_NEAR(max(val_ab), real_t{4.0}, k_epsilon);

    dq<real_t> val_dq{real_t{5.0}, real_t{2.0}};
    EXPECT_NEAR(max(val_dq), real_t{5.0}, k_epsilon);
}

TEST(MathTest, Min)
{
    abc<real_t> val_abc{real_t{1.0}, real_t{3.0}, real_t{2.0}};
    EXPECT_NEAR(min(val_abc), real_t{1.0}, k_epsilon);

    alphabeta<real_t> val_ab{real_t{1.0}, real_t{4.0}};
    EXPECT_NEAR(min(val_ab), real_t{1.0}, k_epsilon);

    dq<real_t> val_dq{real_t{5.0}, real_t{2.0}};
    EXPECT_NEAR(min(val_dq), real_t{2.0}, k_epsilon);
}

}  // namespace
