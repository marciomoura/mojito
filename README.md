# Mojito

A collection of useful classes and functions to write real-time control software for power-electronic applications.

To control three-phase rectifiers or inverters, the `abc`, `alphabeta` and `dq` types are fundamental.

To write a more readable and safe software, the definition of strong-types is highly recommended.

Expressing types as `pu`, `si` or in `percent` avoids many issues when assignment of e.g. `pu` to a `si` happens. 

The integration of `angles` which are used in `pll` algorithms usually needs a `2\pi` wrapping, otherwise it will integrate to infinity.

For real-time code, constants or run-time parameters should pre-compute its reciprocal. Divisions are much more expensive than multiplication.

Well... these are my reasons to create this small project. 

## Compiling the Project

This project uses **CMake Presets** to simplify the build process across different environments.

### Prerequisites

- **CMake** (version 3.28 or higher required for C++20 module support)
- **Ninja** (recommended generator)
- **GCC** (version 15.1+ recommended) or **Clang**

### 1. Configuration

To configure the project using the default GCC preset, run the following from the root directory:

```powershell
cmake --preset host-gcc
```

This will create the `build-host` directory and prepare the build system.

### 2. Building

You can build all targets or specific components using the following build presets:

*   **Build everything** (Library, Module, Tests, and Examples):
    ```powershell
    cmake --build --preset host-gcc-debug
    ```

*   **Build specific targets**:
    ```powershell
    # Build only the C++20 Module library
    cmake --build --preset host-gcc-debug --target mojito_module

    # Build only the module usage example
    cmake --build --preset host-gcc-debug --target example_module_usage
    ```

### 3. Running Tests and Examples

After building, binaries are located in the `build-host/bin/` directory.

*   **Run the C++20 Module Example**:
    ```powershell
    ./build-host/bin/example_module_usage.exe
    ```

*   **Run All Unit Tests**:
    ```powershell
    ctest --preset host-gcc-test
    ```

## Compiling with Bazel

The project also supports the **Bazel** build system (version 7.4.1+). We recommend using [Bazelisk](https://github.com/bazelbuild/bazelisk) to automatically manage the correct Bazel version.

### 1. Building the Library
To build the core `mojito` library:
```powershell
bazelisk build //mojito:mojito
```

### 2. Running Tests
To build and run the entire test suite:
```powershell
bazelisk test //mojito/tests:mojito_tests
```

### 3. Build Everything
To build all targets in the workspace:
```powershell
bazelisk build //...
```

### 4. Cross-Compilation
The project is configured for hermetic cross-compilation using the LLVM toolchain. You can target different architectures using the following configurations:

*   **ARM64 Linux**:
    ```powershell
    bazelisk build --config=arm64 //...
    ```
*   **Cortex-M4 (Bare Metal)**:
    ```powershell
    bazelisk build --config=m4 //...
    ```
    *Note: This configuration ensures the library remains "freestanding" and suitable for microcontrollers.*

## Usage Modes

### Header-Only (Interface)
Link your target against `mojito::mojito` and include the headers:
```cpp
#include <mojito/mojito.hpp>
```

### C++20 Module
Link your target against `mojito::module` and import the module:
```cpp
import mojito;
```
