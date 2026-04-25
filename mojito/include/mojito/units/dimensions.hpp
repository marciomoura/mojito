#pragma once

namespace mojito {

/**
 * @brief Represents the 7 base SI dimensions via integer exponents.
 */
template <int L, int M, int T, int I, int Th, int N, int J>
struct dimension {};

template <typename D>
struct invert_dimensions;

template <int L, int M, int T, int I, int Th, int N, int J>
struct invert_dimensions<dimension<L, M, T, I, Th, N, J>> {
    using type = dimension<-L, -M, -T, -I, -Th, -N, -J>;
};

template <typename D1, typename D2>
struct multiply_dimensions;

template <int L1,
          int M1,
          int T1,
          int I1,
          int Th1,
          int N1,
          int J1,
          int L2,
          int M2,
          int T2,
          int I2,
          int Th2,
          int N2,
          int J2>
struct multiply_dimensions<dimension<L1, M1, T1, I1, Th1, N1, J1>, dimension<L2, M2, T2, I2, Th2, N2, J2>> {
    using type = dimension<L1 + L2, M1 + M2, T1 + T2, I1 + I2, Th1 + Th2, N1 + N2, J1 + J2>;
};

template <typename D1, typename D2>
struct divide_dimensions;

template <int L1,
          int M1,
          int T1,
          int I1,
          int Th1,
          int N1,
          int J1,
          int L2,
          int M2,
          int T2,
          int I2,
          int Th2,
          int N2,
          int J2>
struct divide_dimensions<dimension<L1, M1, T1, I1, Th1, N1, J1>, dimension<L2, M2, T2, I2, Th2, N2, J2>> {
    using type = dimension<L1 - L2, M1 - M2, T1 - T2, I1 - I2, Th1 - Th2, N1 - N2, J1 - J2>;
};

template <typename D, int N>
struct root_dimension;

template <int L, int M, int T, int I, int Th, int N_exp, int J, int N_root>
struct root_dimension<dimension<L, M, T, I, Th, N_exp, J>, N_root> {
    static_assert(L % N_root == 0 && M % N_root == 0 && T % N_root == 0 && I % N_root == 0 && Th % N_root == 0 &&
                      N_exp % N_root == 0 && J % N_root == 0,
                  "Cannot take a root of a dimension with non-divisible exponents.");

    using type = dimension<L / N_root, M / N_root, T / N_root, I / N_root, Th / N_root, N_exp / N_root, J / N_root>;
};

// Predefined SI Dimensions
using dimensionless_dim = dimension<0, 0, 0, 0, 0, 0, 0>;
using length_dim = dimension<1, 0, 0, 0, 0, 0, 0>;
using mass_dim = dimension<0, 1, 0, 0, 0, 0, 0>;
using time_dim = dimension<0, 0, 1, 0, 0, 0, 0>;
using current_dim = dimension<0, 0, 0, 1, 0, 0, 0>;

using area_dim = dimension<2, 0, 0, 0, 0, 0, 0>;
using volume_dim = dimension<3, 0, 0, 0, 0, 0, 0>;
using speed_dim = dimension<1, 0, -1, 0, 0, 0, 0>;
using acceleration_dim = dimension<1, 0, -2, 0, 0, 0, 0>;
using frequency_dim = dimension<0, 0, -1, 0, 0, 0, 0>;
using force_dim = dimension<1, 1, -2, 0, 0, 0, 0>;
using torque_dim = dimension<2, 1, -2, 0, 0, 0, 0>;
using moment_of_inertia_dim = dimension<2, 1, 0, 0, 0, 0, 0>;

using charge_dim = dimension<0, 0, 1, 1, 0, 0, 0>;
using voltage_dim = dimension<2, 1, -3, -1, 0, 0, 0>;
using power_dim = dimension<2, 1, -3, 0, 0, 0, 0>;
using resistance_dim = dimension<2, 1, -3, -2, 0, 0, 0>;
using capacitance_dim = dimension<-2, -1, 4, 2, 0, 0, 0>;
using inductance_dim = dimension<2, 1, -2, -2, 0, 0, 0>;
using magnetic_flux_dim = dimension<2, 1, -2, -1, 0, 0, 0>;
using mag_field_strength_dim = dimension<-1, 0, 0, 1, 0, 0, 0>;
using flux_density_dim = dimension<0, 1, -2, -1, 0, 0, 0>;

}  // namespace mojito
