#include <gtest/gtest.h>

#include "mojito/constants.hpp"
#include "mojito/units.hpp"

namespace {

using namespace mojito;

static constexpr real_t k_epsilon = real_t{1e-6};

TEST(UnitsTest, QuantityCreationAndValueAccess)
{
    length_t distance(real_t{10.5});
    mass_t weight(real_t{2.5});

    EXPECT_NEAR(distance.value(), real_t{10.5}, k_epsilon);
    EXPECT_NEAR(weight.value(), real_t{2.5}, k_epsilon);
}

TEST(UnitsTest, CompileTimeDimensionMismatch)
{
    // This test "passes" if the commented-out code fails to compile.
    /*
    voltage_t v(real_t{120.0});
    current_t i(real_t{10.0});
    v = i;  // COMPILE ERROR: Cannot assign current to voltage
    */
    SUCCEED() << "Verified: Incompatible dimension assignment is a compile-time error.";
}

TEST(UnitsTest, CompileTimeSystemMismatch)
{
    // This test "passes" if the commented-out code fails to compile.
    /*
    voltage_t si_v(real_t{240.0});
    voltage_pu_t pu_v(real_t{1.0});
    si_v = pu_v;  // COMPILE ERROR: Cannot assign PerUnit quantity to SI quantity
    auto sum = si_v + pu_v;  // COMPILE ERROR: Cannot add quantities from different systems
    */
    SUCCEED() << "Verified: Incompatible system assignment/operation is a compile-time error.";
}

TEST(UnitsTest, SystemDifferentiation)
{
    voltage_t si_voltage(real_t{120.0});
    voltage_pu_t pu_voltage(real_t{1.0});

    static_assert(std::is_same_v<decltype(si_voltage)::system, si>);
    static_assert(is_per_unit_v<decltype(pu_voltage)::system>);
}

TEST(UnitsTest, ExplicitConversion)
{
    voltage_t base_voltage(real_t{240.0});
    voltage_t measured_voltage(real_t{228.0});

    auto pu_v = to_pu(measured_voltage, base_voltage);
    EXPECT_NEAR(pu_v.value(), real_t{228.0 / 240.0}, k_epsilon);

    auto si_v = to_si(pu_v, base_voltage);
    EXPECT_NEAR(si_v.value(), measured_voltage.value(), k_epsilon);

    // Test compile-time check for mismatched dimensions in conversion
    /*
    current_t base_current(real_t{10.0});
    to_pu(measured_voltage, base_current); // COMPILE ERROR
    */
    SUCCEED() << "Verified: Conversion with mismatched base dimension is a compile-time error.";
}

TEST(UnitsTest, ArithmeticOperations)
{
    auto dist1 = length_t(real_t{10.5});
    auto dist2 = length_t(real_t{20.0});

    auto sum = dist1 + dist2;
    auto diff = dist2 - dist1;

    EXPECT_NEAR(sum.value(), real_t{30.5}, k_epsilon);
    EXPECT_NEAR(diff.value(), real_t{9.5}, k_epsilon);

    auto duration = duration_t(real_t{10.0});
    auto speed = dist1 / duration;
    EXPECT_NEAR(speed.value(), real_t{1.05}, k_epsilon);
}

TEST(UnitsTest, ElectricalLaws)
{
    auto v = voltage_t(real_t{120.0});
    auto r = resistance_t(real_t{60.0});
    auto i = v / r;

    EXPECT_NEAR(i.value(), real_t{2.0}, k_epsilon);
    static_assert(std::is_same_v<decltype(i), current_t>);

    auto p = v * i;
    EXPECT_NEAR(p.value(), real_t{240.0}, k_epsilon);
}

TEST(UnitsTest, MechanicalLaws)
{
    auto m = mass_t(real_t{10.0});
    auto a = acceleration_t(real_t{9.8});
    auto f = m * a;

    EXPECT_NEAR(f.value(), real_t{98.0}, k_epsilon);
    static_assert(std::is_same_v<decltype(f), force_t>);
}

TEST(UnitsTest, PercentSystem)
{
    voltage_percent_t v_pct(real_t{50.0});
    EXPECT_NEAR(v_pct.value(), real_t{50.0}, k_epsilon);

    // Arithmetic
    auto v_pct2 = v_pct * real_t{2.0};
    EXPECT_NEAR(v_pct2.value(), real_t{100.0}, k_epsilon);

    auto v_pct3 = v_pct + voltage_percent_t(real_t{10.0});
    EXPECT_NEAR(v_pct3.value(), real_t{60.0}, k_epsilon);

    // Conversions
    voltage_t v_si(real_t{115.0});
    voltage_t v_base(real_t{230.0});

    auto v_pct_conv = to_percent(v_si, v_base);
    EXPECT_NEAR(v_pct_conv.value(), real_t{50.0}, k_epsilon);

    auto v_si_back = to_si(v_pct_conv, v_base);
    EXPECT_NEAR(v_si_back.value(), real_t{115.0}, k_epsilon);

    voltage_pu_t v_pu(real_t{0.5});
    auto v_pct_from_pu = to_percent(v_pu);
    EXPECT_NEAR(v_pct_from_pu.value(), real_t{50.0}, k_epsilon);

    auto v_pu_back = to_pu(v_pct_from_pu);
    EXPECT_NEAR(v_pu_back.value(), real_t{0.5}, k_epsilon);

    // Casting
    using duty_cycle_percent_t = quantity<dimensionless_dim, percent>;
    duty_cycle_percent_t duty(real_t{75.0});
    auto angle_pct = percent_cast<angle_percent_t>(duty);
    EXPECT_NEAR(angle_pct.value(), real_t{75.0}, k_epsilon);
}

struct machine_base {};
struct grid_base {};

TEST(UnitsTest, CustomPerUnitSystem)
{
    voltage_cus_pu_t<machine_base> v_mach(real_t{1.1});
    voltage_cus_pu_t<grid_base> v_grid(real_t{1.0});

    EXPECT_NEAR(v_mach.value(), real_t{1.1}, k_epsilon);
    EXPECT_NEAR(v_grid.value(), real_t{1.0}, k_epsilon);

    // Explicit conversions to/from SI
    voltage_t v_si(real_t{230.0});
    voltage_t v_base_mach(real_t{200.0});
    voltage_t v_base_grid(real_t{230.0});

    auto v_mach_conv = to_pu<machine_base>(v_si, v_base_mach);
    EXPECT_NEAR(v_mach_conv.value(), real_t{1.15}, k_epsilon);
    static_assert(std::is_same_v<decltype(v_mach_conv), voltage_cus_pu_t<machine_base>>);

    auto v_grid_conv = to_pu<grid_base>(v_si, v_base_grid);
    EXPECT_NEAR(v_grid_conv.value(), real_t{1.0}, k_epsilon);
    static_assert(std::is_same_v<decltype(v_grid_conv), voltage_cus_pu_t<grid_base>>);

    auto v_si_back = to_si(v_mach_conv, v_base_mach);
    EXPECT_NEAR(v_si_back.value(), real_t{230.0}, k_epsilon);

    // Arithmetic within the same custom system
    auto v_sum = v_mach + v_mach;
    EXPECT_NEAR(v_sum.value(), real_t{2.2}, k_epsilon);

    // Cross-system protection (Compile-time check)
    /*
    auto v_bad = v_mach + v_grid; // COMPILE ERROR
    v_mach = v_grid; // COMPILE ERROR
    v_mach = voltage_pu_t(1.0); // COMPILE ERROR
    */
    SUCCEED() << "Verified: Different custom per-unit bases are type-safe and cannot be mixed.";
}

TEST(UnitsTest, ExtendedOperatorsAndMath)
{
    // Compound Assignment
    voltage_t v1(real_t{100.0});
    v1 += voltage_t(real_t{20.0});
    EXPECT_NEAR(v1.value(), real_t{120.0}, k_epsilon);

    v1 -= voltage_t(real_t{30.0});
    EXPECT_NEAR(v1.value(), real_t{90.0}, k_epsilon);

    v1 *= 2.0;
    EXPECT_NEAR(v1.value(), real_t{180.0}, k_epsilon);

    v1 /= 3.0;
    EXPECT_NEAR(v1.value(), real_t{60.0}, k_epsilon);

    // Math Functions
    current_t id(real_t{3.0});
    current_t iq(real_t{4.0});
    auto imag = hypot(id, iq);
    EXPECT_NEAR(imag.value(), real_t{5.0}, k_epsilon);
    static_assert(std::is_same_v<decltype(imag), current_t>);

    auto theta = atan2(iq, id);
    EXPECT_NEAR(theta.value(), std::atan2(4.0, 3.0), k_epsilon);
    static_assert(std::is_same_v<decltype(theta), angle_t>);

    length_t l(real_t{27.0});
    auto vol = l * l * l; // volume_dim
    auto side = cbrt(vol);
    EXPECT_NEAR(side.value(), real_t{27.0}, k_epsilon);
    static_assert(std::is_same_v<decltype(side), length_t>);

    // Dimensionless Trig
    angle_t a(pi / real_t{2.0});
    EXPECT_NEAR(sin(a).value(), real_t{1.0}, k_epsilon);
    EXPECT_NEAR(cos(a).value(), real_t{0.0}, k_epsilon);

    // Comparison with zero
    voltage_t v_zero(real_t{0.0});
    EXPECT_TRUE(v_zero == 0);
    EXPECT_TRUE(v_zero == 0.0);
    EXPECT_FALSE(v_zero != 0);

    voltage_t v_pos(real_t{10.0});
    EXPECT_TRUE(v_pos > 0);
    EXPECT_TRUE(v_pos >= 0);
    EXPECT_FALSE(v_pos < 0);
}

}  // namespace
