# 🍸 Mojito

A modular, high-performance C++ library for electrical engineering coordinate transforms and physical quantities. Mojito provides a type-safe, unit-aware framework for working with `abc`, `αβ`, and `dq` frames, supporting both classic header-only integration and modern C++20 modules.

## ✨ Features

- **Type-Safe Quantities:** Prevents accidental mixing of different units (e.g., adding Voltage to Current).
- **Coordinate Transforms:** Robust implementations of Clarke, Park, and Line-to-Line transforms.
- **Per-Unit System:** Seamless conversion between SI units and Per-Unit values.
- **Hybrid Integration:** Use it as a header-only library or as a compiled C++20 module.
- **Multi-Build Support:** First-class support for both CMake and Bazel.

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
