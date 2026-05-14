#include <iostream>
#include <mojito/mojito.hpp>

int main()
{
    using namespace mojito;

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
    std::cout << "abc PU frame:\n  " << abc_pu << "\n";

    return 0;
}
