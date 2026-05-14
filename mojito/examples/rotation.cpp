#include <iostream>
#include <mojito/mojito.hpp>

int main()
{
    // Define a dq frame
    mojito::dq<float> frame(1.0f, 0.0f);
    std::cout << "Original dq frame: " << frame << "\n";

    // Rotate counter-clockwise by 90 degrees
    mojito::angle_wrapped angle_90 = mojito::angle_wrapped::from_degrees(90.0f);
    mojito::dq<float> rotated_ccw = frame.rotate_counter_clockwise(angle_90);
    std::cout << "Rotated CCW by 90 deg: " << rotated_ccw << " (expected approx [0, 1])\n";

    // Rotate clockwise by 45 degrees
    mojito::angle_wrapped angle_45 = mojito::angle_wrapped::from_degrees(45.0f);
    mojito::dq<float> rotated_cw = frame.rotate_clockwise(angle_45);
    std::cout << "Rotated CW by 45 deg: " << rotated_cw << " (expected approx [0.707, -0.707])\n";

    // Rotating an alphabeta frame
    mojito::alphabeta<float> ab_frame(1.0f, 0.0f);
    mojito::alphabeta<float> ab_rotated =
        ab_frame.rotate_counter_clockwise(mojito::angle_wrapped::from_degrees(180.0f));
    std::cout << "alphabeta original: " << ab_frame << "\n";
    std::cout << "alphabeta rotated 180 deg: " << ab_rotated << " (expected approx [-1, 0])\n";

    return 0;
}
