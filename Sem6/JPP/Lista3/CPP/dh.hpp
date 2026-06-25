#ifndef DH_HPP
#define DH_HPP

#include <random>
#include <stdexcept>
#include <vector>
#include <cmath>

template <typename T>
class DHSetup {
private:
    T generator;
    std::size_t order;

    std::size_t randomInRange(std::size_t min, std::size_t max) {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<std::size_t> dist(min, max);
        return dist(gen);
    }

    std::vector<std::size_t> primeFactors(std::size_t num) {
        std::vector<std::size_t> factors;
        for (std::size_t i = 2; i * i <= num; ++i) {
            while (num % i == 0) {
                factors.push_back(i);
                num /= i;
            }
        }
        if (num > 1) {
            factors.push_back(num);
        }
        return factors;
    }

    T powerImpl(const T& a, unsigned long b) const {
        T result(1);
        T base = a;
        while (b > 0) {
            if (b % 2 == 1) {
                result = result * base;
            }
            base = base * base;
            b /= 2;
        }
        return result;
    }

    bool isGenerator(const T& candidate, const std::vector<std::size_t>& factors) {
        for (std::size_t q : factors) {
            T powerResult = powerImpl(candidate, (order - 1) / q);
            if (powerResult.get() == 1) {
                return false;
            }
        }
        return true;
    }

public:
    DHSetup() {
        order = T::getModulo(); 
        std::vector<std::size_t> factors = primeFactors(order - 1);
        while (true) {
            T candidate(randomInRange(1, order - 1));
            if (isGenerator(candidate, factors)) {
                generator = candidate;
                break;
            }
        }
    }

    T getGenerator() const {
        return generator;
    }

    T power(T a, unsigned long b) const {
        return powerImpl(a, b);
    }
};

#endif