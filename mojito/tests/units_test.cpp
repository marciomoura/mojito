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

TEST(UnitsTest, SystemDifferentiation)
{
    voltage_t si_voltage(real_t{120.0});
    voltage_pu_t pu_voltage(real_t{1.0});

    static_assert(std::is_same_v<decltype(si_voltage)::system, si>);
    static_assert(std::is_same_v<decltype(pu_voltage)::system, per_unit>);
}

TEST(UnitsTest, ExplicitConversion)
{
    voltage_t base_voltage(real_t{240.0});
    voltage_t measured_voltage(real_t{228.0});

    auto pu_v = to_pu(measured_voltage, base_voltage);
    EXPECT_NEAR(pu_v.value(), real_t{228.0 / 240.0}, k_epsilon);

    auto si_v = to_si(pu_v, base_voltage);
    EXPECT_NEAR(si_v.value(), measured_voltage.value(), k_epsilon);
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

}  // namespace
