#pragma once

#include "systems.hpp"
#include "dimensions.hpp"
#include "quantity.hpp"

namespace mojito {

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

// Percent Type Aliases
using length_percent_t = quantity<length_dim, percent>;
using mass_percent_t = quantity<mass_dim, percent>;
using duration_percent_t = quantity<time_dim, percent>;
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
