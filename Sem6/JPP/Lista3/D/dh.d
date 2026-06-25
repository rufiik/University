module dh;

import std.random;
import std.algorithm;
import std.range;
import std.array;
import ring;

long[] primeFactors(long num) {
    long[] factors;
    for (long i = 2; i * i <= num; ++i) {
        while (num % i == 0) {
            factors ~= i;
            num /= i;
        }
    }
    if (num > 1) {
        factors ~= num;
    }
    return factors;
}

class DHSetup(T) {
    private T generator;
    private long order;

    this() {
        order = T.getModulo();  
        auto factors = primeFactors(order - 1);
        while (true) {
            auto candidate = T(uniform(1, order));
            if (isGenerator(candidate, factors)) {
                generator = candidate;
                break;
            }
        }
    }

    private bool isGenerator(T candidate, long[] factors) {
        foreach (q; factors) {
            auto powerResult = power(candidate, (order - 1) / q);
            if (powerResult.get() == 1) {
                return false;
            }
        }
        return true;
    }

    T power(T a, ulong b) const {
        auto result = T(1);
        while (b > 0) {
            if (b % 2 == 1) {
                result = result * a;
            }
            a = a * a;
            b /= 2;
        }
        return result;
    }

    T getGenerator() const {
        return generator;
    }
}