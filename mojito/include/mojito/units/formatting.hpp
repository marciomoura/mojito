#pragma once

#include "../types.hpp"
#include <string_view>
#if MOJITO_HAS_IOSTREAM
#include <ostream>
#endif
#include "systems.hpp"
#include "dimensions.hpp"
#include "quantity.hpp"

namespace mojito {

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
    else if constexpr (is_per_unit_v<UnitSystem>) os << " pu";
    else if constexpr (std::is_same_v<UnitSystem, percent>) os << " %";
    return os;
}
#endif

}  // namespace mojito
