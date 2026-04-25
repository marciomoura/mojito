#include <gtest/gtest.h>
#include <cmath>
#include "mojito/mojito.hpp"

namespace {

using namespace mojito;

static constexpr real_t k_epsilon = real_t{1e-5};

TEST(PuConversionTest, AbcConversion)
{
    voltage_t base_v{real_t{100.0}};
    abc<voltage_pu_t> pu_v{voltage_pu_t{real_t{1.0}}, voltage_pu_t{real_t{-0.5}}, voltage_pu_t{real_t{-0.5}}};
    
    auto si_v = to_si(pu_v, base_v);
    EXPECT_NEAR(si_v.a().value(), real_t{100.0}, k_epsilon);
    EXPECT_NEAR(si_v.b().value(), real_t{-50.0}, k_epsilon);
    EXPECT_NEAR(si_v.c().value(), real_t{-50.0}, k_epsilon);

    auto back_pu = to_pu(si_v, base_v);
    EXPECT_NEAR(back_pu.a().value(), real_t{1.0}, k_epsilon);
}

TEST(PuConversionTest, AlphabetaConversion)
{
    current_t base_i{real_t{10.0}};
    alphabeta<current_pu_t> pu_i{current_pu_t{real_t{1.0}}, current_pu_t{real_t{0.0}}};

    auto si_i = to_si(pu_i, base_i);
    EXPECT_NEAR(si_i.alpha().value(), real_t{10.0}, k_epsilon);
    EXPECT_NEAR(si_i.beta().value(), real_t{0.0}, k_epsilon);

    auto back_pu = to_pu(si_i, base_i);
    EXPECT_NEAR(back_pu.alpha().value(), real_t{1.0}, k_epsilon);
}

TEST(PuConversionTest, DqConversion)
{
    torque_t base_t{real_t{50.0}};
    dq<torque_pu_t> pu_t{torque_pu_t{real_t{0.8}}, torque_pu_t{real_t{0.6}}};

    auto si_t = to_si(pu_t, base_t);
    EXPECT_NEAR(si_t.d().value(), real_t{40.0}, k_epsilon);
    EXPECT_NEAR(si_t.q().value(), real_t{30.0}, k_epsilon);

    auto back_pu = to_pu(si_t, base_t);
    EXPECT_NEAR(back_pu.d().value(), real_t{0.8}, k_epsilon);
}

TEST(PuConversionTest, DivisorConversion)
{
    voltage_t base_v{real_t{100.0}};
    divisor<voltage_t> v_div{base_v};
    abc<voltage_t> si_v{voltage_t{real_t{100.0}}, voltage_t{real_t{-50.0}}, voltage_t{real_t{-50.0}}};

    auto pu_v = to_pu(si_v, v_div);
    EXPECT_NEAR(pu_v.a().value(), real_t{1.0}, k_epsilon);
    EXPECT_NEAR(pu_v.b().value(), real_t{-0.5}, k_epsilon);
    EXPECT_NEAR(pu_v.c().value(), real_t{-0.5}, k_epsilon);
}

}  // namespace
