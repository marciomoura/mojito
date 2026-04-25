module;

#include <mojito/mojito.hpp>

export module mojito;

export namespace mojito {

// types.hpp
using ::mojito::real_t;

// constants.hpp
using ::mojito::pi;
using ::mojito::two_pi;
using ::mojito::half_pi;
using ::mojito::two_pi_thirds;
using ::mojito::sqrt_2;
using ::mojito::sqrt_3;

// type_traits.hpp
using ::mojito::fundamental_type;
using ::mojito::fundamental_type_t;
using ::mojito::is_coordinate_frame;
using ::mojito::is_coordinate_frame_v;

// units.hpp
using ::mojito::dimension;
using ::mojito::si;
using ::mojito::per_unit;
using ::mojito::percent;
using ::mojito::quantity;
using ::mojito::is_quantity_v;
using ::mojito::use_epsilon_comparison_v;

// Operators
using ::mojito::operator+;
using ::mojito::operator-;
using ::mojito::operator*;
using ::mojito::operator/;
using ::mojito::operator==;
using ::mojito::operator!=;
using ::mojito::operator<;
using ::mojito::operator>;
using ::mojito::operator<=;
using ::mojito::operator>=;
#if MOJITO_HAS_IOSTREAM
using ::mojito::operator<<;
#endif

// Dimensions
using ::mojito::dimensionless_dim;
using ::mojito::length_dim;
using ::mojito::mass_dim;
using ::mojito::time_dim;
using ::mojito::current_dim;
using ::mojito::area_dim;
using ::mojito::volume_dim;
using ::mojito::speed_dim;
using ::mojito::acceleration_dim;
using ::mojito::frequency_dim;
using ::mojito::force_dim;
using ::mojito::torque_dim;
using ::mojito::moment_of_inertia_dim;
using ::mojito::charge_dim;
using ::mojito::voltage_dim;
using ::mojito::power_dim;
using ::mojito::resistance_dim;
using ::mojito::capacitance_dim;
using ::mojito::inductance_dim;
using ::mojito::magnetic_flux_dim;
using ::mojito::mag_field_strength_dim;
using ::mojito::flux_density_dim;

// SI Units
using ::mojito::length_t;
using ::mojito::mass_t;
using ::mojito::duration_t;
using ::mojito::current_t;
using ::mojito::speed_t;
using ::mojito::acceleration_t;
using ::mojito::frequency_t;
using ::mojito::force_t;
using ::mojito::torque_t;
using ::mojito::moment_of_inertia_t;
using ::mojito::angular_frequency_t;
using ::mojito::voltage_t;
using ::mojito::power_t;
using ::mojito::resistance_t;
using ::mojito::impedance_t;
using ::mojito::capacitance_t;
using ::mojito::inductance_t;
using ::mojito::flux_t;
using ::mojito::magnetic_field_strength_t;
using ::mojito::flux_density_t;
using ::mojito::angle_t;
using ::mojito::damping_coefficient_t;
using ::mojito::inertia_constant_t;

// Per-Unit Units
using ::mojito::length_pu_t;
using ::mojito::mass_pu_t;
using ::mojito::duration_pu_t;
using ::mojito::speed_pu_t;
using ::mojito::acceleration_pu_t;
using ::mojito::frequency_pu_t;
using ::mojito::angular_frequency_pu_t;
using ::mojito::force_pu_t;
using ::mojito::torque_pu_t;
using ::mojito::moment_of_inertia_pu_t;
using ::mojito::voltage_pu_t;
using ::mojito::current_pu_t;
using ::mojito::power_pu_t;
using ::mojito::resistance_pu_t;
using ::mojito::impedance_pu_t;
using ::mojito::capacitance_pu_t;
using ::mojito::inductance_pu_t;
using ::mojito::flux_pu_t;
using ::mojito::magnetic_field_strength_pu_t;
using ::mojito::flux_density_pu_t;
using ::mojito::angle_pu_t;
using ::mojito::damping_coefficient_pu_t;
using ::mojito::inertia_constant_pu_t;

// Percent Units
using ::mojito::length_percent_t;
using ::mojito::mass_percent_t;
using ::mojito::duration_percent_t;
using ::mojito::speed_percent_t;
using ::mojito::acceleration_percent_t;
using ::mojito::frequency_percent_t;
using ::mojito::angular_frequency_percent_t;
using ::mojito::force_percent_t;
using ::mojito::torque_percent_t;
using ::mojito::moment_of_inertia_percent_t;
using ::mojito::voltage_percent_t;
using ::mojito::current_percent_t;
using ::mojito::power_percent_t;
using ::mojito::resistance_percent_t;
using ::mojito::impedance_percent_t;
using ::mojito::capacitance_percent_t;
using ::mojito::inductance_percent_t;
using ::mojito::flux_percent_t;
using ::mojito::magnetic_field_strength_percent_t;
using ::mojito::flux_density_percent_t;
using ::mojito::angle_percent_t;
using ::mojito::damping_coefficient_percent_t;
using ::mojito::inertia_constant_percent_t;

// divisor.hpp
using ::mojito::divisor;

// angle_wrapped.hpp
using ::mojito::angle_wrapped;

// Coordinate Frames
using ::mojito::abc;
using ::mojito::alphabeta;
using ::mojito::dq;
using ::mojito::ab_bc_ca;

// transform.hpp
using ::mojito::to_alphabeta;
using ::mojito::to_abc;
using ::mojito::to_dq;
using ::mojito::to_ab_bc_ca;
using ::mojito::to_phase_balanced;
using ::mojito::to_line;
using ::mojito::from_line;
using ::mojito::rotate_counter_clockwise;
using ::mojito::rotate_clockwise;
using ::mojito::make_abc;
using ::mojito::make_alphabeta;
using ::mojito::make_dq;
using ::mojito::cross_product;
using ::mojito::to_si;
using ::mojito::to_pu;
using ::mojito::to_percent;
using ::mojito::to_dimension_abc;
using ::mojito::to_dimension_alphabeta;
using ::mojito::to_dimension_dq;
using ::mojito::to_dimensionless_abc;
using ::mojito::to_dimensionless_alphabeta;
using ::mojito::to_dimensionless_dq;
using ::mojito::sum;
using ::mojito::max;
using ::mojito::min;
using ::mojito::per_unit_cast;
using ::mojito::percent_cast;

} // namespace mojito
