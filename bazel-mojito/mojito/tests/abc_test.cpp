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

const std::array<real_t, 3> balanced_abc = {real_t{1.0}, real_t{-0.5}, real_t{-0.5}};

TEST(AbcTest, DefaultConstructor)
{
    static_assert(std::is_trivially_copyable_v<abc<real_t>>);
    static_assert(std::is_trivially_copyable_v<alphabeta<real_t>>);
    static_assert(std::is_trivially_copyable_v<dq<real_t>>);

    abc<real_t> coords;
    expect_array_near_abc(coords, {real_t{0.0}, real_t{0.0}, real_t{0.0}});
}

TEST(AbcTest, ArrayConstructor)
{
    abc<real_t> coords(balanced_abc);
    expect_array_near_abc(coords, balanced_abc);
}

TEST(AbcTest, ValueConstructor)
{
    abc<real_t> coords(real_t{1.0}, real_t{-0.5}, real_t{-0.5});
    expect_array_near_abc(coords, balanced_abc);
}

TEST(AbcTest, AdditionAndSubtraction)
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

TEST(AbcTest, UnaryNegation)
{
    abc<real_t> abc_coords{real_t{1.0}, real_t{-0.5}, real_t{-0.5}};
    abc<real_t> neg_abc = -abc_coords;
    EXPECT_NEAR(neg_abc[0], real_t{-1.0}, k_epsilon);
    EXPECT_NEAR(neg_abc[1], real_t{0.5}, k_epsilon);
    EXPECT_NEAR(neg_abc[2], real_t{0.5}, k_epsilon);
}

TEST(AbcTest, ScalarOperations)
{
    abc<real_t> a(real_t{1.0}, real_t{2.0}, real_t{3.0});
    abc<real_t> b = a * real_t{3.0};
    expect_array_near_abc(b, {real_t{3.0}, real_t{6.0}, real_t{9.0}});

    abc<real_t> c = a / real_t{2.0};
    expect_array_near_abc(c, {real_t{0.5}, real_t{1.0}, real_t{1.5}});
}

}  // namespace
