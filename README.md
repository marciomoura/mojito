# 🍸 Mojito

A modular, high-performance C++ library for electrical engineering coordinate transforms and physical quantities. Mojito provides a type-safe, unit-aware framework for working with `abc`, `αβ`, and `dq` frames, supporting both classic header-only integration and modern C++20 modules.

## ✨ Features

- **Type-Safe Quantities:** Prevents accidental mixing of different units (e.g., adding Voltage to Current).
- **Coordinate Transforms:** Robust implementations of Clarke, Park, and Line-to-Line transforms.
- **Per-Unit System:** Seamless conversion between SI units and Per-Unit values.
- **Custom Bases:** Support for multiple independent per-unit systems (e.g., Grid vs. Machine).
- **Hybrid Integration:** Use it as a header-only library or as a compiled C++20 module.
- **Multi-Build Support:** First-class support for both CMake and Bazel.

---

## 🛡️ Strong Types & Coordinate Frames

Mojito uses a robust dimensional analysis system to ensure physical correctness at compile-time.

### 1. Defining Quantities
You can represent physical values in SI, Per-Unit (PU), or Percent systems.

```cpp
voltage_t v_si{230.0};             // SI units (Volts)
voltage_pu_t v_pu{1.0};            // Default Per-Unit (0.0 to 1.0)
voltage_percent_t v_pct{100.0};    // Percent system (0.0 to 100.0)

// Custom Per-Unit Bases (prevents mixing independent PU systems)
struct machine_base {};
voltage_custom_pu_t<machine_base> v_mach{1.1}; 
```

### 2. Using Frames with Units
Coordinate frames integrate seamlessly with unit types:

```cpp
// A three-phase voltage frame in SI
abc<voltage_t> v_abc{v_si, v_si, v_si};

// A rotating current frame in Per-Unit
dq<current_pu_t> i_dq{current_pu_t{0.8}, current_pu_t{0.1}};

// Mixed-unit frames are caught at compile-time
// abc<voltage_t> bad{v_si, current_t{10.0}, v_si}; // COMPILE ERROR
```

### 3. Arithmetic Operations
Operations respect physics. Return types are automatically and correctly deduced.

```cpp
voltage_t v{120.0};
resistance_t r{60.0};

auto i = v / r;          // Result is current_t (Ohm's Law)
auto p = v * i;          // Result is power_t
auto v_rms = sqrt(p * r); // Result is voltage_t

v += voltage_t{10.0};    // Compound assignment works as expected
```

### 4. Compile-Time Protection
The library catches common engineering mistakes before your code ever runs:

```cpp
voltage_t v{100.0};
current_t i{10.0};

v = i;                // ERROR: Dimension mismatch
auto x = v + i;       // ERROR: Cannot add Volts and Amps
auto y = v + v_pu;    // ERROR: System mismatch (SI vs PU)

struct grid_base {};
voltage_custom_pu_t<grid_base> v_grid{1.0};
v_mach = v_grid;      // ERROR: Custom base mismatch (Machine != Grid)
```

### 5. Type-Safe APIs
Avoid "Float-Hell" and argument swapping bugs by using explicit types.

```cpp
// ❌ Dangerous API: Easy to swap id/iq or pass raw floats
void update_ctrl(float id, float iq, float v_dc); 

// ✅ Mojito API: Self-documenting and impossible to swap
void update_ctrl(current_pu_t id, current_pu_t iq, voltage_t v_dc);

update_ctrl(1.0f, 0.5f, 24.0f); // ERROR: Requires explicit types
```

---

## 🚀 How to use in your CMake project

The easiest way to integrate Mojito into your own project is using `FetchContent`.

### 1. Add to your `CMakeLists.txt`

```cmake
include(FetchContent)

FetchContent_Declare(
    mojito
    GIT_REPOSITORY https://github.com/marciomoura/mojito.git
    GIT_TAG        main # or a specific commit/tag
)

FetchContent_MakeAvailable(mojito)

# Link against the library
add_executable(my_app main.cpp)
target_link_libraries(my_app PRIVATE mojito::mojito)
```

### 2. Basic Usage Example

```cpp
#include <mojito/mojito.hpp>
#include <iostream>

using namespace mojito;

int main() {
    // Define a 100V peak, 0 degree phase voltage
    auto voltage_abc = make_abc(voltage_t{100.0}, angle_wrapped{0.0});

    // Convert to αβ frame (Clarke Transform)
    auto voltage_ab = to_alphabeta(voltage_abc);

    std::cout << "Alpha: " << voltage_ab.alpha().value() << " V\n";
    std::cout << "Beta:  " << voltage_ab.beta().value() << " V\n";

    return 0;
}
```

---

## 🛠️ Development & Compiling

### CMake Build

This project uses **CMake Presets** to simplify the build process.

- **Prerequisites:** CMake 3.28+, Ninja, GCC 15.1+ or Clang 16+.

```powershell
# 1. Configure
cmake --preset host-gcc

# 2. Build everything
cmake --build --preset host-gcc-debug

# 3. Run Tests
ctest --preset host-gcc-test
```

### Bazel Build

Mojito supports Bazel (version 7.4.1+).

```powershell
# Build the library
bazelisk build //mojito:mojito

# Run the test suite
bazelisk test //mojito/tests:mojito_tests
```

---

## 📦 Usage Modes

### Header-Only (Interface)
Link your target against `mojito::mojito` and include:
```cpp
#include <mojito/mojito.hpp>
```

### C++20 Module
Link your target against `mojito::module` and import:
```cpp
import mojito;
```

---

## 📜 License

This project is licensed under the MIT License.
