#pragma once

#include <algorithm>
#include <cmath>
#if MOJITO_HAS_IOSTREAM
#include <ostream>
#endif
#include <string_view>
#include <type_traits>

#include "divisor.hpp"
#include "types.hpp"

namespace mojito {

namespace internal {
template <typename T, typename = void>
struct is_quantity : std::false_type {};

template <typename T>
struct is_quantity<T, std::void_t<decltype(std::declval<T>().value())>> : std::true_type {};
}  // namespace internal

template <typename T>
inline constexpr bool is_quantity_v = internal::is_quantity<T>::value;

template <typename T>
inline constexpr bool use_epsilon_comparison_v = std::is_floating_point_v<T> || is_quantity_v<T>;

/**
 * @brief Represents the 7 base SI dimensions via integer exponents.
 */
template <int L, int M, int T, int I, int Th, int N, int J>
struct dimension {};

struct si {};
struct per_unit {};

/**
 * @brief Represents a physical quantity with dimension and unit system.
 */
template <typename Dimension, typename UnitSystem>
class quantity {
public:
    using dimension = Dimension;
    using system = UnitSystem;

    constexpr quantity() : _value(real_t{0.0}) {}

    constexpr explicit quantity(real_t val) : _value(val) {}

    constexpr real_t value() const { return _value; }

    operator real_t() const { return _value; }

private:
    real_t _value;
};

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

template <typename D, typename S>
constexpr quantity<D, S> operator+(const quantity<D, S>& lhs, const quantity<D, S>& rhs)
{
    return quantity<D, S>(lhs.value() + rhs.value());
}

template <typename D, typename S>
constexpr quantity<D, S> operator-(const quantity<D, S>& lhs, const quantity<D, S>& rhs)
{
    return quantity<D, S>(lhs.value() - rhs.value());
}

template <typename D, typename S>
constexpr quantity<D, S> operator-(const quantity<D, S>& q)
{
    return quantity<D, S>(-q.value());
}

template <typename D1, typename D2, typename S>
constexpr auto operator*(const quantity<D1, S>& lhs, const quantity<D2, S>& rhs)
{
    using result_dim = typename multiply_dimensions<D1, D2>::type;
    return quantity<result_dim, S>(lhs.value() * rhs.value());
}

template <typename D1, typename D2, typename S>
constexpr auto operator/(const quantity<D1, S>& lhs, const quantity<D2, S>& rhs)
{
    using result_dim = typename divide_dimensions<D1, D2>::type;
    return quantity<result_dim, S>(lhs.value() / rhs.value());
}

template <typename D, typename S>
constexpr auto sqrt(const quantity<D, S>& q)
{
    using result_dim = typename root_dimension<D, 2>::type;
    return quantity<result_dim, S>(std::sqrt(q.value()));
}

template <typename D, typename S>
constexpr auto abs(const quantity<D, S>& q)
{
    return quantity<D, S>(std::abs(q.value()));
}

template <typename D, typename S>
constexpr quantity<D, S> max(const quantity<D, S>& lhs, const quantity<D, S>& rhs)
{
    return (lhs.value() > rhs.value()) ? lhs : rhs;
}

template <typename D, typename S>
constexpr quantity<D, S> min(const quantity<D, S>& lhs, const quantity<D, S>& rhs)
{
    return (lhs.value() < rhs.value()) ? lhs : rhs;
}

template <typename D, typename S, typename Scalar, std::enable_if_t<std::is_arithmetic_v<Scalar>, int> = 0>
constexpr quantity<D, S> operator*(const quantity<D, S>& q, const Scalar& s)
{
    return quantity<D, S>(q.value() * static_cast<real_t>(s));
}

template <typename D, typename S, typename Scalar, std::enable_if_t<std::is_arithmetic_v<Scalar>, int> = 0>
constexpr quantity<D, S> operator*(const Scalar& s, const quantity<D, S>& q)
{
    return q * s;
}

template <typename D, typename S, typename Scalar, std::enable_if_t<std::is_arithmetic_v<Scalar>, int> = 0>
constexpr quantity<D, S> operator/(const quantity<D, S>& q, const Scalar& s)
{
    return quantity<D, S>(q.value() / static_cast<real_t>(s));
}

template <typename D, typename S, typename Scalar, std::enable_if_t<std::is_arithmetic_v<Scalar>, int> = 0>
constexpr auto operator/(const Scalar& s, const quantity<D, S>& q)
{
    using inverted_dim = typename invert_dimensions<D>::type;
    return quantity<inverted_dim, S>(static_cast<real_t>(s) / q.value());
}

template <typename D, typename S>
constexpr bool operator==(const quantity<D, S>& lhs, const quantity<D, S>& rhs)
{
    return lhs.value() == rhs.value();
}

template <typename D, typename S>
constexpr bool operator!=(const quantity<D, S>& lhs, const quantity<D, S>& rhs)
{
    return !(lhs == rhs);
}

template <typename D, typename S>
constexpr bool operator<(const quantity<D, S>& lhs, const quantity<D, S>& rhs)
{
    return lhs.value() < rhs.value();
}

template <typename D, typename S>
constexpr bool operator>(const quantity<D, S>& lhs, const quantity<D, S>& rhs)
{
    return rhs < lhs;
}

template <typename D, typename S>
constexpr bool operator<=(const quantity<D, S>& lhs, const quantity<D, S>& rhs)
{
    return !(rhs < lhs);
}

template <typename D, typename S>
constexpr bool operator>=(const quantity<D, S>& lhs, const quantity<D, S>& rhs)
{
    return !(lhs < rhs);
}

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

// SI Quantity Type Aliases
using length_t = quantity<length_dim, si>;
using mass_t = quantity<mass_dim, si>;
using duration_t = quantity<time_dim, si>;
using current_t = quantity<current_dim, si>;
using speed_t = quantity<speed_dim, si>;
using acceleration_t = quantity<acceleration_dim, si>;
using frequency_t = quantity<frequency_dim, si>;
using force_t = quantity<force_dim, si>;
using torque_t = quantity<torque_dim, si>;
using moment_of_inertia_t = quantity<moment_of_inertia_dim, si>;
using angular_frequency_t = quantity<frequency_dim, si>;
using voltage_t = quantity<voltage_dim, si>;
using power_t = quantity<power_dim, si>;
using resistance_t = quantity<resistance_dim, si>;
using impedance_t = quantity<resistance_dim, si>;
using capacitance_t = quantity<capacitance_dim, si>;
using inductance_t = quantity<inductance_dim, si>;
using flux_t = quantity<magnetic_flux_dim, si>;
using magnetic_field_strength_t = quantity<mag_field_strength_dim, si>;
using flux_density_t = quantity<flux_density_dim, si>;
using angle_t = quantity<dimensionless_dim, si>;
using damping_coefficient_t = quantity<dimensionless_dim, si>;
using inertia_constant_t = quantity<time_dim, si>;

// Per-Unit Type Aliases
using length_pu_t = quantity<length_dim, per_unit>;
using mass_pu_t = quantity<mass_dim, per_unit>;
using duration_pu_t = quantity<time_dim, per_unit>;
using speed_pu_t = quantity<speed_dim, per_unit>;
using acceleration_pu_t = quantity<acceleration_dim, per_unit>;
using frequency_pu_t = quantity<frequency_dim, per_unit>;
using angular_frequency_pu_t = quantity<frequency_dim, per_unit>;
using force_pu_t = quantity<force_dim, per_unit>;
using torque_pu_t = quantity<torque_dim, per_unit>;
using moment_of_inertia_pu_t = quantity<moment_of_inertia_dim, per_unit>;
using voltage_pu_t = quantity<voltage_dim, per_unit>;
using current_pu_t = quantity<current_dim, per_unit>;
using power_pu_t = quantity<power_dim, per_unit>;
using resistance_pu_t = quantity<resistance_dim, per_unit>;
using impedance_pu_t = quantity<resistance_dim, per_unit>;
using capacitance_pu_t = quantity<capacitance_dim, per_unit>;
using inductance_pu_t = quantity<inductance_dim, per_unit>;
using flux_pu_t = quantity<magnetic_flux_dim, per_unit>;
using magnetic_field_strength_pu_t = quantity<mag_field_strength_dim, per_unit>;
using flux_density_pu_t = quantity<flux_density_dim, per_unit>;
using angle_pu_t = quantity<dimensionless_dim, per_unit>;
using damping_coefficient_pu_t = quantity<dimensionless_dim, per_unit>;
using inertia_constant_pu_t = quantity<time_dim, per_unit>;

template <typename Dim>
constexpr auto to_si(const quantity<Dim, per_unit>& pu_quantity, const quantity<Dim, si>& base_quantity)
{
    return quantity<Dim, si>(pu_quantity.value() * base_quantity.value());
}

template <typename Dim>
constexpr auto to_pu(const quantity<Dim, si>& si_quantity, const quantity<Dim, si>& base_quantity)
{
    return quantity<Dim, per_unit>(si_quantity.value() / base_quantity.value());
}

template <typename Dim>
constexpr auto to_pu(const quantity<Dim, si>& si_quantity, const divisor<quantity<Dim, si>>& divisor)
{
    return quantity<Dim, per_unit>(si_quantity.value() * divisor.reciprocal());
}

template <typename Dim>
constexpr auto to_si(const quantity<Dim, per_unit>& pu_quantity, const divisor<quantity<Dim, si>>& divisor)
{
    return quantity<Dim, si>(pu_quantity.value() * divisor.value().value());
}

namespace internal {
template <typename Dim>
constexpr std::string_view get_unit_symbol()
{
    if constexpr (std::is_same_v<Dim, dimensionless_dim>) return "";
    else if constexpr (std::is_same_v<Dim, length_dim>) return " m";
    else if constexpr (std::is_same_v<Dim, mass_dim>) return " kg";
    else if constexpr (std::is_same_v<Dim, time_dim>) return " s";
    else if constexpr (std::is_same_v<Dim, current_dim>) return " A";
    else if constexpr (std::is_same_v<Dim, area_dim>) return " m^2";
    else if constexpr (std::is_same_v<Dim, volume_dim>) return " m^3";
    else if constexpr (std::is_same_v<Dim, speed_dim>) return " m/s";
    else if constexpr (std::is_same_v<Dim, acceleration_dim>) return " m/s^2";
    else if constexpr (std::is_same_v<Dim, frequency_dim>) return " Hz";
    else if constexpr (std::is_same_v<Dim, force_dim>) return " N";
    else if constexpr (std::is_same_v<Dim, torque_dim>) return " Nm";
    else if constexpr (std::is_same_v<Dim, moment_of_inertia_dim>) return " kg*m^2";
    else if constexpr (std::is_same_v<Dim, charge_dim>) return " C";
    else if constexpr (std::is_same_v<Dim, voltage_dim>) return " V";
    else if constexpr (std::is_same_v<Dim, power_dim>) return " W";
    else if constexpr (std::is_same_v<Dim, resistance_dim>) return " Ohm";
    else if constexpr (std::is_same_v<Dim, capacitance_dim>) return " F";
    else if constexpr (std::is_same_v<Dim, inductance_dim>) return " H";
    else if constexpr (std::is_same_v<Dim, magnetic_flux_dim>) return " Wb";
    else if constexpr (std::is_same_v<Dim, mag_field_strength_dim>) return " A/m";
    else if constexpr (std::is_same_v<Dim, flux_density_dim>) return " T";
    else return " [unknown unit]";
}
}  // namespace internal

#if MOJITO_HAS_IOSTREAM
template <typename Dim, typename UnitSystem>
std::ostream& operator<<(std::ostream& os, const quantity<Dim, UnitSystem>& q)
{
    os << q.value();
    if constexpr (std::is_same_v<UnitSystem, si>) os << internal::get_unit_symbol<Dim>();
    else if constexpr (std::is_same_v<UnitSystem, per_unit>) os << " pu";
    return os;
}
#endif

template <typename NewType, typename OldDim>
constexpr quantity<typename NewType::dimension, per_unit> per_unit_cast(const quantity<OldDim, per_unit>& q)
{
    static_assert(std::is_same_v<typename NewType::system, per_unit>, "NewType must be a per-unit quantity type.");
    return quantity<typename NewType::dimension, per_unit>(q.value());
}

}  // namespace mojito
