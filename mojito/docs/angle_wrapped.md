# Angle Wrapped

## Overview

The `angle_wrapped` class provides a high-performance, type-safe representation of angles with automatic range wrapping to [0, 2π]. It leverages the natural overflow behavior of `uint32_t` arithmetic to achieve zero-cost wrapping, making it ideal for real-time control applications.

## Key Features

- **Zero-Cost Wrapping**: Uses `uint32_t` overflow for automatic range wrapping.
- **High Resolution**: ~1.46e-9 radians per LSB.
- **Exact Binary Angles**: Values like π/2, π, 3π/2 are represented exactly.
- **Branchless**: Wrapping logic requires no conditional statements.

## Construction and Access

```cpp
#include "mojito/angle_wrapped.hpp"
using namespace mojito;

// Construction
angle_wrapped theta1{1.57};  // From radians
angle_wrapped theta2 = angle_wrapped::from_degrees(90.0);
angle_wrapped theta3;        // Defaults to 0

// Accessing Values
angle_t rad = theta1.get_radians();
real_t deg = theta1.get_degrees();
angle_pu_t pu = theta1.get_pu();  // Normalized [0, 1]
```

## Arithmetic Operations

`angle_wrapped` supports standard arithmetic operations with automatic wrapping:

```cpp
angle_wrapped a = angle_wrapped::from_degrees(350.0);
angle_wrapped b = angle_wrapped::from_degrees(30.0);

angle_wrapped sum = a + b;   // 380° -> wraps to 20°
angle_wrapped diff = b - a;  // -320° -> wraps to 40°

// Scalar operations
angle_wrapped scaled = a * 2.0;
```

## Comparison Operations

Comparisons are performed on the underlying `uint32_t` representation.

```cpp
bool is_less = (theta1 < theta2);
bool is_equal = (theta1 == theta2);
```

## Practical Example: Phase Accumulation

```cpp
class SignalGenerator {
public:
    SignalGenerator(real_t freq, real_t dt)
        : _phase{0.0}, _increment{2.0 * pi * freq * dt} {}

    real_t next() {
        real_t val = std::sin(_phase.get_radians());
        _phase += _increment;  // Automatic wrapping at 2π
        return val;
    }

private:
    angle_wrapped _phase;
    angle_wrapped _increment;
};
```

## Design Rationale

- **Efficiency**: Integer overflow is well-defined and handled at the hardware level, avoiding expensive `fmod` or branching.
- **Precision**: 32-bit resolution is more than sufficient for most control applications and avoids accumulated floating-point errors from repeated wrapping.

## See Also

- [reference_frame.md](reference_frame.md) - Coordinate transformations using angles
- [units.hpp](../include/mojito/units.hpp) - Type-safe dimensional quantities
