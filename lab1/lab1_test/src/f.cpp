#include <iostream>

void f() {
    unsigned char var1{ 4 };
    unsigned long var2{ 12 };
    float var3{ 24.5 };

    std::cout << "var1(" << var1 << ") * var2(" << var2 << ") = " << var1 * var2 << "\n";
    std::cout << "var2(" << var2 << ") * var3(" << var3 << ") = " << var2 * var3 << "\n";
    std::cout << "var1(" << var1 << ") + var1(" << var1 << ") = " << var1 + var1 << "\n";
}
