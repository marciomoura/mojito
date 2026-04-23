#include <gtest/gtest.h>

#include "mojito/divisor.hpp"
#include "mojito/units.hpp"

using namespace mojito;

// Test fixture for divisor with scalar types
class PrecomputedDivisorScalarTest : public ::testing::Test {};

TEST_F(PrecomputedDivisorScalarTest, ConstructionAndAccessors)
{
    constexpr double val = 4.0;
    divisor<double> d(val);

    EXPECT_DOUBLE_EQ(d.value(), val);
    EXPECT_DOUBLE_EQ(d.reciprocal(), 1.0 / val);
}

TEST_F(PrecomputedDivisorScalarTest, DivisionOperator)
{
    constexpr double numerator = 20.0;
    constexpr double denominator = 5.0;
    divisor<double> d(denominator);

    double result = numerator / d;
    EXPECT_DOUBLE_EQ(result, 4.0);
}

// Test fixture for divisor with dimensioned quantities
class PrecomputedDivisorUnitsTest : public ::testing::Test {};

TEST_F(PrecomputedDivisorUnitsTest, ConstructionAndAccessors)
{
    voltage_t v(10.0);

    // First, test that our scalar/quantity division works
    auto reciprocal_manual = 1.0 / v;

    // Check what type this gives us
    using manual_reciprocal_type = decltype(reciprocal_manual);
    static_assert(!std::is_same_v<manual_reciprocal_type, real_t>, "Manual reciprocal should not be scalar");

    // Now test the precomputed divisor
    divisor<voltage_t> d(v);
    EXPECT_EQ(d.value(), v);

    auto reciprocal = d.reciprocal();
    EXPECT_FLOAT_EQ(reciprocal.value(), 0.1);
}

TEST_F(PrecomputedDivisorUnitsTest, DivisionWithUnits)
{
    power_t power(1000.0);     // W
    voltage_t voltage(100.0);  // V

    // I = P / V (direct calculation for verification)
    current_t expected_current = power / voltage;
    EXPECT_DOUBLE_EQ(expected_current.value(), 10.0);

    // Now test with precomputed divisor
    divisor<voltage_t> voltage_divisor(voltage);
    auto result = power / voltage_divisor;

    // The result from power/voltage_divisor should be current_t
    // result = power * reciprocal(voltage) = power * (1/voltage) = current
    EXPECT_DOUBLE_EQ(result.value(), 10.0);

    // Check the result type by trying to assign to current_t
    // If this fails, it means the dimension calculation is wrong
    static_assert(std::is_same_v<decltype(result), current_t>, "Result should be current_t");
}
TEST_F(PrecomputedDivisorUnitsTest, OptimizedPerUnitConversion)
{
    // Define SI values and a base quantity for conversion
    voltage_t si_voltage(150.0);
    voltage_t base_voltage(200.0);

    // Create a precomputed divisor for the base quantity.
    // This would be done once during initialization in a real application.
    divisor<voltage_t> base_voltage_divisor(base_voltage);

    // Perform the optimized to_pu conversion using the precomputed divisor.
    // The division operator returns a scalar ratio (SI/SI = dimensionless)
    auto pu_ratio = si_voltage / base_voltage_divisor;

    EXPECT_DOUBLE_EQ(pu_ratio, 0.75);

    // Verify the reverse operation (to_si).
    // To get back voltage, we need to multiply the ratio by the base voltage
    voltage_t converted_si_voltage(pu_ratio * base_voltage.value());
    EXPECT_DOUBLE_EQ(converted_si_voltage.value(), si_voltage.value());
}

TEST_F(PrecomputedDivisorUnitsTest, OptimizedPerUnitTorqueConversion)
{
    // Define a more complex conversion: torque
    torque_t si_torque(75.0);
    power_t base_power(3000.0);
    frequency_t base_frequency(50.0);

    // Base torque is derived from base power and base frequency
    // T_base = P_base / (2 * pi * f_base)
    constexpr real_t pi = 3.14159265358979323846;
    torque_t base_torque = base_power / (2.0 * pi * base_frequency);

    // Pre-calculate the divisor for the base torque
    divisor<torque_t> base_torque_divisor(base_torque);

    // Perform the optimized to_pu conversion using division
    auto pu_torque_ratio = si_torque / base_torque_divisor;

    // Expected value: 75.0 / (3000.0 / (2 * pi * 50.0)) = 7.8539...
    EXPECT_NEAR(pu_torque_ratio, 7.8539, 1e-4);
}

TEST_F(PrecomputedDivisorUnitsTest, ToPuToSiFunctionIntegration)
{
    // Test voltage conversion using to_pu/to_si functions
    voltage_t base_voltage(1000.0);
    voltage_t test_voltage(500.0);

    divisor<voltage_t> voltage_divisor(base_voltage);

    // Convert to per-unit using to_pu function
    auto pu_voltage = to_pu(test_voltage, voltage_divisor);
    EXPECT_DOUBLE_EQ(pu_voltage.value(), 0.5);

    // Convert back to SI using to_si function
    auto reconstructed_voltage = to_si(pu_voltage, voltage_divisor);
    EXPECT_DOUBLE_EQ(reconstructed_voltage.value(), test_voltage.value());

    // Test current conversion
    current_t base_current(100.0);
    current_t test_current(75.0);

    divisor<current_t> current_divisor(base_current);

    auto pu_current = to_pu(test_current, current_divisor);
    EXPECT_DOUBLE_EQ(pu_current.value(), 0.75);

    auto reconstructed_current = to_si(pu_current, current_divisor);
    EXPECT_DOUBLE_EQ(reconstructed_current.value(), test_current.value());
}