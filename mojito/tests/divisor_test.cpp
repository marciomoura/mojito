#include <gtest/gtest.h>

#include "mojito/divisor.hpp"
#include "mojito/units.hpp"

namespace {

using namespace mojito;

static constexpr real_t k_epsilon = real_t{1e-6};

TEST(DivisorTest, ConstructionAndAccessors)
{
    constexpr real_t val = real_t{4.0};
    divisor<real_t> d(val);

    EXPECT_NEAR(d.value(), val, k_epsilon);
    EXPECT_NEAR(d.reciprocal(), real_t{1.0} / val, k_epsilon);
}

TEST(DivisorTest, DivisionOperator)
{
    constexpr real_t numerator = real_t{20.0};
    constexpr real_t denominator = real_t{5.0};
    divisor<real_t> d(denominator);

    real_t result = numerator / d;
    EXPECT_NEAR(result, real_t{4.0}, k_epsilon);
}

TEST(DivisorTest, UnitsConstructionAndAccessors)
{
    voltage_t v(real_t{10.0});
    divisor<voltage_t> d(v);
    EXPECT_NEAR(d.value().value(), v.value(), k_epsilon);

    auto reciprocal = d.reciprocal();
    EXPECT_NEAR(reciprocal.value(), real_t{0.1}, k_epsilon);
}

TEST(DivisorTest, DivisionWithUnits)
{
    power_t power(real_t{1000.0});
    voltage_t voltage(real_t{100.0});

    current_t expected_current = power / voltage;
    EXPECT_NEAR(expected_current.value(), real_t{10.0}, k_epsilon);

    divisor<voltage_t> voltage_divisor(voltage);
    auto result = power / voltage_divisor;

    EXPECT_NEAR(result.value(), real_t{10.0}, k_epsilon);
    static_assert(std::is_same_v<decltype(result), current_t>);
}

TEST(DivisorTest, OptimizedPerUnitConversion)
{
    voltage_t si_voltage(real_t{150.0});
    voltage_t base_voltage(real_t{200.0});
    divisor<voltage_t> base_voltage_divisor(base_voltage);

    auto pu_ratio = si_voltage / base_voltage_divisor;
    EXPECT_NEAR(pu_ratio, real_t{0.75}, k_epsilon);

    voltage_t converted_si_voltage(pu_ratio * base_voltage.value());
    EXPECT_NEAR(converted_si_voltage.value(), si_voltage.value(), k_epsilon);
}

TEST(DivisorTest, ToPuToSiFunctionIntegration)
{
    voltage_t base_voltage(real_t{1000.0});
    voltage_t test_voltage(real_t{500.0});
    divisor<voltage_t> voltage_divisor(base_voltage);

    auto pu_voltage = to_pu(test_voltage, voltage_divisor);
    EXPECT_NEAR(pu_voltage.value(), real_t{0.5}, k_epsilon);

    auto reconstructed_voltage = to_si(pu_voltage, voltage_divisor);
    EXPECT_NEAR(reconstructed_voltage.value(), test_voltage.value(), k_epsilon);
}

}  // namespace
