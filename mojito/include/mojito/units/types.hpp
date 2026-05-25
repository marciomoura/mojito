#pragma once

#include "systems.hpp"
#include "dimensions.hpp"
#include "quantity.hpp"

namespace mojito {

// SI Quantity Type Aliases
using length_t = quantity<length_dim, si>;
using mass_t = quantity<mass_dim, si>;
using duration_t = quantity<time_dim, si>;
using microseconds_t = quantity<time_dim, microseconds>;
using milliseconds_t = quantity<time_dim, milliseconds>;
using seconds_t = quantity<time_dim, seconds>;
using minutes_t = quantity<time_dim, minutes>;
using hours_t = quantity<time_dim, hours>;
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

// Default Per-Unit Type Aliases
using length_pu_t = quantity<length_dim, per_unit<default_base>>;
using mass_pu_t = quantity<mass_dim, per_unit<default_base>>;
using duration_pu_t = quantity<time_dim, per_unit<default_base>>;
using microseconds_pu_t = quantity<time_dim, per_unit<microseconds_base>>;
using milliseconds_pu_t = quantity<time_dim, per_unit<milliseconds_base>>;
using seconds_pu_t = quantity<time_dim, per_unit<seconds_base>>;
using minutes_pu_t = quantity<time_dim, per_unit<minutes_base>>;
using hours_pu_t = quantity<time_dim, per_unit<hours_base>>;
using speed_pu_t = quantity<speed_dim, per_unit<default_base>>;
using acceleration_pu_t = quantity<acceleration_dim, per_unit<default_base>>;
using frequency_pu_t = quantity<frequency_dim, per_unit<default_base>>;
using angular_frequency_pu_t = quantity<frequency_dim, per_unit<default_base>>;
using force_pu_t = quantity<force_dim, per_unit<default_base>>;
using torque_pu_t = quantity<torque_dim, per_unit<default_base>>;
using moment_of_inertia_pu_t = quantity<moment_of_inertia_dim, per_unit<default_base>>;
using voltage_pu_t = quantity<voltage_dim, per_unit<default_base>>;
using current_pu_t = quantity<current_dim, per_unit<default_base>>;
using power_pu_t = quantity<power_dim, per_unit<default_base>>;
using resistance_pu_t = quantity<resistance_dim, per_unit<default_base>>;
using impedance_pu_t = quantity<resistance_dim, per_unit<default_base>>;
using capacitance_pu_t = quantity<capacitance_dim, per_unit<default_base>>;
using inductance_pu_t = quantity<inductance_dim, per_unit<default_base>>;
using flux_pu_t = quantity<magnetic_flux_dim, per_unit<default_base>>;
using magnetic_field_strength_pu_t = quantity<mag_field_strength_dim, per_unit<default_base>>;
using flux_density_pu_t = quantity<flux_density_dim, per_unit<default_base>>;
using angle_pu_t = quantity<dimensionless_dim, per_unit<default_base>>;
using damping_coefficient_pu_t = quantity<dimensionless_dim, per_unit<default_base>>;
using inertia_constant_pu_t = quantity<time_dim, per_unit<default_base>>;

// Custom Per-Unit Templated Aliases
template <typename Base> using length_custom_pu_t = quantity<length_dim, per_unit<Base>>;
template <typename Base> using mass_custom_pu_t = quantity<mass_dim, per_unit<Base>>;
template <typename Base> using duration_custom_pu_t = quantity<time_dim, per_unit<Base>>;
template <typename Base> using microseconds_custom_pu_t = quantity<time_dim, per_unit<Base>>;
template <typename Base> using milliseconds_custom_pu_t = quantity<time_dim, per_unit<Base>>;
template <typename Base> using seconds_custom_pu_t = quantity<time_dim, per_unit<Base>>;
template <typename Base> using minutes_custom_pu_t = quantity<time_dim, per_unit<Base>>;
template <typename Base> using hours_custom_pu_t = quantity<time_dim, per_unit<Base>>;
template <typename Base> using speed_custom_pu_t = quantity<speed_dim, per_unit<Base>>;
template <typename Base> using acceleration_custom_pu_t = quantity<acceleration_dim, per_unit<Base>>;
template <typename Base> using frequency_custom_pu_t = quantity<frequency_dim, per_unit<Base>>;
template <typename Base> using angular_frequency_custom_pu_t = quantity<frequency_dim, per_unit<Base>>;
template <typename Base> using force_custom_pu_t = quantity<force_dim, per_unit<Base>>;
template <typename Base> using torque_custom_pu_t = quantity<torque_dim, per_unit<Base>>;
template <typename Base> using moment_of_inertia_custom_pu_t = quantity<moment_of_inertia_dim, per_unit<Base>>;
template <typename Base> using voltage_custom_pu_t = quantity<voltage_dim, per_unit<Base>>;
template <typename Base> using current_custom_pu_t = quantity<current_dim, per_unit<Base>>;
template <typename Base> using power_custom_pu_t = quantity<power_dim, per_unit<Base>>;
template <typename Base> using resistance_custom_pu_t = quantity<resistance_dim, per_unit<Base>>;
template <typename Base> using impedance_custom_pu_t = quantity<resistance_dim, per_unit<Base>>;
template <typename Base> using capacitance_custom_pu_t = quantity<capacitance_dim, per_unit<Base>>;
template <typename Base> using inductance_custom_pu_t = quantity<inductance_dim, per_unit<Base>>;
template <typename Base> using flux_custom_pu_t = quantity<magnetic_flux_dim, per_unit<Base>>;
template <typename Base> using magnetic_field_strength_custom_pu_t = quantity<mag_field_strength_dim, per_unit<Base>>;
template <typename Base> using flux_density_custom_pu_t = quantity<flux_density_dim, per_unit<Base>>;
template <typename Base> using angle_custom_pu_t = quantity<dimensionless_dim, per_unit<Base>>;
template <typename Base> using damping_coefficient_custom_pu_t = quantity<dimensionless_dim, per_unit<Base>>;
template <typename Base> using inertia_constant_custom_pu_t = quantity<time_dim, per_unit<Base>>;

// Percent Type Aliases
using length_percent_t = quantity<length_dim, percent>;
using mass_percent_t = quantity<mass_dim, percent>;
using duration_percent_t = quantity<time_dim, percent>;
using microseconds_percent_t = quantity<time_dim, percent>;
using milliseconds_percent_t = quantity<time_dim, percent>;
using seconds_percent_t = quantity<time_dim, percent>;
using minutes_percent_t = quantity<time_dim, percent>;
using hours_percent_t = quantity<time_dim, percent>;
using speed_percent_t = quantity<speed_dim, percent>;
using acceleration_percent_t = quantity<acceleration_dim, percent>;
using frequency_percent_t = quantity<frequency_dim, percent>;
using angular_frequency_percent_t = quantity<frequency_dim, percent>;
using force_percent_t = quantity<force_dim, percent>;
using torque_percent_t = quantity<torque_dim, percent>;
using moment_of_inertia_percent_t = quantity<moment_of_inertia_dim, percent>;
using voltage_percent_t = quantity<voltage_dim, percent>;
using current_percent_t = quantity<current_dim, percent>;
using power_percent_t = quantity<power_dim, percent>;
using resistance_percent_t = quantity<resistance_dim, percent>;
using impedance_percent_t = quantity<resistance_dim, percent>;
using capacitance_percent_t = quantity<capacitance_dim, percent>;
using inductance_percent_t = quantity<inductance_dim, percent>;
using flux_percent_t = quantity<magnetic_flux_dim, percent>;
using magnetic_field_strength_percent_t = quantity<mag_field_strength_dim, percent>;
using flux_density_percent_t = quantity<flux_density_dim, percent>;
using angle_percent_t = quantity<dimensionless_dim, percent>;
using damping_coefficient_percent_t = quantity<dimensionless_dim, percent>;
using inertia_constant_percent_t = quantity<time_dim, percent>;

}  // namespace mojito
