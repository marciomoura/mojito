#include <iostream>
#include <mojito/mojito.hpp>

// Define custom base tags
struct motor_side {};
struct grid_side {};

int main()
{
    using namespace mojito;

    // --- Standard Unit Conversions ---
    
    // 1. Define base quantities using SI type aliases
    voltage_t base_voltage(230.0f);
    std::cout << "Base Voltage: " << base_voltage << "\n";

    // 2. Define an SI quantity using type aliases
    voltage_t v_si(115.0f);
    std::cout << "SI Voltage: " << v_si << "\n";

    // 3. Convert SI to Per-Unit (PU) - results in voltage_pu_t
    voltage_pu_t v_pu = to_pu(v_si, base_voltage);
    std::cout << "Voltage in PU: " << v_pu << "\n";

    // 4. Convert SI to Percent - results in voltage_percent_t
    voltage_percent_t v_percent = to_percent(v_si, base_voltage);
    std::cout << "Voltage in Percent: " << v_percent << "\n";

    // 5. Convert PU to Percent
    voltage_percent_t v_percent_from_pu = to_percent(v_pu);
    std::cout << "Voltage in Percent (from PU): " << v_percent_from_pu << "\n";

    // 6. Convert PU back to SI
    voltage_t v_si_back = to_si(v_pu, base_voltage);
    std::cout << "SI Voltage (back from PU): " << v_si_back << "\n";

    // 7. Using frame types with unit aliases
    abc<voltage_t> abc_si(voltage_t(100.0f), voltage_t(-50.0f), voltage_t(-50.0f));
    std::cout << "abc SI frame:\n  " << abc_si << "\n";

    // Conversion works seamlessly with frames
    abc<voltage_pu_t> abc_pu = to_pu(abc_si, base_voltage);
    std::cout << "abc PU frame:\n  " << abc_pu << "\n\n";

    // --- Custom Per-Unit Bases ---
    std::cout << "--- Custom Per-Unit Bases ---\n";

    // 8. Define custom base quantities
    voltage_t motor_base_v(440.0f);
    voltage_t grid_base_v(380.0f);
    
    std::cout << "Motor side base: " << motor_base_v << "\n";
    std::cout << "Grid side base:  " << grid_base_v << "\n";

    // 9. Convert SI to custom PU bases
    voltage_custom_pu_t<motor_side> v_pu_motor = to_pu<motor_side>(v_si, motor_base_v);
    voltage_custom_pu_t<grid_side> v_pu_grid = to_pu<grid_side>(v_si, grid_base_v);

    std::cout << "Voltage in Motor-side PU: " << v_pu_motor << "\n";
    std::cout << "Voltage in Grid-side PU:  " << v_pu_grid << "\n";

    // 10. Convert between custom bases (manually through SI)
    voltage_custom_pu_t<grid_side> v_grid_from_motor_manual = to_pu<grid_side>(to_si(v_pu_motor, motor_base_v), grid_base_v);
    std::cout << "Voltage in Grid-side PU (manual through SI): " << v_grid_from_motor_manual << "\n";

    // 11. Convert between custom bases using to_different_pu
    voltage_custom_pu_t<grid_side> v_grid_from_motor_direct = to_different_pu<grid_side>(v_pu_motor, motor_base_v, grid_base_v);
    std::cout << "Voltage in Grid-side PU (using to_different_pu): " << v_grid_from_motor_direct << "\n";

    return 0;
}
