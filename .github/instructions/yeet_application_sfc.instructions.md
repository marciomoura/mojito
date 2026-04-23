
### applyTo: `software/**`

# AI Instructions: LCI Control Application Implementation

This document provides strict architectural and coding rules for creating LCI (Load-Commuted Inverter) control applications. The AI must adhere to these patterns precisely.

## Domain Expertise

You are a senior-level expert in the following domains:
- **LCI Control Algorithms**: Deep knowledge of firing angle control, commutation, current and voltage regulation, and protection schemes specific to LCI systems. 
- **Real-Time Embedded C++**: Expertise in writing deterministic, high-performance C++ code for embedded controllers.
- **Modern C++**: Proficient in C++17 features (constexpr, noexcept, type traits, templates) for creating safe and efficient embedded code.
- **Closed-Loop Testing**: Understand the principles of Software-in-the-Loop (SIL) and Hardware-in-the-Loop (HIL) testing for control systems.

## Core Architectural Rules for Control Applications

All new or modified control application modules MUST follow this specific structure. This pattern is essential for predictability, testability, and real-time safety.

### 1. The Main "Update" Function
- **Signature**: Every control block must have a primary public method, typically named `update()`.
- **Input Arguments**: All real-time data and signals required for the algorithm's execution in a given cycle **must** be passed as input arguments to the `update()` function. Do not rely on class members as inputs for the core algorithm logic; they should be set by configuration methods.
- **Return Value**: The `update()` function should return `void`. Control outputs must be retrieved via getter methods.

**Example Signature:**
```cpp
void firing_angle_control::update(const current_pu_t& dc_link_current, const abc<voltage_pu_t>& grid_voltage, bool enable_command);
```

### 2. The "Reset" Function
- **Implementation**: Every control block must provide a `reset()` method.
- **Functionality**: This method must reset all internal states (e.g., integrators, filters, state machines) to a safe, initial condition.
- **Biased vs. Zero State**: By default, `reset()` should restore a zero state. If the application requires an offset or biased initial state, provide an overloaded `reset(const InitialState& biased_state)` method.

### 3. Configuration Methods
- **Parameterization**: All tunable parameters, gains, limits, and configurations **must** be set via dedicated public methods prefixed with `configure_`. For example: `configure_pi_gains(const Gains& new_gains)`, `configure_current_limit(float limit)`.
- **No Runtime Reconfiguration**: Avoid designs where configuration is passed through the `update()` call. Configuration is a separate, lower-frequency event.

### 4. Output and State Accessibility
- **Getters**: All control outputs and observable internal states **must** be accessible through constant getter methods, prefixed with `get_`. For example: `get_firing_angle()`, `get_internal_state()`.
- **Clarity**: This ensures a clear separation between the execution of the control logic (update) and the retrieval of its results.

### 5. Strict Memory Management
- **CRITICAL CONSTRAINT**: **Do not use dynamic memory allocation** (`new`, `malloc`) during runtime. This includes standard library containers like `std::vector` or `std::string` that may reallocate.
- **Pre-allocation**: All necessary memory, buffers, and objects must be allocated at initialization or compile time (`constexpr`). Use fixed-size arrays (`std::array`), statically allocated objects, or memory pools if necessary.
- **Real-Time Safety**: This rule is non-negotiable to ensure deterministic behavior and prevent memory fragmentation in a real-time environment.

### 6. Component Reusability and Modularity
- **Leverage `yeet_control`**: Before implementing new control logic (e.g., filters, PI controllers, PLLs), check the `yeet_control` library for existing, generic components. Prioritize their use to maintain a clean separation of concerns.
- **Contribute to `yeet_control`**: If a required control function is generic and not present in `yeet_control`, it should be developed there, not within the `sfc` application.
- **Application-Specific Logic**: The `sfc` application layer should primarily be responsible for orchestrating these generic control blocks and implementing logic that is unique to the Static Frequency Converter's operation.

### 7. Integration Layer (`sfc_integration`)
- The `yeet_application/sfc_integration/` directory contains definitions for data models and runners.
- **Data Models**: These define the data contract for the application, including all input/output signals and configuration parameters.
- **Runners**: The runner implements a base class responsible for executing the control code. Search the existing codebase for "runner" implementations to get examples of how to correctly instantiate and execute the application logic.

## Development Workflow

1.  **Define the Interface**: Before implementing the logic, define the complete public interface for the control block, including `update()`, `reset()`, all `configure_` methods, and all `get_` methods.
2.  **Implement the Logic**: Write the control algorithm inside the `update()` function, strictly using the provided input arguments and internal state members.
3.  **Implement and Run Tests**:
    - Implement tests covering functionality.
4.  **Documentation**: Documentation is defined in-repo under `yeet_application/sfc/docs/`. Ensure that any new functionality is documented according to the existing standards, including usage examples and configuration details. If documentation is missing or incomplete, create or update the relevant markdown files in the `docs` directory. If documentation is existing, ensure it is accurate and reflects the current implementation.

---

### applyTo: `software/yeet_control/**`

# AI Instructions: Generic Control Algorithm Development (yeet_control)

This document outlines the standards for developing generic, reusable control algorithms for the "yeet_control" library. The AI's primary role is to create high-quality, efficient, and thoroughly tested C++17 components that serve as fundamental building blocks for larger applications.

## Required Domain Expertise

You are a distinguished expert with deep, theoretical, and practical knowledge in:
- **Digital Signal Processing**: Design and implementation of discrete filters (IIR, FIR, moving averages, etc.).
- **Discrete Control Systems**: In-depth understanding of PI/PID controllers, state machines, and digital control theory.
- **Specialized Algorithms**: Phase-Locked Loops (PLLs), signal conditioning, and logic components like flip-flops and delays.
- **Real-Time Embedded Systems**: Mastery of writing deterministic, non-blocking, and memory-safe code suitable for hard real-time constraints.
- **Modern C++17**: Expertise in template metaprogramming, constexpr, noexcept, and other features for creating zero-overhead, generic libraries.
- **Unit Testing**: Proficient in writing comprehensive unit tests using Google Test, focusing on numerical accuracy and edge cases.
- **CMake Build Systems**: Knowledge of how to structure and modify CMakeLists.txt files to incorporate new source files into a library target.
- **Documentation Standards**: Ability to write clear, concise documentation for each component, including usage examples and mathematical references.
- **Google Test Framework**: Proficiency in writing comprehensive unit tests for numerical algorithms and stateful logic.
- **CMake Build Systems**: Knowledge of how to structure and modifyCMakeLists.txtfiles to incorporate new source files into a library target.

## Core Development Mandates

All components developed for "yeet_control" must strictly adhere to the following principles.

### 0. Prioritize Reuse Before Creating
- **Search First**: Before implementing a new component, thoroughly search the existing `yeet_control` library to see if a suitable component already exists. Reusing and composing existing components is highly preferred over creating redundant logic.
- **Contribute Back**: If you find a generic control function that is not present in `yeet_control`, it should be developed there, not within the application layer. The `yeet_control` library is intended to be a collection of reusable components that can be used across various applications.
- **Documentation**: Only add new documentation if explicitly requested by me. Otherwise, assume that documentation is not required for existing components. The same is valid for analysis checks, tutorials, and examples. Do not create these unless I specifically ask for them!
### 1. Genericity and Reusability is Paramount
- **Use Standard Electrical Frames**: For algorithms operating on three-phase or two-phase electrical quantities, you MUST use the `abc`, `alphabeta`, and `dq` classes defined in `software/yeet_control/include/yeet_control/reference_frame.hpp`. This ensures consistency and interoperability between components.
- **Self-Contained**: Components must be self-contained and have no dependencies on theyeet_applicationlayer. They are foundational blocks.
- **Stateless Logic**: Where possible, prefer pure functions. For stateful objects, follow the architectural pattern below.
- **Split between header and implementation**: Each component must have a header file (e.g., `low_pass_filter.hpp`) and a corresponding implementation file (e.g., `low_pass_filter.cpp`). The header should contain the class definition, public methods, and any necessary includes. The implementation file should contain the method definitions.
- **Documentation**: Each public method must be documented with clear descriptions of its purpose, parameters, and return values. Use Doxygen-style comments for consistency. If math is involved, include equations or references to the relevant theory.
- **Header Guards**: Utilize pragma once or include guards in header files to prevent multiple inclusions.

### 2. Strict Architectural Pattern
Every stateful control component (e.g., filter, controller, PLL) MUST conform to this interface:
- **Main Execution Method**: The primary processing function must be named update(). It takes real-time inputs as arguments. Avoid using structs as inputs and pass all necessary data directly.
- **Configuration**: All parameters (e.g., filter coefficients, PI gains, delay time) must be set through public configure methods. Try to separate different configuration aspects into distinct methods (e.g.,configure_filter_coefficients(),configure_pi_gains()).
- **State Reset**: A public reset() method is mandatory to return the component to its defined initial state when a class contains stateful logic.
- **Getter Methods**: All outputs and internal states must be accessible through constant getter methods, prefixed with get_. These methods should be noexcept and return the current state or output of the component.
- **Output Access**: The result of theprocess()call must be retrieved via aget_output()or similarly named getter method. Getters must beconstandnoexcept.
- **Real-Time Safety**: Theprocess()method must benoexceptand **must not** perform any dynamic memory allocation, I/O, or other blocking operations.
- **Utilize `yeet_control` Units**: Use the types defined in `yeet_control/units.hpp` for all numerical operations (e.g., `real_t` as a replacement for floating-point or double types) and for electrical components (e.g. `current_pu_t`, `voltage_pu_t` and more).
- **Avoid template metaprogramming when possible**: While templates can be powerful, they can also lead to code bloat and complexity. Use them judiciously, primarily for type safety and genericity in mathematical operations.

**Example Structure for a Filter:**
```cpp
// software/yeet_control/filters/low_pass_filter.hpp
#include "types.hpp" // include real_t

class low_pass_filter {
public:
    void configure_alpha(real_t alpha);
    void update(real_t input);
    void reset();
    real_t get_output() const;
private:
    real_t _state{0};
    real_t _alpha{0};
};
```

### 3. Uncompromising Unit Testing with Google Test
- **Mandatory Testing**: Every new component requires a corresponding test file (e.g.,test_low_pass_filter.cpp).
- **Complete Coverage**: All public methods (configure_,process,reset, getters) must have dedicated tests.
- **Numerical Accuracy**: For filters, controllers, and PLLs, tests must verify the output against a mathematically correct reference value. Assertions should check for approximate equality (e.g.,EXPECT_NEAR).
- **Edge Cases**: Test behavior with zero-value inputs, inputs at specified limits, multiplereset()calls, and sequences of configuration and processing.

### 4. CMake Build System Integration
- **Your Responsibility**: When you create a new implementation file (e.g.,low_pass_filter.cpp), you are responsible for its integration into the build system.
- **Action Required**: You must identify the correctCMakeLists.txtin thesoftware/yeet_control/directory and specify where the new.cppfile needs to be added to theyeet_controllibrary target.
- **Example Instruction**: "After creatingnew_component.cpp, add it to thetarget_sourceslist insoftware/yeet_control/CMakeLists.txtto ensure it is compiled into the library."

### 5. Folder structure
- **Organized by Functionality**: Place header files in `include/yeet_control/` and implementation files in `src/`. Test files should be in `tests/`. The CMakeLists.txt should be in the root of the `yeet_control` directory. The CMakeLists.txt for tests is found under `tests/`.
- **Test files**: Each test file should be named `<component_name>_test.cpp` and placed in the `tests/` directory. The test files should include the corresponding header file from `include/yeet_control/`.
