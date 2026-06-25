import std.stdio;
import ring;

void main() {
    alias R7 = Ring!7;

    // Konstruktory i konwersje
    R7 a = R7(3);           // konstruktor z liczby
    R7 b = a;               // konstruktor kopiujący
    R7 c = R7();            // domyślny (0)
    writeln("a = ", a, ", b = ", b, ", c = ", c);

    // get()
    writeln("a.get() = ", a.get());

    // Operatory arytmetyczne
    writeln("a + b = ", a + b);
    writeln("a - b = ", a - b);
    writeln("a * b = ", a * b);
    writeln("b / a = ", b / a);

    // Operatory przypisania
    R7 d = R7(2);
    d += a;
    writeln("d += a -> d = ", d);
    d -= a;
    writeln("d -= a -> d = ", d);
    d *= a;
    writeln("d *= a -> d = ", d);
    d /= a;
    writeln("d /= a -> d = ", d);

    // Operatory porównań
    writeln("a == b? ", a == b);
    writeln("a != b? ", a != b);
    writeln("a < b? ", a < b);
    writeln("a > b? ", a > b);
    writeln("a <= b? ", a <= b);
    writeln("a >= b? ", a >= b);

    // Operator strumieniowy
    writeln("Wypisanie a przez toString: ", a);

    // Wyjątek przy dzieleniu przez zero
    try {
        R7 zero = R7(0);
        writeln("a / zero = ", a / zero);
    } catch (Exception e) {
        writeln("Wyjątek przy dzieleniu przez zero: ", e.msg);
    }

    // Wyjątek przy dzieleniu przez element bez odwrotności (np. w Ring!8)
    try {
        alias R8 = Ring!8;
        R8 x = R8(2), y = R8(4);
        writeln("y / x = ", y / x);
    } catch (Exception e) {
        writeln("Wyjątek w Ring!8: ", e.msg);
    }
}