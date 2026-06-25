#ifndef RING_HPP
#define RING_HPP
#include <iostream>
#include <stdexcept>
#include <type_traits>
#include "mathlib.hpp"
template <std::size_t n>
class Ring {
    static_assert(n > 1, "n must be greater than 1");
    std::size_t value;
public:
    // Konstruktory
    Ring(std::size_t v = 0) : value(v % n) {}
    Ring(const Ring& other) = default;

    // Operatory arytmetyczne
    Ring operator+(const Ring& other) const;
    Ring operator-(const Ring& other) const;
    Ring operator*(const Ring& other) const;
    Ring operator/(const Ring& other) const;

    // Operatory przypisania
    Ring& operator+=(const Ring& other);
    Ring& operator-=(const Ring& other);
    Ring& operator*=(const Ring& other);
    Ring& operator/=(const Ring& other);

    // Operatory porównań
    bool operator==(const Ring& other) const;
    bool operator!=(const Ring& other) const;
    bool operator<(const Ring& other) const;
    bool operator>(const Ring& other) const;
    bool operator<=(const Ring& other) const;
    bool operator>=(const Ring& other) const;

    // Operator strumieniowy
    friend std::ostream& operator<<(std::ostream& os, const Ring& r) {
        os << "mod " << n << ": " << r.value;
        return os;
    }
    std::size_t get() const { return value; }
};
//arytmetyczne
template <std::size_t n>
Ring<n> Ring<n>::operator+(const Ring& other) const{
    return Ring((value+ other.value)%n);
}
template <std::size_t n>
Ring<n> Ring<n>::operator-(const Ring& other) const {
    return Ring((n+value-other.value)% n);
}
template <std::size_t n>
Ring<n> Ring<n>::operator*(const Ring& other) const {
    return Ring((value *other.value)%n);
}
template <std::size_t n>
Ring<n> Ring<n>::operator/(const Ring& other) const {
    auto solution = solve_linear_diophantine(other.value,n, 1);
    if(!solution.has_solution){
        throw std::invalid_argument("Brak odwrotności (dzielenie przez element nieodwracalny)");
    }
    std::size_t inv = solution.x % n;
    return Ring((value*inv)%n);
}
//podstawienia
template <std::size_t n>
Ring<n>& Ring<n>::operator+=(const Ring& other) {
    value = (value + other.value) % n;
    return *this;
}

template <std::size_t n>
Ring<n>& Ring<n>::operator-=(const Ring& other) {
    value = (n + value - other.value) % n;
    return *this;
}

template <std::size_t n>
Ring<n>& Ring<n>::operator*=(const Ring& other) {
    value = (value * other.value) % n;
    return *this;
}

template <std::size_t n>
Ring<n>& Ring<n>::operator/=(const Ring& other) {
    auto solution = solve_linear_diophantine(other.value, n, 1);
    if (!solution.has_solution) {
        throw std::invalid_argument("Brak odwrotności (dzielenie przez element nieodwracalny)");
    }
    std::size_t inv = solution.x % n;
    value = (value * inv) % n;
    return *this;
}
// Porównania
template <std::size_t n>
bool Ring<n>::operator==(const Ring& other) const {
    return value == other.value;
}

template <std::size_t n>
bool Ring<n>::operator!=(const Ring& other) const {
    return value != other.value;
}

template <std::size_t n>
bool Ring<n>::operator<(const Ring& other) const {
    return value < other.value;
}

template <std::size_t n>
bool Ring<n>::operator>(const Ring& other) const {
    return value > other.value;
}

template <std::size_t n>
bool Ring<n>::operator<=(const Ring& other) const {
    return value <= other.value;
}

template <std::size_t n>
bool Ring<n>::operator>=(const Ring& other) const {
    return value >= other.value;
}






#endif // RING_HPP