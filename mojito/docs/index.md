# YeeT Control Library Documentation

## Overview

The YeeT Control Library provides fundamental control system components and algorithms for real-time power electronics applications. The library is designed for deterministic execution in embedded systems and supports various coordinate frame transformations commonly used in power systems control.

## Core Components

### Mathematical Utilities

#### [Dimension and Units Library](units.md)
Type-safe dimensional analysis framework for scientific and engineering computations.
- **Features**: Compile-time dimensional checking, SI and Per-Unit systems, zero runtime overhead
- **File**: `yeet_control/units.hpp`
- **Applications**: Power calculations, unit conversion, engineering equation validation

#### [Angle Wrapped](angle_wrapped.md)
Type-safe angle representation with automatic wrapping using uint32_t overflow.
- **Features**: Zero-cost wrapping, high resolution (~1.46e-9 rad), exact binary angles, O(1) operations
- **File**: `yeet_control/angle_wrapped.hpp`
- **Applications**: Phase angle tracking, firing angle control, PLL integration, rotor position

#### [Coordinate Reference Frames](reference_frame.md)
Strongly-typed coordinate system representations for three-phase power systems.
- **Types**: `abc<T>`, `alphabeta<T>`, `dq<T>` with bidirectional transformations
- **File**: `yeet_control/reference_frame.hpp`
- **Applications**: Field-oriented control, Clarke/Park transforms, vector control, power quality analysis

#### **Constants and Utilities**
Mathematical constants and utility functions optimized for power electronics.
- **Features**: High-precision π, √2, √3, unit conversion functions
- **File**: `yeet_control/constants.hpp`
- **Applications**: Transformation scaling, power calculations, harmonic analysis

### Control Algorithms

#### [PI Controller](pi_controller.md)
Discrete-time Proportional-Integral controller with advanced anti-windup capabilities.
- **Features**: Back-calculation and integrator clamping anti-windup, coordinate frame support
- **File**: `yeet_control/pi_controller.hpp`
- **Applications**: Current control, voltage regulation, speed control

#### [Integrator](integrator.md)
Discrete-time numerical integrator using rectangular (forward Euler) method.
- **Features**: Fixed sampling time, template-based, resettable state, coordinate frame support
- **File**: `yeet_control/integrator.hpp`
- **Applications**: Position from velocity, PLL angle integration, PI control integral term, flux estimation

#### [Rate of Change Limiter](rate_of_change_limiter.md)
Slew rate limiter for smooth signal transitions and acceleration control.
- **Features**: Symmetric rate limiting, enable/disable control, configurable rate, real-time safe
- **File**: `yeet_control/rate_of_change_limiter.hpp`
- **Applications**: Reference smoothing, motor acceleration control, voltage ramping, torque limiting

#### [Mechanical Load Model](mechanical_load.md)
Dynamic mechanical load simulation for motor control applications.
- **Features**: Inertia and damping modeling, per-unit operation, speed tracking
- **File**: `yeet_control/mechanical_load.hpp`
- **Applications**: Motor simulation, speed control testing, drive system modeling

#### [Pulse Generator (Thyristor) - 6-Pulse Configuration](pulse_generator_thyristor_6pulse.md)
Precision timing control for 6-pulse thyristor bridge converters.
- **Features**: IEEE standard firing sequence, phase-locked operation, safety interlocks
- **Files**: `yeet_control/six_pulse_generator.hpp`, `yeet_control/pulse_generator.hpp`
- **Applications**: Load-Commutated Inverters (LCI), soft-starters, rectifier control

#### [Speed-Voltage Relationship](speed_voltage_relationship.md)
Model for the relationship between motor speed and voltage in synchronous machines.
- **Features**: Linear V/Hz control, field weakening, per-unit operation
- **File**: `yeet_control/speed_voltage_relationship.hpp`
- **Applications**: Motor control, V/f scalar control, field weakening strategies

### Utility Components

#### [Lookup Tables](lookup_table.md)
Efficient 1D and 2D lookup table implementations with interpolation.
- **Features**: Linear interpolation, boundary clamping, compile-time optimization
- **Files**: `yeet_control/lookup_table_1d.hpp`, `yeet_control/lookup_table_2d.hpp`
- **Applications**: Nonlinear function approximation, saturation curves, efficiency maps

### Signal Processing

#### **Three-Phase Waveform Generator**
Generates balanced and unbalanced three-phase reference signals with harmonic content.
- **Features**: Fundamental and harmonic generation, noise injection, multiple coordinate frames
- **File**: `yeet_control/three_phase_waveform_generator.hpp`
- **Applications**: Test signal generation, plant simulation, controller validation

#### **Coordinate Frame Transformations**
Standard power systems coordinate transformations for three-phase analysis.
- **Transformations**: ABC ↔ αβ (Clarke), αβ ↔ dq (Park), symmetrical components
- **File**: `yeet_control/transforms.hpp`
- **Applications**: Field-oriented control, unbalance analysis, harmonic filtering

### Filtering and Signal Conditioning

#### [First-Order Low-Pass Filter](first_order_low_pass_filter.md)
Discrete-time first-order low-pass filter using bilinear transform with frequency pre-warping.
- **Features**: Double-precision arithmetic, frequency response analysis, settling time calculation
- **File**: `yeet_control/first_order_low_pass_filter.hpp`
- **Applications**: Sensor filtering, current measurement smoothing, DC bus voltage filtering, speed estimation

#### [SOGI Filter and Dual-SOGI](sogi.md)
Second-Order Generalized Integrator for signal filtering and positive sequence extraction.
- **Features**: Adaptive filtering, orthogonal signal generation, unbalance rejection
- **Files**: `yeet_control/sogi_filter.hpp`, `yeet_control/dual_sogi_srf_pll.hpp`
- **Applications**: Grid synchronization, harmonic filtering, sequence component extraction

#### **Phase-Locked Loop (PLL)**
Grid synchronization and frequency tracking for power converter applications.

##### [Three-Phase PLL](three_phase_pll.md)
Synchronous reference frame (SRF) based PLL for three-phase systems.
- **Features**: Clarke/Park transformations, PI controller, frequency limits, lock detection
- **File**: `yeet_control/three_phase_pll.hpp`
- **Applications**: Grid-tied inverters, motor control, power quality monitoring

##### [SRF-PLL](srf_pll.md)
Mathematical foundation and theory of synchronous reference frame PLLs.
- **Features**: Position and speed estimation, field-oriented control
- **Applications**: Sensorless motor control, back-EMF tracking

## Real-Time Considerations

### Performance Guidelines

**Execution Time Requirements:**
- All control components designed for sub-microsecond execution
- Deterministic memory allocation (no dynamic allocation in control loops)
- Fixed-point arithmetic support for embedded processors

**Sampling Rate Compatibility:**
- Standard control rates: 10 kHz, 20 kHz, 50 kHz
- Thyristor firing: 100 kHz for precision timing
- Measurement sampling: 1 MHz for high-frequency applications

**Memory Usage:**
- Minimal stack allocation in real-time functions
- Pre-allocated buffers for filter states and histories
- Compile-time configuration for memory optimization

### Safety and Reliability

**Fault Tolerance:**
- Input validation and range checking
- Safe default states for all components
- Graceful degradation during parameter errors

**Diagnostic Features:**
- Status reporting for all control components
- Error logging with minimal performance impact
- Real-time monitoring capabilities

## Integration with SFC Applications

### Software Platform Integration

The yeet_control library integrates seamlessly with the SFC (Soft-starter and Frequency Converter) application layer:

**Component Reuse:**
- Control algorithms used by SFC motor control
- Pulse generators for thyristor-based soft-starters
- Signal processing for power quality monitoring

**Data Flow:**
- Standardized interfaces between control and application layers
- Dictionary-based signal naming for Simulink integration
- Type-safe coordinate frame passing

### Hardware Abstraction

**Platform Independence:**
- Template-based design supports different numeric types
- Configurable precision for different hardware platforms
- Cross-platform compilation support (x86, ARM, DSP)

**Hardware-in-the-Loop (HIL):**
- Real-time simulation compatibility
- Deterministic timing for hardware interfaces
- Support for multiple sampling rates

## Testing and Validation

### Unit Testing

All components include comprehensive unit tests following Google Test framework:
- **Functional verification**: Algorithm correctness and edge cases
- **Performance testing**: Execution time and memory usage validation
- **Integration testing**: Component interaction and data flow verification

### Plot-Based Validation

Specialized test suites generate time-domain plots for visual verification:
- **Pulse timing verification**: Gate signal timing and sequence validation
- **Control response analysis**: Step response and frequency domain characteristics
- **Signal quality assessment**: Harmonic content and noise analysis

### Real-Time Validation

Hardware-in-the-loop testing with actual power electronics:
- **Timing verification**: Real-time execution on target hardware
- **Fault injection**: Robustness testing under adverse conditions
- **Long-term stability**: Extended operation validation

## Contributing Guidelines

### Code Standards

- **C++ Core Guidelines**: Strict adherence to modern C++ practices
- **Real-time constraints**: No dynamic allocation, deterministic execution
- **Template design**: Generic programming for coordinate frame support
- **Documentation**: Comprehensive Doxygen comments for all public interfaces

### Performance Requirements

- **Execution time**: All functions must complete within allocated time budgets
- **Memory usage**: Static allocation only, no heap operations in control loops
- **Numerical stability**: Validated across full operating parameter ranges

### Integration Requirements

- **SFC compatibility**: Must integrate with existing SFC application architecture
- **Simulink interface**: Support for code generation and hardware deployment
- **Cross-platform**: Windows/Linux development, embedded target deployment
