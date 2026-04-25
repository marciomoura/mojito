#include <gtest/gtest.h>
#include <type_traits>

#include "mojito/constants.hpp"
#include "mojito/units.hpp"

namespace {

using namespace mojito;

static constexpr real_t k_epsilon = real_t{1e-4};

// --- Compile-time Safety Tests (Verification of "Should Fail" cases) ---

// Helper to check if an expression is valid
template <typename T, typename = void>
struct is_addition_valid : std::false_type {};

template <typename T>
struct is_addition_valid<T, std::void_t<decltype(std::declval<T>() + std::declval<T>())>> : std::true_type {};

// 1. Dimension Mismatch
static_assert(!std::is_assignable_v<voltage_t&, current_t>, "Cannot assign current to voltage");
// Note: + operator for different dimensions is not defined, so it should fail deduction.

// 2. System Mismatch
static_assert(!std::is_assignable_v<voltage_t&, voltage_pu_t>, "Cannot assign PU to SI");
static_assert(!std::is_assignable_v<voltage_pu_t&, voltage_percent_t>, "Cannot assign Percent to PU");

// 3. Custom Base Mismatch
struct base_a {};
struct base_b {};
using v_a_t = voltage_custom_pu_t<base_a>;
using v_b_t = voltage_custom_pu_t<base_b>;
static_assert(!std::is_assignable_v<v_a_t&, v_b_t>, "Different custom bases are incompatible");

// --- Unit Tests ---

TEST(UnitsTest, QuantityCreationAndValueAccess)
{
    length_t distance(real_t{10.5});
    mass_t weight(real_t{2.5});

    EXPECT_NEAR(distance.value(), real_t{10.5}, k_epsilon);
    EXPECT_NEAR(weight.value(), real_t{2.5}, k_epsilon);
    
    // Test implicit conversion to real_t
    real_t raw_dist = distance;
    EXPECT_NEAR(raw_dist, real_t{10.5}, k_epsilon);
}

TEST(UnitsTest, BasicArithmetic)
{
    auto d1 = length_t(10.0);
    auto d2 = length_t(5.0);

    // Addition/Subtraction
    EXPECT_NEAR((d1 + d2).value(), 15.0, k_epsilon);
    EXPECT_NEAR((d1 - d2).value(), 5.0, k_epsilon);
    EXPECT_NEAR((-d1).value(), -10.0, k_epsilon);

    // Multiplication/Division (Dimensions)
    auto area = d1 * d2;
    static_assert(std::is_same_v<decltype(area), quantity<area_dim, si>>);
    EXPECT_NEAR(area.value(), 50.0, k_epsilon);

    auto ratio = d1 / d2;
    static_assert(std::is_same_v<decltype(ratio), quantity<dimensionless_dim, si>>);
    EXPECT_NEAR(ratio.value(), 2.0, k_epsilon);

    // Scalar Operations
    EXPECT_NEAR((d1 * 2.0).value(), 20.0, k_epsilon);
    EXPECT_NEAR((2.0 * d1).value(), 20.0, k_epsilon);
    EXPECT_NEAR((d1 / 2.0).value(), 5.0, k_epsilon);
    
    auto inv_d = 1.0 / d1;
    static_assert(std::is_same_v<typename decltype(inv_d)::dimension, dimension<-1, 0, 0, 0, 0, 0, 0>>);
    EXPECT_NEAR(inv_d.value(), 0.1, k_epsilon);
}

TEST(UnitsTest, CompoundAssignments)
{
    length_t d(10.0);
    
    d += length_t(5.0);
    EXPECT_NEAR(d.value(), 15.0, k_epsilon);
    
    d -= length_t(3.0);
    EXPECT_NEAR(d.value(), 12.0, k_epsilon);
    
    d *= 2.0;
    EXPECT_NEAR(d.value(), 24.0, k_epsilon);
    
    d /= 4.0;
    EXPECT_NEAR(d.value(), 6.0, k_epsilon);
}

TEST(UnitsTest, RelationalOperators)
{
    length_t d1(10.0);
    length_t d2(20.0);
    length_t d3(10.0);

    EXPECT_TRUE(d1 < d2);
    EXPECT_TRUE(d2 > d1);
    EXPECT_TRUE(d1 <= d2);
    EXPECT_TRUE(d1 <= d3);
    EXPECT_TRUE(d2 >= d1);
    EXPECT_TRUE(d1 >= d3);
    EXPECT_TRUE(d1 == d3);
    EXPECT_TRUE(d1 != d2);

    // Comparison with zero
    EXPECT_TRUE(d1 > 0);
    EXPECT_TRUE(d1 > 0.0);
    EXPECT_TRUE(length_t(0.0) == 0);
    EXPECT_FALSE(d1 < 0);
}

TEST(UnitsTest, MathFunctions)
{
    // Sqrt / Cbrt
    EXPECT_NEAR(sqrt(quantity<area_dim, si>(16.0)).value(), 4.0, k_epsilon);
    EXPECT_NEAR(cbrt(quantity<volume_dim, si>(27.0)).value(), 3.0, k_epsilon);

    // Hypot / Atan2
    length_t x(3.0);
    length_t y(4.0);
    EXPECT_NEAR(hypot(x, y).value(), 5.0, k_epsilon);
    EXPECT_NEAR(atan2(y, x).value(), std::atan2(4.0, 3.0), k_epsilon);

    // Abs / Min / Max
    EXPECT_NEAR(abs(length_t(-5.0)).value(), 5.0, k_epsilon);
    EXPECT_NEAR(max(length_t(10.0), length_t(20.0)).value(), 20.0, k_epsilon);
    EXPECT_NEAR(min(length_t(10.0), length_t(20.0)).value(), 10.0, k_epsilon);

    // Trig
    angle_t a(pi / 4.0);
    EXPECT_NEAR(sin(a).value(), std::sin(pi / 4.0), k_epsilon);
    EXPECT_NEAR(cos(a).value(), std::cos(pi / 4.0), k_epsilon);
    EXPECT_NEAR(tan(a).value(), std::tan(pi / 4.0), k_epsilon);
}

TEST(UnitsTest, PerUnitSystem)
{
    voltage_t base_v(230.0);
    voltage_t si_v(253.0);

    // SI -> PU
    auto pu_v = to_pu(si_v, base_v);
    EXPECT_NEAR(pu_v.value(), 1.1, k_epsilon);
    static_assert(is_per_unit_v<decltype(pu_v)::system>);

    // PU -> SI
    auto si_back = to_si(pu_v, base_v);
    EXPECT_NEAR(si_back.value(), 253.0, k_epsilon);

    // Using Divisor
    divisor<voltage_t> inv_base_v(base_v);
    auto pu_v2 = to_pu(si_v, inv_base_v);
    EXPECT_NEAR(pu_v2.value(), 1.1, k_epsilon);

    auto si_back2 = to_si(pu_v2, inv_base_v);
    EXPECT_NEAR(si_back2.value(), 253.0, k_epsilon);
}

TEST(UnitsTest, CustomPerUnitBases)
{
    struct machine_base {};
    struct grid_base {};

    voltage_t v_si(100.0);
    voltage_t base_m(100.0);
    voltage_t base_g(200.0);

    auto v_m = to_pu<machine_base>(v_si, base_m);
    auto v_g = to_pu<grid_base>(v_si, base_g);

    EXPECT_NEAR(v_m.value(), 1.0, k_epsilon);
    EXPECT_NEAR(v_g.value(), 0.5, k_epsilon);

    static_assert(!std::is_same_v<decltype(v_m), decltype(v_g)>);
    
    // Casting between PU systems
    auto v_m_as_g = per_unit_cast<voltage_custom_pu_t<grid_base>>(v_m);
    EXPECT_NEAR(v_m_as_g.value(), 1.0, k_epsilon); // Note: cast just copies value, doesn't re-scale
}

TEST(UnitsTest, PercentSystem)
{
    voltage_t base_v(100.0);
    voltage_t si_v(75.0);

    // SI -> Percent
    auto pct_v = to_percent(si_v, base_v);
    EXPECT_NEAR(pct_v.value(), 75.0, k_epsilon);

    // Percent -> SI
    auto si_back = to_si(pct_v, base_v);
    EXPECT_NEAR(si_back.value(), 75.0, k_epsilon);

    // PU <-> Percent
    voltage_pu_t pu_v(0.5);
    auto pct_from_pu = to_percent(pu_v);
    EXPECT_NEAR(pct_from_pu.value(), 50.0, k_epsilon);

    auto pu_from_pct = to_pu(pct_from_pu);
    EXPECT_NEAR(pu_from_pct.value(), 0.5, k_epsilon);
    
    // Percent Cast
    using duty_cycle_t = quantity<dimensionless_dim, percent>;
    auto d = percent_cast<duty_cycle_t>(pct_v);
    EXPECT_NEAR(d.value(), 75.0, k_epsilon);
}

TEST(UnitsTest, PhysicalLaws)
{
    // Ohm's Law: V = I * R
    voltage_t v(120.0);
    resistance_t r(60.0);
    auto i = v / r;
    static_assert(std::is_same_v<decltype(i), current_t>);
    EXPECT_NEAR(i.value(), 2.0, k_epsilon);

    // Power: P = V * I
    auto p = v * i;
    static_assert(std::is_same_v<decltype(p), power_t>);
    EXPECT_NEAR(p.value(), 240.0, k_epsilon);

    // Kinetic Energy: E = 0.5 * m * v^2
    mass_t m(10.0);
    speed_t vel(2.0);
    auto e = 0.5 * m * vel * vel;
    // Energy dimension: M * (L/T)^2 = M * L^2 * T^-2 (Same as torque/work)
    static_assert(std::is_same_v<typename decltype(e)::dimension, torque_dim>);
    EXPECT_NEAR(e.value(), 20.0, k_epsilon);
}

}  // namespace
