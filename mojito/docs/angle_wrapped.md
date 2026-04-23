# Angle Wrapped

## Overview

The `angle_wrapped` class provides a high-performance, type-safe representation of angles with automatic range wrapping to [0, 2π]. It uses the natural overflow behavior of uint32_t arithmetic to achieve zero-cost wrapping, making it ideal for real-time embedded control applications where angles are continuously integrated or accumulated.

## Key Features

- **Zero-Cost Wrapping**: Leverages uint32_t overflow for automatic range wrapping without modulo operations
- **Perfect Determinism**: All operations have O(1) constant time complexity
- **High Resolution**: ~1.46e-9 radians (~8.38e-8 degrees) per LSB (Least Significant Bit)
- **Exact Binary Angles**: Values like π/2, π, 3π/2 can be represented exactly
- **No Branching**: Wrapping logic requires no conditional statements

## Mathematical Principle

The class maps the full range of a 32-bit unsigned integer (0 to 2³²-1) directly to the angular range [0, 2π):

```
angle_in_radians = (uint32_value / 2³²) × 2π
```

This bijective mapping ensures that:
- Addition/subtraction of uint32 values corresponds to angle addition/subtraction
- Overflow/underflow automatically wraps angles to the correct range
- No explicit wrapping logic is needed

## Construction

### From Radians

```cpp
#include "yeet_control/angle_wrapped.hpp"
using namespace yeet;

// Direct construction from radians
angle_wrapped theta1{1.57};  // π/2 radians

// Static factory method
angle_wrapped theta2 = angle_wrapped::from_radians(angle_t{3.14159});
```

### From Degrees

```cpp
// Convert degrees to angle
angle_wrapped theta_90 = angle_wrapped::from_degrees(90.0);
angle_wrapped theta_45 = angle_wrapped::from_degrees(45.0);
angle_wrapped theta_360 = angle_wrapped::from_degrees(360.0);  // Wraps to 0
```

### Default Construction

```cpp
// Default constructs to 0 radians
angle_wrapped theta;  // 0 radians
```

## Accessing Values

### Get as Radians

```cpp
angle_wrapped theta{1.0};
angle_t radians = theta.get_radians();  // Returns angle_t (dimensioned type)
double rad_value = radians.value();     // Extract numerical value

// Or use implicit conversion
angle_t rad = theta;
```

### Get as Degrees

```cpp
angle_wrapped theta{3.14159};
real_t degrees = theta.get_degrees();  // Returns ~180.0
```

### Get as Per-Unit

The angle can be retrieved as a normalized value in [0, 1] where 1 corresponds to 2π:

```cpp
angle_wrapped theta = angle_wrapped::from_degrees(180.0);
angle_pu_t pu = theta.get_pu();  // Returns 0.5 (180° = π = 0.5 × 2π)
```

### Explicit Conversions

```cpp
// Explicit cast to real_t (double/float)
angle_wrapped theta{1.57};
real_t value = static_cast<real_t>(theta);  // ~1.57

// Function call operator
angle_t rad = theta();
```

## Arithmetic Operations

### Angle Addition and Subtraction

```cpp
angle_wrapped theta1 = angle_wrapped::from_degrees(30.0);
angle_wrapped theta2 = angle_wrapped::from_degrees(60.0);

// Addition (automatically wraps)
angle_wrapped sum = theta1 + theta2;  // 90°

// Subtraction (automatically wraps)
angle_wrapped diff = theta1 - theta2;  // -30° wraps to 330°

// Compound assignment
theta1 += theta2;
theta1 -= angle_wrapped::from_degrees(15.0);

// Negation
angle_wrapped neg = -theta1;
```

### Scalar Operations

Multiply or divide angles by scalars for frequency integration or scaling:

```cpp
angle_wrapped theta{1.0};
real_t scalar = 2.0;

// Scalar multiplication
angle_wrapped doubled = theta * scalar;        // 2.0 radians
angle_wrapped also_doubled = scalar * theta;   // Commutative

// Scalar division
angle_wrapped halved = theta / 2.0;  // 0.5 radians

// Compound scalar operations
theta *= 3.0;
theta /= 2.0;
```

### Wraparound Examples

```cpp
// Addition with wraparound
angle_wrapped a1 = angle_wrapped::from_degrees(350.0);  // 350°
angle_wrapped a2 = angle_wrapped::from_degrees(30.0);   // 30°
angle_wrapped sum = a1 + a2;  // 380° → wraps to 20°

// Subtraction with wraparound
angle_wrapped b1 = angle_wrapped::from_degrees(10.0);   // 10°
angle_wrapped b2 = angle_wrapped::from_degrees(50.0);   // 50°
angle_wrapped diff = b1 - b2;  // -40° → wraps to 320°

// Multiple rotations
angle_wrapped theta = angle_wrapped::from_degrees(45.0);
for (int i = 0; i < 10; ++i) {
    theta += angle_wrapped::from_degrees(36.0);  // Each += 36°
}
// Result: 45° + 10×36° = 45° + 360° = 45° (full rotation wraps to original)
```

## Comparison Operations

```cpp
angle_wrapped theta1{1.0};
angle_wrapped theta2{2.0};

// Equality
bool equal = (theta1 == theta2);  // false

// Inequality
bool not_equal = (theta1 != theta2);  // true

// Relational comparisons (based on uint32 representation)
bool less = (theta1 < theta2);      // true (1.0 rad < 2.0 rad)
bool greater = (theta1 > theta2);   // false
bool less_eq = (theta1 <= theta2);  // true
bool greater_eq = (theta1 >= theta2);  // false
```

**Note on Comparisons**: Since angles wrap, comparisons are performed on the underlying uint32 representation. An angle of 0.1 radians is "less than" 6.0 radians, but both represent valid angles in [0, 2π].

## Trigonometric Functions

Compute sine and cosine directly from the angle object:

```cpp
angle_wrapped theta = angle_wrapped::from_degrees(45.0);

// Sine and cosine
real_t sine = std::sin(theta.get_radians());
real_t cosine = std::cos(theta.get_radians());

// Or using the implicit conversion
real_t tangent = std::tan(static_cast<real_t>(theta));
```

## Practical Usage Examples

### Phase Accumulation for Signal Generation

```cpp
class SignalGenerator {
public:
    SignalGenerator(real_t frequency, real_t sampling_period)
        : _phase{0.0}, _phase_increment{2.0 * pi * frequency * sampling_period}
    {
    }

    real_t generate_sine() {
        real_t output = std::sin(_phase.get_radians());
        _phase += _phase_increment;  // Automatic wrapping
        return output;
    }

private:
    angle_wrapped _phase;
    angle_wrapped _phase_increment;
};

// Usage
SignalGenerator gen(50.0, 100e-6);  // 50 Hz, 100 µs sample time
for (int i = 0; i < 1000; ++i) {
    real_t sine_value = gen.generate_sine();
    // Phase automatically wraps every 0.02 seconds (50 Hz period)
}
```

### Rotor Position Tracking in Motor Control

```cpp
// Integrate angular velocity to get position
angle_wrapped rotor_position{0.0};
real_t angular_velocity = 100.0;  // rad/s
real_t dt = 100e-6;  // 100 µs sample time

for (int step = 0; step < 10000; ++step) {
    // Integrate velocity (automatically wraps at 2π)
    rotor_position += angle_wrapped{angular_velocity * dt};
    
    // Use position for field-oriented control
    dq<current_pu_t> currents_dq = measured_currents.to_dq(rotor_position);
    
    // ... control logic ...
}
```

### Phase-Locked Loop Angle Estimation

```cpp
class SimplePLL {
public:
    void update(real_t phase_error) {
        // PI controller on phase error
        real_t frequency_estimate = _nominal_freq + _kp * phase_error + _ki * _integral;
        _integral += phase_error * _dt;
        
        // Integrate frequency to get angle (automatic wrapping)
        _estimated_angle += angle_wrapped{frequency_estimate * _dt};
    }
    
    angle_wrapped get_angle() const { return _estimated_angle; }

private:
    angle_wrapped _estimated_angle{0.0};
    real_t _nominal_freq{2.0 * pi * 50.0};  // 50 Hz
    real_t _kp{100.0}, _ki{10000.0};
    real_t _integral{0.0};
    real_t _dt{100e-6};
};
```

### Firing Angle Control for Thyristors

```cpp
// Calculate firing angle for thyristor bridge
angle_wrapped grid_phase = pll.get_estimated_angle();
angle_wrapped firing_delay = angle_wrapped::from_degrees(30.0);  // α = 30°

// Firing angles for 6-pulse bridge (60° apart)
std::array<angle_wrapped, 6> firing_angles;
for (size_t i = 0; i < 6; ++i) {
    firing_angles[i] = grid_phase + firing_delay + 
                       angle_wrapped::from_degrees(i * 60.0);
}
```

## Performance Characteristics

### Computational Efficiency

- **Addition/Subtraction**: Single uint32 add/subtract (1-2 CPU cycles)
- **Multiplication**: One float multiply + conversion (~10 cycles)
- **Division**: One float divide + conversion (~20 cycles)
- **Wrapping**: Zero additional cost (handled by hardware overflow)
- **Comparisons**: Single uint32 comparison (1 cycle)

### Memory Footprint

- Storage: 4 bytes (uint32_t)
- No dynamic allocation
- Suitable for stack allocation in real-time code

### Numerical Precision

- Resolution: 2π / 2³² ≈ 1.46 × 10⁻⁹ radians
- At 50 Hz integration: Accumulates full rotation in exactly 2³² samples
- No accumulated floating-point error from repeated wrapping

## Integration with Other Components

### With Coordinate Frames

```cpp
#include "yeet_control/reference_frame.hpp"

// Park and Clarke transformations use angle_wrapped
angle_wrapped theta = angle_wrapped::from_degrees(30.0);
abc<voltage_pu_t> voltages{...};

alphabeta<voltage_pu_t> ab = voltages.to_alphabeta();
dq<voltage_pu_t> dq_frame = ab.to_dq(theta);
```

### With Phase-Locked Loops

```cpp
#include "yeet_control/three_phase_pll.hpp"

three_phase_pll<double> pll(100e-6);  // 100 µs sampling
pll.update(measured_voltages);

angle_wrapped grid_angle = pll.get_estimated_angle();  // Returns angle_wrapped
real_t grid_freq = pll.get_estimated_frequency();
```

### With Pulse Generators

```cpp
#include "yeet_control/six_pulse_generator.hpp"

six_pulse_generator generator(100e-6);
generator.configure_firing_angle(angle_wrapped::from_degrees(45.0));
generator.update(grid_angle);
```

## Design Rationale

### Why uint32_t?

1. **Natural Wrapping**: Integer overflow is well-defined in C++ for unsigned types
2. **Portability**: uint32_t is available on all platforms (8-bit to 64-bit)
3. **Efficiency**: Hardware-level addition without conditional branches
4. **Resolution**: 32 bits provides more than sufficient precision for control applications

### Comparison with Modulo-Based Wrapping

Traditional approach:
```cpp
// Requires branching and/or expensive modulo operation
angle = fmod(angle + delta, 2.0 * pi);
if (angle < 0) angle += 2.0 * pi;
```

`angle_wrapped` approach:
```cpp
// Zero-cost, branchless wrapping via hardware
angle = angle + delta;  // uint32 overflow handles wrapping
```

## Best Practices

1. **Use for Accumulated Angles**: Ideal for integrating angular velocities or accumulating phase
2. **Avoid for Small Angle Differences**: For angle differences expected to be small, consider using raw floats to preserve sign information
3. **Convert at Boundaries**: Convert to/from angle_wrapped at algorithm boundaries to maintain type safety
4. **Leverage Implicit Conversions**: The class provides implicit conversion to angle_t for use with standard library functions

## Limitations

- **Range Fixed to [0, 2π]**: Cannot represent angles outside this range (but this is usually desired for control)
- **Comparison Semantics**: Comparison operators compare uint32 representation, not "angular distance"
- **Precision at Extremes**: Very small angles near 0 or 2π have identical representation

## See Also

- [reference_frame.md](reference_frame.md) - Coordinate transformations using angles
- [three_phase_pll.md](three_phase_pll.md) - Phase tracking producing angle_wrapped
- [units.md](units.md) - Type-safe dimensional quantities including angle_t
