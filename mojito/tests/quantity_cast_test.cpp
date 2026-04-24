#include <gtest/gtest.h>
#include "mojito/mojito.hpp"

namespace {

using namespace mojito;

static constexpr real_t k_epsilon = real_t{1e-6};

TEST(QuantityCastTest, AbcCast)
{
    abc<real_t> scalar_v{real_t{1.0}, real_t{-0.5}, real_t{-0.5}};
    auto quantity_v = to_dimension_abc<voltage_t>(scalar_v);
    EXPECT_NEAR(quantity_v.a().value(), real_t{1.0}, k_epsilon);

    auto back_scalar = to_dimensionless_abc(quantity_v);
    EXPECT_NEAR(back_scalar.a(), real_t{1.0}, k_epsilon);
}

TEST(QuantityCastTest, AlphabetaCast)
{
    alphabeta<real_t> scalar_i{real_t{1.0}, real_t{0.0}};
    auto quantity_i = to_dimension_alphabeta<current_t>(scalar_i);
    EXPECT_NEAR(quantity_i.alpha().value(), real_t{1.0}, k_epsilon);

    auto back_scalar = to_dimensionless_alphabeta(quantity_i);
    EXPECT_NEAR(back_scalar.alpha(), real_t{1.0}, k_epsilon);
}

TEST(QuantityCastTest, DqCast)
{
    dq<real_t> scalar_t{real_t{0.8}, real_t{0.6}};
    auto quantity_t = to_dimension_dq<torque_t>(scalar_t);
    EXPECT_NEAR(quantity_t.d().value(), real_t{0.8}, k_epsilon);

    auto back_scalar = to_dimensionless_dq(quantity_t);
    EXPECT_NEAR(back_scalar.d(), real_t{0.8}, k_epsilon);
}

}  // namespace
