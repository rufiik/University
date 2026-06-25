#include <iostream>
#include "ring.hpp"

int main() {
    using R7 = Ring<7>;

    // Konstruktory i konwersje
    R7 a(3);           // konstruktor z liczby
    R7 b = a;          // konstruktor kopiujący
    R7 c;              // domyślny (0)
    std::cout << "a = " << a << ", b = " << b << ", c = " << c << "\n";

    // get()
    std::cout << "a.get() = " << a.get() << "\n";

    // Operatory arytmetyczne
    std::cout << "a + b = " << (a + b) << "\n";
    std::cout << "a - b = " << (a - b) << "\n";
    std::cout << "a * b = " << (a * b) << "\n";
    std::cout << "b / a = " << (b / a) << "\n";

    // Operatory przypisania
    R7 d(2);
    d += a;
    std::cout << "d += a -> d = " << d << "\n";
    d -= a;
    std::cout << "d -= a -> d = " << d << "\n";
    d *= a;
    std::cout << "d *= a -> d = " << d << "\n";
    d /= a;
    std::cout << "d /= a -> d = " << d << "\n";

    // Operatory porównań
    std::cout << std::boolalpha;
    std::cout << "a == b? " << (a == b) << "\n";
    std::cout << "a != b? " << (a != b) << "\n";
    std::cout << "a < b? " << (a < b) << "\n";
    std::cout << "a > b? " << (a > b) << "\n";
    std::cout << "a <= b? " << (a <= b) << "\n";
    std::cout << "a >= b? " << (a >= b) << "\n";

    // Operator strumieniowy
    std::cout << "Wypisanie a przez operator<<: " << a << "\n";

    // Wyjątek przy dzieleniu przez zero
    try {
        R7 zero(0);
        std::cout << "a / zero = " << (a / zero) << "\n";
    } catch (const std::invalid_argument& e) {
        std::cout << "Wyjątek przy dzieleniu przez zero: " << e.what() << "\n";
    }

    // Wyjątek przy dzieleniu przez element bez odwrotności (np. w Ring<8>)
    try {
        Ring<8> x(2), y(4);
        std::cout << "y / x = " << (y / x) << "\n";
    } catch (const std::invalid_argument& e) {
        std::cout << "Wyjątek w Ring<8>: " << e.what() << "\n";
    }

    return 0;
}