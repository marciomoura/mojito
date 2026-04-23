# Coordinate Reference Frames

## Overview

The `reference_frame.hpp` header defines three strongly-typed coordinate frame classes (`abc`, `alphabeta`, and `dq`) used throughout the yeet_control library for representing three-phase and two-phase electrical quantities. These classes provide type-safe transformations between different reference frames commonly used in power electronics and motor control applications.

## Core Concepts

### Reference Frame Theory

In power electronics and motor control, coordinate transformations simplify analysis and control:

1. **Three-Phase ABC Frame**: Natural representation of three-phase systems (phase A, B, C)
2. **Two-Phase αβ Frame (Clarke Transform)**: Stationary orthogonal frame, reduces complexity from 3 to 2 axes
3. **Rotating dq Frame (Park Transform)**: Synchronous rotating frame aligned with a reference angle, converts AC quantities to DC for easier control

**Transformation Chain:**
```
abc → alphabeta (Clarke) → dq (Park)
```

### Mathematical Transformations

#### Clarke Transformation (abc → αβ)

Converts three-phase stationary frame to two-phase stationary frame:

```
α = a
β = (a + 2b) / √3
```

For balanced systems where `a + b + c = 0`:
```
α = a
β = (a - c) / √3 = (2a + c) / √3
```

#### Inverse Clarke Transformation (αβ → abc)

```
a = α
b = (-α + √3·β) / 2
c = (-α - √3·β) / 2
```

#### Park Transformation (αβ → dq)

Transforms to a rotating reference frame using angle θ:

```
d = α·cos(θ) + β·sin(θ)
q = -α·sin(θ) + β·cos(θ)
```

#### Inverse Park Transformation (dq → αβ)

```
α = d·cos(θ) - q·sin(θ)
β = d·sin(θ) + q·cos(θ)
```

## Class Reference

### `abc<T>` - Three-Phase Frame

Represents quantities in the natural three-phase stationary frame.

#### Construction

```cpp
#include "yeet_control/reference_frame.hpp"
using namespace yeet;

// Default construction
abc<double> voltages;

// Component-wise construction
abc<double> currents{1.0, -0.5, -0.5};

// From array
std::array<double, 3> values = {1.0, 0.0, -1.0};
abc<double> signals{values};
```

#### Access Methods

```cpp
// Component access
double phase_a = voltages.a();
double phase_b = voltages.b();
double phase_c = voltages.c();

// Array-style access
double phase = voltages[0];  // Same as voltages.a()

// Iteration
for (const auto& phase : voltages) {
    // Process each phase
}
```

#### Transformations

```cpp
// Transform to αβ frame (Clarke)
alphabeta<double> ab = voltages.to_alphabeta();

// Transform to dq frame (requires angle)
angle_wrapped theta = angle_wrapped::from_degrees(30.0);
dq<double> dq_frame = voltages.to_dq(theta);

// Rotate in abc frame
abc<double> rotated = voltages.rotate(theta);
```

#### Arithmetic Operations

```cpp
abc<double> v1{1.0, 0.0, -1.0};
abc<double> v2{0.5, 0.5, 0.0};

// Addition and subtraction
abc<double> sum = v1 + v2;
abc<double> diff = v1 - v2;

// Scalar multiplication and division
abc<double> scaled = v1 * 2.0;
abc<double> divided = v1 / 3.0;

// Negation
abc<double> inverted = -v1;

// Absolute values
abc<double> abs_vals = v1.absolute();

// Compound operations
v1 += v2;
v1 *= 2.0;
```

### `alphabeta<T>` - Two-Phase Stationary Frame

Represents quantities in the two-phase stationary orthogonal frame.

#### Construction

```cpp
// Component-wise construction
alphabeta<double> ab{1.0, 0.866};

// From array
std::array<double, 2> values = {1.0, 0.0};
alphabeta<double> signals{values};
```

#### Access Methods

```cpp
double alpha_component = ab.alpha();
double beta_component = ab.beta();

// Magnitude and phase
double mag = ab.magnitude();  // √(α² + β²)
angle_wrapped phase = ab.phase();  // atan2(β, α)
```

#### Transformations

```cpp
// Transform to abc frame (Inverse Clarke)
abc<double> abc_frame = ab.to_abc();

// Transform to dq frame (Park)
angle_wrapped theta = angle_wrapped::from_radians(0.523);
dq<double> dq_frame = ab.to_dq(theta);
```

#### Rotation

```cpp
// Rotate counter-clockwise
angle_wrapped rotation_angle = angle_wrapped::from_degrees(45.0);
alphabeta<double> rotated_ccw = ab.rotate_counter_clockwise(rotation_angle);

// Rotate clockwise
alphabeta<double> rotated_cw = ab.rotate_clockwise(rotation_angle);
```

### `dq<T>` - Rotating Reference Frame

Represents quantities in the synchronous rotating reference frame.

#### Construction

```cpp
// Component-wise construction
dq<double> dq_vals{1.5, 0.2};

// From array
std::array<double, 2> values = {1.0, 0.0};
dq<double> signals{values};
```

#### Access Methods

```cpp
double d_component = dq_vals.d();  // Direct axis (flux-producing)
double q_component = dq_vals.q();  // Quadrature axis (torque-producing)

// Magnitude in dq frame
double mag = dq_vals.magnitude();  // √(d² + q²)

// Phase relative to frame angle
angle_wrapped frame_angle = angle_wrapped::from_degrees(30.0);
angle_wrapped absolute_phase = dq_vals.phase(frame_angle);
```

#### Transformations

```cpp
// Transform to αβ frame (Inverse Park)
angle_wrapped theta = angle_wrapped::from_degrees(60.0);
alphabeta<double> ab_frame = dq_vals.to_alphabeta(theta);

// Transform to abc frame (Inverse Park + Inverse Clarke)
abc<double> abc_frame = dq_vals.to_abc(theta);
```

#### Rotation

```cpp
// Rotate in dq frame
angle_wrapped rotation = angle_wrapped::from_degrees(15.0);
dq<double> rotated_ccw = dq_vals.rotate_counter_clockwise(rotation);
dq<double> rotated_cw = dq_vals.rotate_clockwise(rotation);
```

## Type Safety and Units

All three coordinate frame classes are templates that work with any numeric type, including the type-safe units from `yeet_control/units.hpp`:

```cpp
#include "yeet_control/units.hpp"
#include "yeet_control/reference_frame.hpp"

using namespace yeet;

// Per-unit voltages
abc<voltage_pu_t> voltages_pu{
    voltage_pu_t{1.0},
    voltage_pu_t{-0.5},
    voltage_pu_t{-0.5}
};

// Per-unit currents
dq<current_pu_t> currents_dq{
    current_pu_t{1.2},
    current_pu_t{0.3}
};

// Mixed-type operations
abc<power_pu_t> power_abc = voltages_pu * abc<current_pu_t>{...};
```

## Usage Examples

### Three-Phase Current Control

```cpp
// Measure three-phase currents
abc<current_pu_t> measured_currents = measure_currents();

// Transform to dq frame for control
angle_wrapped rotor_angle = pll.get_estimated_angle();
dq<current_pu_t> currents_dq = measured_currents.to_dq(rotor_angle);

// PI control in dq frame (DC quantities)
dq<current_pu_t> current_reference{current_pu_t{1.0}, current_pu_t{0.0}};
dq<current_pu_t> error = current_reference - currents_dq;

dq<voltage_pu_t> voltage_command_dq;
voltage_command_dq.d() = d_controller.update(error.d());
voltage_command_dq.q() = q_controller.update(error.q());

// Transform back to abc for PWM generation
abc<voltage_pu_t> voltage_command_abc = voltage_command_dq.to_abc(rotor_angle);
```

### Unbalance Detection

```cpp
// Measure three-phase voltages
abc<voltage_pu_t> grid_voltages = measure_grid();

// Check for balance
alphabeta<voltage_pu_t> ab = grid_voltages.to_alphabeta();
voltage_pu_t magnitude = ab.magnitude();
angle_wrapped phase = ab.phase();

// Magnitude should be constant for balanced system
// Phase should rotate uniformly
```

### Power Calculation

```cpp
// Three-phase instantaneous power
abc<voltage_pu_t> v_abc = measure_voltages();
abc<current_pu_t> i_abc = measure_currents();

// Convert to αβ frame
alphabeta<voltage_pu_t> v_ab = v_abc.to_alphabeta();
alphabeta<current_pu_t> i_ab = i_abc.to_alphabeta();

// Instantaneous power
power_pu_t p_inst = v_ab.alpha() * i_ab.alpha() + v_ab.beta() * i_ab.beta();
```

## Advanced Features

### Mixed-Type Arithmetic

The coordinate frames support mixed-type operations that preserve dimensional correctness:

```cpp
// Voltage * Current = Power (unit types enforce this)
abc<voltage_pu_t> voltages{...};
abc<current_pu_t> currents{...};
auto power = voltages * currents;  // Results in abc<decltype(V*I)>

// Scalar multiplication with different types
abc<double> scaled = voltages * 2.0;  // Type converts appropriately
```

### Conversion Between Types

```cpp
// Convert between different underlying types
abc<float> float_vals{1.0f, 2.0f, 3.0f};
abc<double> double_vals{float_vals};  // Automatic type conversion

// Extract raw values from unit types
abc<voltage_pu_t> pu_voltages{...};
auto raw_frame = pu_voltages.to_raw_frame();  // Returns abc<double>
```

## Performance Considerations

- All transformation methods are `constexpr` where possible, enabling compile-time evaluation
- Frame objects use fixed-size `std::array` internally (no dynamic allocation)
- Arithmetic operations are inlined for zero-overhead abstraction
- Suitable for hard real-time applications with deterministic execution

## Integration with Control Algorithms

The coordinate frames are designed to work seamlessly with yeet_control components:

- **PI Controllers**: `pi_controller<dq<voltage_pu_t>>` for vector control
- **Phase-Locked Loops**: Accept `abc<voltage_pu_t>`, internally use αβ and dq
- **Pulse Generators**: Output in abc frame for direct hardware interface
- **Signal Processing**: Filters can operate on any frame type

## See Also

- [angle_wrapped.md](angle_wrapped.md) - Type-safe angle representation
- [units.md](units.md) - Dimensional analysis and unit types
- [three_phase_pll.md](three_phase_pll.md) - Grid synchronization using dq frame
- [pi_controller.md](pi_controller.md) - Control in coordinate frames
