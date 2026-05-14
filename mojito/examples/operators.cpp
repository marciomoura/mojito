#include <iostream>
#include <mojito/mojito.hpp>

int main()
{
    // abc frame operators
    mojito::abc<float> abc1(1.0f, 0.5f, -0.5f);
    mojito::abc<float> abc2(0.5f, 1.0f, 0.5f);

    std::cout << "abc1: " << abc1 << "\n";
    std::cout << "abc2: " << abc2 << "\n";
    std::cout << "abc1 + abc2: " << (abc1 + abc2) << "\n";
    std::cout << "abc1 - abc2: " << (abc1 - abc2) << "\n";
    std::cout << "abc1 * 2.0: " << (abc1 * 2.0f) << "\n";
    std::cout << "0.5 * abc1: " << (0.5f * abc1) << "\n";
    std::cout << "abc1 / 2.0: " << (abc1 / 2.0f) << "\n\n";

    // alphabeta frame operators
    mojito::alphabeta<float> ab1(1.0f, 0.0f);
    mojito::alphabeta<float> ab2(0.0f, 1.0f);

    std::cout << "ab1: " << ab1 << "\n";
    std::cout << "ab2: " << ab2 << "\n";
    std::cout << "ab1 + ab2: " << (ab1 + ab2) << "\n";
    std::cout << "ab1 - ab2: " << (ab1 - ab2) << "\n";
    std::cout << "ab1 * 1.5: " << (ab1 * 1.5f) << "\n";
    std::cout << "ab1 / 2.0: " << (ab1 / 2.0f) << "\n\n";

    // dq frame operators
    mojito::dq<float> dq1(0.8f, 0.2f);
    mojito::dq<float> dq2(0.1f, 0.7f);

    std::cout << "dq1: " << dq1 << "\n";
    std::cout << "dq2: " << dq2 << "\n";
    std::cout << "dq1 + dq2: " << (dq1 + dq2) << "\n";
    std::cout << "dq1 - dq2: " << (dq1 - dq2) << "\n";
    std::cout << "dq1 * 2.0: " << (dq1 * 2.0f) << "\n";
    std::cout << "dq1 / 2.0: " << (dq1 / 2.0f) << "\n";

    return 0;
}
