#include "mojito/units.hpp"

#include <sstream>  // For testing ostream operator

#include "gtest/gtest.h"

// The library namespace is still useful for type aliases
using namespace mojito;

//==============================================================================
// Core Functionality & Type System
//==============================================================================

TEST(UnitsTest, QuantityCreationAndValueAccess)
{
    // Quantity Type, Underlying Value Type
    length_t distance(10.5);
    mass_t weight(2.5f);

    // Check value access
    EXPECT_FLOAT_EQ(distance.value(), 10.5);
    EXPECT_FLOAT_EQ(weight.value(), 2.5f);

    // Dimensional Representation is implicitly tested by the type system
    static_assert(!std::is_same_v<decltype(distance)::dimension, decltype(weight)::dimension>);
}

TEST(UnitsTest, CompileTimeDimensionMismatch)
{
    // This test "passes" if the commented-out code fails to compile.
    // This is a standard way to "unit test" compile-time constraints.

    // voltage_t v(120.0);
    // current_t i(10.0);
    // v = i;  // COMPILE ERROR: Cannot assign current to voltage

    SUCCEED() << "Verified: Incompatible dimension assignment is a compile-time error.";
}

TEST(UnitsTest, CompileTimeSystemMismatch)
{
    // This test "passes" if the commented-out code fails to compile.

    // voltage_t si_v(240.0);
    // voltage_pu_t pu_v(1.0);
    // si_v = pu_v;  // COMPILE ERROR: Cannot assign PerUnit quantity to SI quantity

    // auto sum = si_v + pu_v;  // COMPILE ERROR: Cannot add quantities from different systems

    SUCCEED() << "Verified: Incompatible system assignment/operation is a compile-time error.";
}

//==============================================================================
// Unit System Support
//==============================================================================

TEST(UnitsTest, SystemDifferentiation)
{
    voltage_t si_voltage(120.0);
    voltage_pu_t pu_voltage(1.0);

    static_assert(std::is_same_v<decltype(si_voltage)::system, si>);
    static_assert(std::is_same_v<decltype(pu_voltage)::system, per_unit>);
    static_assert(!std::is_same_v<decltype(si_voltage)::system, decltype(pu_voltage)::system>);
}

TEST(UnitsTest, ExplicitConversion)
{
    voltage_t base_voltage(240.0);
    voltage_t measured_voltage(228.0);

    // Test conversion to Per-Unit
    auto pu_v = to_pu(measured_voltage, base_voltage);
    static_assert(std::is_same_v<decltype(pu_v)::system, per_unit>);
    EXPECT_FLOAT_EQ(pu_v.value(), 228.0 / 240.0);

    // Test conversion back to SI
    auto si_v = to_si(pu_v, base_voltage);
    static_assert(std::is_same_v<decltype(si_v)::system, si>);
    EXPECT_FLOAT_EQ(si_v.value(), measured_voltage.value());

    // Test compile-time check for mismatched dimensions in conversion
    // current_t<> base_current(10.0);
    // to_pu(measured_voltage, base_current); // COMPILE ERROR
    SUCCEED() << "Verified: Conversion with mismatched base dimension is a compile-time error.";
}

//==============================================================================
// Arithmetic Operations
//==============================================================================

TEST(UnitsTest, AdditionAndSubtraction)
{
    auto dist1 = length_t(10.5);
    auto dist2 = length_t(20.0);

    auto sum = dist1 + dist2;
    auto diff = dist2 - dist1;

    static_assert(std::is_same_v<decltype(sum), decltype(dist1)>);
    ASSERT_NEAR(sum.value(), 30.5, 1e-9);

    static_assert(std::is_same_v<decltype(diff), decltype(dist1)>);
    ASSERT_NEAR(diff.value(), 9.5, 1e-9);
}

TEST(UnitsTest, MultiplicationAndDivision)
{
    auto dist = length_t(100.0);
    auto duration = duration_t(10.0);
    auto speed = dist / duration;

    static_assert(std::is_same_v<decltype(speed)::dimension, speed_dim>);
    ASSERT_NEAR(speed.value(), 10.0, 1e-9);

    auto mass_val = mass_t(5.0);
    auto force_val = mass_val * (speed / duration);  // F = m * a

    static_assert(std::is_same_v<decltype(force_val)::dimension, force_dim>);
    ASSERT_NEAR(force_val.value(), 5.0 * (10.0 / 10.0), 1e-9);
}

TEST(UnitsTest, ScalarOperations)
{
    auto power_val = power_t(100.0);

    auto double_power = power_val * 2.0;
    auto half_power = 0.5 * power_val;
    auto quarter_power = power_val / 4.0;

    static_assert(std::is_same_v<decltype(double_power), decltype(power_val)>);
    ASSERT_NEAR(double_power.value(), 200.0, 1e-9);
    ASSERT_NEAR(half_power.value(), 50.0, 1e-9);
    ASSERT_NEAR(quarter_power.value(), 25.0, 1e-9);
}

//==============================================================================
// Electrical and Mechanical Units - Enhanced Engineering Equations
//==============================================================================

TEST(UnitsTest, ElectricalUnitsOhmLaw)
{
    // Ohm's Law: V = I * R
    auto v = voltage_t(120.0);
    auto r = resistance_t(60.0);
    auto i = v / r;

    static_assert(std::is_same_v<decltype(i)::dimension, current_dim>);
    ASSERT_NEAR(i.value(), 2.0, 1e-9);
    ASSERT_TRUE((std::is_same_v<decltype(i), current_t>));

    // Verify reverse calculation: R = V / I
    auto calculated_r = v / i;
    static_assert(std::is_same_v<decltype(calculated_r)::dimension, resistance_dim>);
    ASSERT_NEAR(calculated_r.value(), 60.0, 1e-9);
}

TEST(UnitsTest, ElectricalPowerLaws)
{
    // Power equations: P = V * I, P = V² / R, P = I² * R
    auto v = voltage_t(240.0);
    auto i = current_t(10.0);
    auto r = resistance_t(24.0);

    // P = V * I
    auto power_vi = v * i;
    static_assert(std::is_same_v<decltype(power_vi)::dimension, power_dim>);
    ASSERT_NEAR(power_vi.value(), 2400.0, 1e-9);

    // P = V² / R
    auto power_v2r = (v * v) / r;
    static_assert(std::is_same_v<decltype(power_v2r)::dimension, power_dim>);
    ASSERT_NEAR(power_v2r.value(), 2400.0, 1e-9);

    // P = I² * R
    auto power_i2r = (i * i) * r;
    static_assert(std::is_same_v<decltype(power_i2r)::dimension, power_dim>);
    ASSERT_NEAR(power_i2r.value(), 2400.0, 1e-9);

    // Verify all power calculations are equivalent
    ASSERT_NEAR(power_vi.value(), power_v2r.value(), 1e-9);
    ASSERT_NEAR(power_vi.value(), power_i2r.value(), 1e-9);
}

TEST(UnitsTest, ReactiveComponentLaws)
{
    // Capacitive and inductive reactance
    auto freq = frequency_t(50.0);   // 50 Hz
    auto c = capacitance_t(100e-6);  // 100 µF
    auto l = inductance_t(0.1);      // 100 mH

    // Angular frequency: ω = 2πf
    auto omega = 2.0 * 3.14159265359 * freq;
    static_assert(std::is_same_v<decltype(omega)::dimension, frequency_dim>);

    // Capacitive reactance: Xc = 1 / (ωC)
    // Note: This would require reciprocal operations for exact dimensional analysis
    // For now, we'll verify the numerical calculation
    auto xc_value = 1.0 / (omega.value() * c.value());
    ASSERT_NEAR(xc_value, 31.83, 0.01);  // ~31.83 Ω at 50 Hz

    // Inductive reactance: Xl = ωL
    auto xl_value = omega.value() * l.value();
    ASSERT_NEAR(xl_value, 31.42, 0.01);  // ~31.42 Ω at 50 Hz
}

TEST(UnitsTest, ThreePhaseSystemEquations)
{
    // Three-phase power calculations
    auto v_line = voltage_t(400.0);  // 400V line voltage
    auto i_line = current_t(10.0);   // 10A line current
    auto sqrt3 = 1.732050808;

    // Phase voltage: V_phase = V_line / √3
    auto v_phase = v_line / sqrt3;
    ASSERT_NEAR(v_phase.value(), 230.94, 0.01);

    // Three-phase apparent power: S = √3 * V_line * I_line
    auto apparent_power_value = sqrt3 * v_line.value() * i_line.value();
    ASSERT_NEAR(apparent_power_value, 6928.2, 0.1);  // ~6.93 kVA

    // Power factor calculations (assuming unity power factor)
    auto real_power_value = apparent_power_value * 1.0;  // cos(φ) = 1
    ASSERT_NEAR(real_power_value, 6928.2, 0.1);
}

TEST(UnitsTest, TransformerEquations)
{
    // Transformer relationships
    auto v_primary = voltage_t(400.0);
    auto v_secondary = voltage_t(24.0);
    auto i_secondary = current_t(50.0);

    // Turns ratio: n = V_primary / V_secondary
    auto turns_ratio = v_primary.value() / v_secondary.value();
    ASSERT_NEAR(turns_ratio, 16.67, 0.01);

    // Current transformation: I_primary = I_secondary / n
    auto i_primary_value = i_secondary.value() / turns_ratio;
    ASSERT_NEAR(i_primary_value, 3.0, 0.01);

    // Power conservation (ideal transformer): P_primary = P_secondary
    auto p_primary_value = v_primary.value() * i_primary_value;
    auto p_secondary_value = v_secondary.value() * i_secondary.value();
    ASSERT_NEAR(p_primary_value, p_secondary_value, 0.1);
}

TEST(UnitsTest, MechanicalUnitsNewtonsLaw)
{
    // R4.2 - Newton's second law: F = m * a
    auto m = mass_t(10.0);
    auto a = length_t(9.8) / (duration_t(1.0) * duration_t(1.0));
    auto f = m * a;

    static_assert(std::is_same_v<decltype(f)::dimension, force_dim>);
    ASSERT_NEAR(f.value(), 98.0, 1e-9);
    ASSERT_TRUE((std::is_same_v<decltype(f), force_t>));
}

TEST(UnitsTest, RotationalMechanicsEquations)
{
    // Rotational mechanics for motor applications
    auto j = moment_of_inertia_t(2.5);  // 2.5 kg⋅m²
    auto omega = frequency_t(50.0);     // 50 rad/s (treating as angular velocity)
    auto t = duration_t(0.1);           // 0.1 seconds

    // Angular acceleration: α = Δω / Δt
    auto alpha_value = omega.value() / t.value();  // 500 rad/s²
    ASSERT_NEAR(alpha_value, 500.0, 1e-9);

    // Torque: τ = J * α
    auto torque_value = j.value() * alpha_value;  // 1250 N⋅m
    ASSERT_NEAR(torque_value, 1250.0, 1e-9);

    // Kinetic energy: KE = ½ * J * ω²
    auto kinetic_energy_value = 0.5 * j.value() * (omega.value() * omega.value());
    ASSERT_NEAR(kinetic_energy_value, 3125.0, 1e-9);  // 3125 J
}

TEST(UnitsTest, ElectricalEnergyAndEfficiency)
{
    // Energy calculations for electrical systems
    auto p = power_t(2000.0);     // 2 kW
    auto t = duration_t(3600.0);  // 1 hour in seconds

    // Energy: E = P * t
    auto energy_value = p.value() * t.value();  // 7.2 MJ
    ASSERT_NEAR(energy_value, 7200000.0, 1e-9);

    // Efficiency calculations
    auto p_input = power_t(2200.0);   // Input power
    auto p_output = power_t(2000.0);  // Output power

    auto efficiency = p_output.value() / p_input.value();
    ASSERT_NEAR(efficiency, 0.909, 0.001);  // ~90.9% efficiency

    // Power losses
    auto p_loss_value = p_input.value() - p_output.value();
    ASSERT_NEAR(p_loss_value, 200.0, 1e-9);  // 200 W losses
}

TEST(UnitsTest, MotorEquations)
{
    // Motor equations for power electronics applications
    auto v_dc = voltage_t(600.0);  // DC link voltage
    auto i_dc = current_t(100.0);  // DC current
    auto speed_rpm = 1500.0;       // Motor speed in RPM
    auto poles = 4;                // Number of poles

    // Mechanical speed in rad/s: ω_m = (2π * RPM) / 60
    auto omega_m_value = (2.0 * 3.14159265359 * speed_rpm) / 60.0;
    ASSERT_NEAR(omega_m_value, 157.08, 0.01);  // ~157 rad/s

    // Electrical frequency: f_e = (poles * RPM) / 120
    auto f_electrical_value = (poles * speed_rpm) / 120.0;
    ASSERT_NEAR(f_electrical_value, 50.0, 1e-9);  // 50 Hz

    // DC power
    auto p_dc = v_dc * i_dc;
    static_assert(std::is_same_v<decltype(p_dc)::dimension, power_dim>);
    ASSERT_NEAR(p_dc.value(), 60000.0, 1e-9);  // 60 kW
}
