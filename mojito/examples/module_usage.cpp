#include <iostream>
#include <iomanip>
import mojito;

int main() {
    using namespace mojito;

    // Use units and quantities
    voltage_t v{120.0};
    resistance_t r{60.0};
    current_t i = v / r;

    std::cout << "--- Mojito C++20 Module Example ---" << std::endl;
    std::cout << "Voltage:    " << v << std::endl;
    std::cout << "Resistance: " << r << std::endl;
    std::cout << "Current:    " << i << " (calculated via Ohm's Law)" << std::endl;

    // Use coordinate frames
    abc<real_t> v_abc{1.0, -0.5, -0.5};
    alphabeta<real_t> v_ab = to_alphabeta(v_abc);

    std::cout << "\nCoordinate Transform (abc to alphabeta):" << std::endl;
    std::cout << "abc:        [" << v_abc.a() << ", " << v_abc.b() << ", " << v_abc.c() << "]" << std::endl;
    std::cout << "alphabeta:  [" << v_ab.alpha() << ", " << v_ab.beta() << "]" << std::endl;

    // Use angles and constants
    angle_wrapped theta = angle_wrapped::from_degrees(45.0);
    std::cout << "\nAngle:      " << std::fixed << std::setprecision(2) 
              << 45.0 << " degrees is " << theta.get_radians().value() << " radians" << std::endl;
    std::cout << "Pi is approx: " << pi << std::endl;

    return 0;
}
