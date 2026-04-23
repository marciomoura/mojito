# Coordinate Reference Frames

## Overview

The `mojito` library provides three strongly-typed coordinate frame classes (`abc`, `alphabeta`, and `dq`) for representing three-phase and two-phase electrical quantities. These classes enable type-safe transformations commonly used in power electronics and motor control.

## Core Concepts

- **Three-Phase ABC Frame**: Natural representation of phases A, B, and C.
- **Two-Phase αβ Frame (Clarke)**: Stationary orthogonal frame.
- **Rotating dq Frame (Park)**: Synchronous rotating frame aligned with a reference angle θ.

**Transformation Chain:** `abc` ↔ `alphabeta` ↔ `dq`

## Mathematical Transformations

### Clarke (abc → αβ)
```
α = a
β = (a + 2b) / √3
```

### Park (αβ → dq)
```
d = α·cos(θ) + β·sin(θ)
q = -α·sin(θ) + β·cos(θ)
```

## Class Reference

All frames are templates supporting any numeric type or `mojito` unit types.

### `abc<T>`
```cpp
#include "mojito/reference_frame.hpp"
using namespace mojito;

abc<double> v_abc{1.0, -0.5, -0.5};
alphabeta<double> v_ab = v_abc.to_alphabeta();
```

### `alphabeta<T>`
```cpp
alphabeta<double> v_ab{1.0, 0.0};
dq<double> v_dq = v_ab.to_dq(theta);
double mag = v_ab.magnitude();
```

### `dq<T>`
```cpp
dq<double> v_dq{1.0, 0.0};
abc<double> v_abc = v_dq.to_abc(theta);
```

## Arithmetic and Type Safety

Frames support standard arithmetic and preserve unit types:

```cpp
abc<voltage_pu_t> v{...};
abc<current_pu_t> i{...};
auto p = v * i; // Component-wise multiplication
```

## Performance

- `constexpr` transformations where possible.
- No dynamic allocation (uses `std::array`).
- Inlined operations for zero-overhead.

## See Also

- [angle_wrapped.md](angle_wrapped.md) - Type-safe angle representation
- [units.hpp](../include/mojito/units.hpp) - Dimensional analysis and unit types
