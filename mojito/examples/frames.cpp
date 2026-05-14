#include <iostream>
#include <mojito/mojito.hpp>

int main()
{
    using namespace mojito;

    // Define a 3-phase current in SI (Amperes)
    current_t i_base(100.0f);
    abc<current_t> i_abc(current_t(100.0f), current_t(-50.0f), current_t(-50.0f));

    std::cout << "Original abc Current (SI): " << i_abc << "\n";

    // Convert to alphabeta frame (Clarke Transform)
    alphabeta<current_t> i_alphabeta = to_alphabeta(i_abc);
    std::cout << "Clarke Transform (alphabeta): " << i_alphabeta << "\n";

    // Convert to dq frame (Park Transform) at 30 degrees
    angle_wrapped theta = angle_wrapped::from_degrees(30.0f);
    dq<current_t> i_dq = to_dq(i_alphabeta, theta);
    std::cout << "Park Transform (dq) at 30 deg: " << i_dq << "\n";

    // Demonstrate the reverse transform (dq -> abc)
    abc<current_t> i_abc_back = to_abc(i_dq, theta);
    std::cout << "Reverse Transform back to abc: " << i_abc_back << "\n\n";

    // Mixing transformations and unit conversions
    std::cout << "Converting the dq frame to Per-Unit (PU):\n";
    dq<current_pu_t> i_dq_pu = to_pu(i_dq, i_base);
    std::cout << "dq Current (PU): " << i_dq_pu << "\n";

    return 0;
}
