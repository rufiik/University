module ring;

import std.stdio;
import std.exception;
import std.conv;
import std.traits;
import std.typecons;

struct DiophantineSolution {
    bool hasSolution;
    long x;
    long y;
}

DiophantineSolution solveLinearDiophantine(long a, long b, long c) {
    long g = gcd(a, b);
    DiophantineSolution sol = DiophantineSolution(false, 0, 0);

    if (g == 0 || c % g != 0)
        return sol;

    if (b == 0) {
        if (c % a == 0) {
            sol.hasSolution = true;
            sol.x = c / a;
            sol.y = 0;
        }
        return sol;
    }

    long x0, y0;
    extendedGCD(a, b, x0, y0);

    long factor = c / g;
    x0 *= factor;
    y0 *= factor;

    y0 = -y0;

    long step_x = b / g;
    long step_y = a / g;

    if (step_x == 0 || step_y == 0)
        return sol;

    long t_min_x = step_x > 0 ? (-x0 + step_x - 1) / step_x : (-x0) / step_x;
    long t_min_y = step_y > 0 ? (-y0 + step_y - 1) / step_y : (-y0) / step_y;
    long t = t_min_x > t_min_y ? t_min_x : t_min_y;

    x0 += t * step_x;
    y0 += t * step_y;

    if (x0 >= 0 && y0 >= 0) {
        sol.hasSolution = true;
        sol.x = x0;
        sol.y = y0;
    }
    return sol;
}

void extendedGCD(long a, long b, out long x, out long y) {
    if (b == 0) {
        x = 1;
        y = 0;
        return;
    }
    long x1, y1;
    extendedGCD(b, a % b, x1, y1);
    x = y1;
    y = x1 - (a / b) * y1;
}

long gcd(long a, long b) {
    while (b != 0) {
        long t = b;
        b = a % b;
        a = t;
    }
    return a < 0 ? -a : a;
}

struct Ring(long n) {
    static assert(n > 1, "n must be greater than 1");
    private long value;

    // Konstruktory
    this(long v) {
        value = ((v % n) + n) % n;
    }
    // Dostęp do wartości
    long get() const { return value; }
    static long getModulo() {
            return n;
        }
    // Operatory arytmetyczne
    auto opBinary(string op : "+")(Ring rhs) const { return Ring!n((value + rhs.value) % n); }
    auto opBinary(string op : "-")(Ring rhs) const { return Ring!n(((n + value - rhs.value) % n)); }
    auto opBinary(string op : "*")(Ring rhs) const { return Ring!n((value * rhs.value) % n); }
    auto opBinary(string op : "/")(Ring rhs) const {
        auto sol = solveLinearDiophantine(rhs.value, n, 1);
        enforce(sol.hasSolution, "Brak odwrotności (dzielenie przez element nieodwracalny)");
        long inv = ((sol.x % n) + n) % n;
        return Ring!n((value * inv) % n);
    }

    // Operatory przypisania
    ref Ring opOpAssign(string op : "+")(Ring rhs) { value = (value + rhs.value) % n; return this; }
    ref Ring opOpAssign(string op : "-")(Ring rhs) { value = ((n + value - rhs.value) % n); return this; }
    ref Ring opOpAssign(string op : "*")(Ring rhs) { value = (value * rhs.value) % n; return this; }
    ref Ring opOpAssign(string op : "/")(Ring rhs) {
        auto sol = solveLinearDiophantine(rhs.value, n, 1);
        enforce(sol.hasSolution, "Brak odwrotności (dzielenie przez element nieodwracalny)");
        long inv = ((sol.x % n) + n) % n;
        value = (value * inv) % n;
        return this;
    }

    // Operatory porównań
    bool opEquals(Ring rhs) const { return value == rhs.value; }
    int opCmp(Ring rhs) const { return value < rhs.value ? -1 : (value > rhs.value ? 1 : 0); }

    // Operator strumieniowy
    void toString(scope void delegate(const(char)[]) sink) const {
        import std.format : format;
        sink(format("mod %d: %d", n, value));
    }
}