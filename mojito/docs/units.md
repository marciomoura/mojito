# Dimension and Units Library

## 1. Overview

This library provides a type-safe, zero-overhead C++17 framework for dimensional analysis in scientific and engineering computations. Its primary goal is to leverage the C++ type system to catch unit-related errors at compile time, preventing a common class of bugs in numerical code.

By encoding physical dimensions (like Length, Mass, Time) and unit systems (like SI and Per-Unit) into types, the library ensures that operations are physically coherent. This eliminates logical errors such as adding a voltage to a current or comparing a value in SI units to a per-unit value without explicit conversion. The design guarantees that these compile-time checks add no performance penalty to runtime calculations.

## 2. Core Concepts

### Dimensional Representation
Physical dimensions are represented using a template-based system that encodes the seven base SI units as integer exponents:

```cpp
template <int L, int M, int T, int I, int Th, int N, int J>
struct dimension {};
```

Where:
- **L**: Length exponent
- **M**: Mass exponent  
- **T**: Time exponent
- **I**: Current exponent
- **Th**: Temperature exponent
- **N**: Amount of substance exponent
- **J**: Luminous intensity exponent

For example, a `Velocity` (Length/Time) has dimension `<1, 0, -1, 0, 0, 0, 0>`, while a `Force` (Mass × Length / Time²) has dimension `<1, 1, -2, 0, 0, 0, 0>`.

### The Quantity
The central component of the library is the **quantity** template class:

```cpp
template <typename Dimension, typename UnitSystem>
class quantity {
    // Uses real_t as representation type
    // Implementation details...
};
```

A `quantity` bundles:
- A numerical value (using `real_t` which is `float` by default or `double` with `YEET_PRECISION_DOUBLE`)
- Its physical dimension (encoded as a type)
- Its unit system (`si` or `per_unit`)

### Unit System Differentiation
The library provides first-class support for two unit systems through tag types:

```cpp
struct si {};        // International System of Units
struct per_unit {};  // Per-Unit system for power systems
```

## 3. Key Features

### Compile-Time Safety
The library enforces correctness before the program runs:

#### Dimensional Correctness
Operations between incompatible dimensions result in compile-time errors:

```cpp
auto length_val = length_t{10.0};           // 10 meters
auto time_val = duration_t{2.0};            // 2 seconds
auto velocity_val = length_val / time_val;  // OK: Length/Time = Velocity
// auto invalid = length_val + time_val;    // Compile error!
```

#### Unit System Integrity
Direct mixing of SI and Per-Unit quantities is prohibited:

```cpp
auto v_si = voltage_t{120.0};      // 120 V (SI)
auto v_pu = voltage_pu_t{0.95};    // 0.95 pu
// auto invalid = v_si + v_pu;     // Compile error!
```

### Comprehensive Arithmetic Operations

#### Addition & Subtraction
Only permitted between quantities of identical dimension and unit system:

```cpp
auto v1 = voltage_t{120.0};
auto v2 = voltage_t{240.0};
auto v_total = v1 + v2;  // Result: 360 V
```

#### Multiplication & Division
Automatically calculates resulting dimensions based on dimensional analysis:

```cpp
auto v = voltage_t{120.0};  // Volts
auto i = current_t{5.0};    // Amperes
auto p = v * i;             // Result: power_t (Watts)

auto f = force_t{100.0};    // Newtons
auto a = length_t{10.0};    // Meters (area)
auto pressure = f / a;      // Result: pressure (force_dim/area_dim)
```

#### Scalar Operations
Quantities can be multiplied or divided by plain numbers:

```cpp
auto base_voltage = voltage_t{120.0};
auto doubled = base_voltage * 2.0;  // 240 V
auto halved = base_voltage / 2.0;   // 60 V
```

### Predefined Unit Types

All quantity types use `real_t` as the underlying representation type (configurable as `float` or `double` via `YEET_PRECISION_DOUBLE`).

#### Base SI Units
```cpp
using length_t = quantity<length_dim, si>;
using mass_t = quantity<mass_dim, si>;
using duration_t = quantity<time_dim, si>;
using current_t = quantity<current_dim, si>;
```

#### Mechanical Units
```cpp
using speed_t = quantity<speed_dim, si>;
using acceleration_t = quantity<acceleration_dim, si>;
using frequency_t = quantity<frequency_dim, si>;
using angular_frequency_t = quantity<frequency_dim, si>;
using force_t = quantity<force_dim, si>;
using torque_t = quantity<torque_dim, si>;
using moment_of_inertia_t = quantity<moment_of_inertia_dim, si>;
using angle_t = quantity<dimensionless_dim, si>;
using damping_coefficient_t = quantity<dimensionless_dim, si>;
using inertia_constant_t = quantity<time_dim, si>;
```

#### Electrical Units
```cpp
using voltage_t = quantity<voltage_dim, si>;
using power_t = quantity<power_dim, si>;
using resistance_t = quantity<resistance_dim, si>;
using impedance_t = quantity<resistance_dim, si>;
using capacitance_t = quantity<capacitance_dim, si>;
using inductance_t = quantity<inductance_dim, si>;
using flux_t = quantity<magnetic_flux_dim, si>;
using magnetic_field_strength_t = quantity<mag_field_strength_dim, si>;
using flux_density_t = quantity<flux_density_dim, si>;
```

#### Per-Unit Equivalents
```cpp
// Base quantities
using length_pu_t = quantity<length_dim, per_unit>;
using mass_pu_t = quantity<mass_dim, per_unit>;
using duration_pu_t = quantity<time_dim, per_unit>;

// Mechanical quantities
using speed_pu_t = quantity<speed_dim, per_unit>;
using acceleration_pu_t = quantity<acceleration_dim, per_unit>;
using frequency_pu_t = quantity<frequency_dim, per_unit>;
using angular_frequency_pu_t = quantity<frequency_dim, per_unit>;
using force_pu_t = quantity<force_dim, per_unit>;
using torque_pu_t = quantity<torque_dim, per_unit>;
using moment_of_inertia_pu_t = quantity<moment_of_inertia_dim, per_unit>;

// Electrical quantities
using voltage_pu_t = quantity<voltage_dim, per_unit>;
using current_pu_t = quantity<current_dim, per_unit>;
using power_pu_t = quantity<power_dim, per_unit>;
using resistance_pu_t = quantity<resistance_dim, per_unit>;
using impedance_pu_t = quantity<impedance_dim, per_unit>;
using capacitance_pu_t = quantity<capacitance_dim, per_unit>;
using inductance_pu_t = quantity<inductance_dim, per_unit>;
using flux_pu_t = quantity<magnetic_flux_dim, per_unit>;
using magnetic_field_strength_pu_t = quantity<mag_field_strength_dim, per_unit>;
using flux_density_pu_t = quantity<flux_density_dim, per_unit>;

// Dimensionless quantities
using angle_pu_t = quantity<dimensionless_dim, per_unit>;
using damping_coefficient_pu_t = quantity<dimensionless_dim, per_unit>;
using inertia_constant_pu_t = quantity<time_dim, per_unit>;
```

### Safe System Conversion
Explicit conversion functions ensure safe transitions between unit systems:

```cpp
// Convert from Per-Unit to SI (requires base quantity)
auto v_pu = voltage_pu_t{1.05};   // 1.05 pu
auto v_base = voltage_t{400.0};   // 400 V base
auto v_si = to_si(v_pu, v_base);  // Result: 420 V

// Convert from SI to Per-Unit
auto v_actual = voltage_t{380.0};         // 380 V actual
auto v_pu_result = to_pu(v_actual, v_base); // Result: 0.95 pu
```

### Comparison Operations
Standard comparison operators work with quantities of the same dimension and system:

```cpp
auto v1 = voltage_t{120.0};
auto v2 = voltage_t{240.0};

bool is_greater = (v2 > v1);    // true
bool is_equal = (v1 == v2);     // false
bool is_less_eq = (v1 <= v2);   // true
```

### Human-Readable Output
Quantities automatically format with appropriate unit symbols:

```cpp
auto v = voltage_t{120.0};
auto i = current_t{5.0};
auto p_pu = power_pu_t{0.85};

std::cout << v << std::endl;     // Output: "120 V"
std::cout << i << std::endl;     // Output: "5 A"
std::cout << p_pu << std::endl;  // Output: "0.85 pu"
```

## 4. Usage Examples

### Basic Electrical Calculations

```cpp
// Ohm's Law: V = I × R
auto current_val = current_t{10.0};         // 10 A
auto resistance_val = resistance_t{12.0};   // 12 Ω
auto voltage_val = current_val * resistance_val;  // 120 V

// Power calculation: P = V × I
auto power_val = voltage_val * current_val;       // 1200 W
```

### Three-Phase Power Systems

```cpp
// Line voltage and current
auto v_line = voltage_t{400.0};  // 400 V line voltage
auto i_line = current_t{50.0};   // 50 A line current

// Three-phase power (simplified, without √3 factor in this example)
auto power_3ph = v_line * i_line * 3.0;  // Approximate 3-phase power
```

### Per-Unit System Usage

```cpp
// Base quantities for per-unit system
auto v_base = voltage_t{400.0};     // 400 V base
auto p_base = power_t{100000.0};    // 100 kW base

// Per-unit quantities
auto v_pu = voltage_pu_t{1.05};  // 1.05 pu voltage
auto p_pu = power_pu_t{0.80};    // 0.80 pu power

// Convert to SI for actual calculations
auto v_actual = to_si(v_pu, v_base);  // 420 V
auto p_actual = to_si(p_pu, p_base);  // 80 kW
```

### Mechanical Calculations

```cpp
// Rotational mechanics
auto inertia_val = moment_of_inertia_t{2.5};  // 2.5 kg⋅m²
auto torque_val = torque_t{100.0};            // 100 N⋅m

// Note: Angular acceleration would require additional dimension arithmetic
// This demonstrates the type safety of the system
```

## 5. Integration with Power Electronics Applications

### LCI (Load-Commutated Inverter) Applications
The units library is particularly valuable for LCI and thyristor-based power conversion:

```cpp
// DC link calculations
auto v_dc = voltage_t{600.0};   // DC voltage
auto i_dc = current_t{100.0};   // DC current
auto p_dc = v_dc * i_dc;        // DC power

// Per-unit conversion for control systems
auto v_dc_base = voltage_t{650.0};
auto v_dc_pu = to_pu(v_dc, v_dc_base);  // 0.923 pu
```

### Soft-Starter Applications
```cpp
// Motor nameplate data
auto v_rated = voltage_t{400.0};     // Rated voltage
auto p_rated = power_t{200000.0};    // Rated power (200 kW)
auto f_rated = frequency_t{50.0};    // Rated frequency

// Soft-start calculations with reduced voltage
auto v_start = v_rated * 0.6;             // 60% voltage start
auto v_start_pu = to_pu(v_start, v_rated); // 0.6 pu
```

## 6. Performance Characteristics

### Zero Runtime Overhead
The dimensional analysis is performed entirely at compile time. Runtime operations are equivalent to operations on raw numeric types:

```cpp
// This quantity operation...
auto result = voltage_val * current_val;

// ...compiles to the same assembly as:
// real_t result = voltage_raw * current_raw;
```

### Memory Efficiency
A `quantity` object has the same memory footprint as its underlying numeric type:

```cpp
static_assert(sizeof(voltage_t) == sizeof(real_t));    // Always true
static_assert(sizeof(power_pu_t) == sizeof(real_t));   // Always true
```

### Compile-Time Validation
All dimensional checking happens during compilation, ensuring no runtime performance penalty for type safety.

## 7. Advanced Features

### Template Metaprogramming
The library uses sophisticated template metaprogramming for dimension arithmetic:

```cpp
// Automatic dimension calculation for complex expressions
auto energy = force_t{100.0} * length_t{50.0};        // 5000 J (energy_dim)
auto power_from_energy = energy / duration_t{10.0};   // 500 W (power_dim)
```

### Precision Control
The underlying representation type is controlled via the `YEET_PRECISION_DOUBLE` macro:

```cpp
// By default, uses float (real_t = float)
// Define YEET_PRECISION_DOUBLE to use double precision
#define YEET_PRECISION_DOUBLE
#include "yeet_control/units.hpp"
// Now real_t = double, all quantities use double precision
```

### Extensibility
New unit types can be easily added by defining new dimension combinations:

```cpp
// Example: Creating custom dimensions
// (Would require defining new dimension types in the library)
```

## 8. Error Prevention Examples

The library prevents common engineering mistakes at compile time:

```cpp
// These will cause compilation errors:
// auto invalid1 = voltage_val + current_val;        // Different dimensions
// auto invalid2 = voltage_val + voltage_pu_val;     // Different unit systems
// auto invalid3 = voltage_val == power_val;         // Incompatible comparison
```

## 9. Best Practices

### Consistent Unit System Usage
Always use consistent unit systems within calculation blocks:

```cpp
// Good: All SI units
auto v_si = voltage_t{400.0};
auto i_si = current_t{50.0};
auto p_si = v_si * i_si;

// Good: All per-unit
auto v_pu = voltage_pu_t{1.0};
auto i_pu = current_pu_t{1.0};
auto p_pu = v_pu * i_pu;
```

### Explicit Conversions
Always use explicit conversion functions when changing unit systems:

```cpp
auto v_base = voltage_t{400.0};
auto v_pu = voltage_pu_t{1.05};
auto v_si = to_si(v_pu, v_base);  // Explicit and safe
```

### Type Aliases for Domain-Specific Applications
Create meaningful type aliases for your specific application domain:

```cpp
using grid_voltage_t = voltage_t;
using motor_current_t = current_t;
using firing_angle_t = angle_t;
```

## 10. Compliance and Dependencies

- **C++17 Standard**: Full compliance with C++17 features including `constexpr`, `if constexpr`, and structured bindings support
- **No External Dependencies**: Uses only standard library components (`<iostream>`, `<string>`, `<type_traits>`)
- **Header-Only**: Complete implementation in a single header file for easy integration
- **Cross-Platform**: Works on all major compilers (GCC, Clang, MSVC) and platforms

## 11. Integration with YeeT Control Library

The units library integrates seamlessly with other YeeT control components:

### PI Controller Integration
```cpp
// PI controller with typed quantities
pi_controller<voltage_t> voltage_controller;
auto voltage_error = voltage_ref - voltage_measured;
auto voltage_output = voltage_controller.update(voltage_error);
```

### Reference Frame Transformations
```cpp
// Three-phase voltage measurements with units
abc<voltage_pu_t> voltages{
    voltage_pu_t{1.0}, 
    voltage_pu_t{-0.5}, 
    voltage_pu_t{-0.5}
};

// Clarke and Park transforms preserve unit types
alphabeta<voltage_pu_t> ab = voltages.to_alphabeta();
dq<voltage_pu_t> dq_frame = ab.to_dq(theta);
```

### LCI Pulse Generator
```cpp
// Firing angle control with type safety
auto firing_angle = angle_t{15.0 * pi / 180.0};  // 15 degrees
auto commutation_voltage = voltage_t{500.0};
// Pulse generator configuration...
```

## 12. Types Reference

### Basic Type (`real_t`)

The fundamental type used by all quantities:

```cpp
// Default: float precision
using real_t = float;

// Or with YEET_PRECISION_DOUBLE defined: double precision
using real_t = double;
```

### Complete Type List

**SI Unit Types** (suffix `_t`):
- `length_t`, `mass_t`, `duration_t`, `current_t`
- `speed_t`, `acceleration_t`, `frequency_t`, `angular_frequency_t`
- `force_t`, `torque_t`, `moment_of_inertia_t`
- `voltage_t`, `power_t`, `resistance_t`, `impedance_t`
- `capacitance_t`, `inductance_t`, `flux_t`
- `magnetic_field_strength_t`, `flux_density_t`
- `angle_t`, `damping_coefficient_t`, `inertia_constant_t`

**Per-Unit Types** (suffix `_pu_t`):
- All SI types have corresponding per-unit equivalents
- Examples: `voltage_pu_t`, `current_pu_t`, `power_pu_t`, etc.

This units library provides the foundation for type-safe, efficient engineering calculations throughout the YeeT control platform, particularly valuable for power electronics and motor control applications where unit errors can have significant safety and performance implications.