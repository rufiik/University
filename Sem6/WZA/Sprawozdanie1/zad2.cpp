#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <tuple>
#include <string>

class Polynomial {
private:
    std::vector<double> coeff;
    
    void trim() {
        while (coeff.size() > 1 && std::abs(coeff.back()) < 1e-12) {
            coeff.pop_back();
        }
        if (coeff.empty()) {
            coeff = {0.0};
        }
    }
    
    // Normalizuje wielomian 
    Polynomial normalized() const {
        if (isZero()) return *this;
        double leading = coeff.back();
        std::vector<double> normCoeff(coeff.size());
        for (size_t i = 0; i < coeff.size(); i++) {
            normCoeff[i] = coeff[i] / leading;
        }
        return Polynomial(normCoeff);
    }
    
public:
    Polynomial() : coeff({0.0}) {}
    
    Polynomial(const std::vector<double>& c) : coeff(c) {
        trim();
    }
    
    Polynomial(int degree, double value = 0.0) : coeff(degree + 1, value) {
        if (degree == 0 && value == 0) coeff = {0.0};
        trim();
    }
    
    int degree() const {
        if (coeff.size() == 1 && std::abs(coeff[0]) < 1e-12) return -1;
        return coeff.size() - 1;
    }
    
    double getCoeff(int power) const {
        if (power < 0 || power > degree()) return 0.0;
        return coeff[power];
    }
    
    int norm() const {
        return degree();
    }
    
    bool isZero() const {
        return degree() == -1;
    }
    
    Polynomial operator+(const Polynomial& other) const {
        int maxDeg = std::max(degree(), other.degree());
        if (maxDeg == -1) maxDeg = 0;
        std::vector<double> result(maxDeg + 1, 0.0);
        
        for (int i = 0; i <= degree(); i++) {
            if (i < (int)result.size()) result[i] += coeff[i];
        }
        for (int i = 0; i <= other.degree(); i++) {
            if (i < (int)result.size()) result[i] += other.coeff[i];
        }
        
        return Polynomial(result);
    }
    
    Polynomial operator-(const Polynomial& other) const {
        int maxDeg = std::max(degree(), other.degree());
        if (maxDeg == -1) maxDeg = 0;
        std::vector<double> result(maxDeg + 1, 0.0);
        
        for (int i = 0; i <= degree(); i++) {
            if (i < (int)result.size()) result[i] += coeff[i];
        }
        for (int i = 0; i <= other.degree(); i++) {
            if (i < (int)result.size()) result[i] -= other.coeff[i];
        }
        
        return Polynomial(result);
    }
    
    Polynomial operator*(const Polynomial& other) const {
        if (isZero() || other.isZero()) return Polynomial(std::vector<double>{0.0});
        
        int newDeg = degree() + other.degree();
        if (newDeg < 0) return Polynomial(std::vector<double>{0.0});
        
        std::vector<double> result(newDeg + 1, 0.0);
        
        for (int i = 0; i <= degree(); i++) {
            for (int j = 0; j <= other.degree(); j++) {
                result[i + j] += coeff[i] * other.coeff[j];
            }
        }
        
        return Polynomial(result);
    }
    
    Polynomial operator*(double scalar) const {
        if (isZero() || std::abs(scalar) < 1e-12) return Polynomial(std::vector<double>{0.0});
        
        std::vector<double> result(coeff.size());
        for (size_t i = 0; i < coeff.size(); i++) {
            result[i] = coeff[i] * scalar;
        }
        return Polynomial(result);
    }
    
    std::pair<Polynomial, Polynomial> divide(const Polynomial& divisor) const {
        if (divisor.isZero()) {
            throw std::invalid_argument("Dzielenie przez wielomian zerowy");
        }
        
        Polynomial remainder = *this;
        Polynomial quotient;
        
        int divisorDeg = divisor.degree();
        double leadingCoeff = divisor.coeff.back();
        
        while (remainder.degree() >= divisorDeg && !remainder.isZero()) {
            int degDiff = remainder.degree() - divisorDeg;
            double factor = remainder.coeff.back() / leadingCoeff;
            
            std::vector<double> termCoeff(degDiff + 1, 0.0);
            termCoeff[degDiff] = factor;
            Polynomial term(termCoeff);
            
            quotient = quotient + term;
            remainder = remainder - (term * divisor);
            remainder.trim();
        }
        
        return {quotient, remainder};
    }
    
    Polynomial exactDivide(const Polynomial& divisor) const {
        auto [q, r] = divide(divisor);
        if (!r.isZero()) {
            throw std::runtime_error("Dzielenie nie jest dokładne");
        }
        return q;
    }
    
    static Polynomial gcd(const Polynomial& a, const Polynomial& b) {
        if (a.isZero() && b.isZero()) return Polynomial(std::vector<double>{0.0});
        if (a.isZero()) return b.normalized();
        if (b.isZero()) return a.normalized();
        
        Polynomial x = a;
        Polynomial y = b;
        
        while (!y.isZero()) {
            auto [q, r] = x.divide(y);
            x = y;
            y = r;
        }
        
        return x.normalized();
    }
    
    static std::tuple<Polynomial, Polynomial, Polynomial> extendedGCD(const Polynomial& a, const Polynomial& b) {
        if (b.isZero()) {
            Polynomial one(std::vector<double>{1.0});
            Polynomial zero(std::vector<double>{0.0});
            Polynomial normA = a.normalized();
            double factor = 1.0;
            if (!a.isZero()) {
                factor = a.coeff.back();
            }
            Polynomial normOne = one * (1.0 / factor);
            return {normA, normOne, zero};
        }
        
        auto [q, r] = a.divide(b);
        auto [g, s1, t1] = extendedGCD(b, r);
        
        Polynomial s = t1;
        Polynomial t = s1 - (t1 * q);
        
        return {g, s, t};
    }
    
    static Polynomial lcm(const Polynomial& a, const Polynomial& b) {
        if (a.isZero() || b.isZero()) {
            return Polynomial(std::vector<double>{0.0});
        }
        
        Polynomial g = gcd(a, b);
        Polynomial product = a * b;
        
        return product.exactDivide(g);
    }
    
    void print() const {
        if (isZero()) {
            std::cout << "0";
            return;
        }
        
        bool first = true;
        for (int i = degree(); i >= 0; i--) {
            double c = coeff[i];
            if (std::abs(c) < 1e-10) continue;
            
            if (!first && c > 0) std::cout << "+";
            first = false;
            
            if (std::abs(c - 1.0) < 1e-10 && i > 0) {
            } else if (std::abs(c + 1.0) < 1e-10 && i > 0) {
                std::cout << "-";
            } else {
                if (std::abs(c - std::round(c)) < 1e-10) {
                    std::cout << std::round(c);
                } else {
                    std::cout << c;
                }
            }
            
            if (i > 0) {
                std::cout << "x";
                if (i > 1) std::cout << "^" << i;
            }
        }
    }
    
    friend std::ostream& operator<<(std::ostream& os, const Polynomial& p) {
        p.print();
        return os;
    }
};

Polynomial makePolynomial(const std::vector<double>& coeffsHighToLow) {
    std::vector<double> coeffsLowToHigh(coeffsHighToLow.rbegin(), coeffsHighToLow.rend());
    return Polynomial(coeffsLowToHigh);
}

void runTask2Tests(int a, int b, int c, int d, int e, int f) {
    std::cout << "\n========== ZADANIE 2: PIERŚCIEŃ WIELOMIANÓW R[x] ==========\n";
    std::cout << "a=" << a << ", b=" << b << ", c=" << c << ", d=" << d << ", e=" << e << ", f=" << f << "\n\n";
    
    std::cout << "=== 2a) Norma i dzielenie ===\n";
    
    std::vector<double> coeff_v(a + 1, 0.0);
    coeff_v[a] = c;      
    coeff_v[0] = b;      
    Polynomial poly1_plus_b(coeff_v);
    
    Polynomial poly2 = makePolynomial({1.0, 1.0}); 
    
    std::cout << "Wielomian v(x) = " << c << "x^" << a << " + " << b << " = " << poly1_plus_b << "\n";
    std::cout << "Norma (stopień) v(x) = " << poly1_plus_b.norm() << "\n\n";
    
    std::cout << "Dzielenie: (" << poly1_plus_b << ") / (" << poly2 << ")\n";
    auto [q, r] = poly1_plus_b.divide(poly2);
    std::cout << "Iloraz = " << q << "\n";
    std::cout << "Reszta = " << r << "\n";
    std::cout << "Sprawdzenie: " << q << " * " << poly2 << " + " << r << " = " << (q * poly2 + r) << "\n\n";
    
    std::cout << "=== 2b) Rozszerzony NWD i NWW ===\n";
    
    std::vector<double> v_coeff = {(double)d, (double)c, (double)b, (double)a};
    Polynomial v(v_coeff);
    
    std::vector<double> w_coeff = {0.0, (double)f, (double)e, (double)d};
    Polynomial w(w_coeff);
    
    std::cout << "v(x) = " << v << "\n";
    std::cout << "w(x) = " << w << "\n\n";
    
    auto [g, s, t] = Polynomial::extendedGCD(v, w);
    std::cout << "NWD(v, w) = " << g << "\n";
    std::cout << "s(x) = " << s << "\n";
    std::cout << "t(x) = " << t << "\n";
    std::cout << "Sprawdzenie: s*v + t*w = " << (s * v + t * w) << "\n\n";
    
    Polynomial lcm_v_w = Polynomial::lcm(v, w);
    std::cout << "NWW(v, w) = " << lcm_v_w << "\n\n";
    
    std::cout << "=== Dodatkowo: NWW(v, w+g) dla g = 52.2738407131298 ===\n";
    
    double g_wolfram = 52.2738407131298;
    std::vector<double> wg_coeff = {g_wolfram, (double)f, (double)e, (double)d};
    Polynomial wg(wg_coeff);
    
    std::cout << "w(x)+g = " << wg << "\n";
    
    Polynomial gcd_v_wg = Polynomial::gcd(v, wg);
    Polynomial lcm_v_wg = Polynomial::lcm(v, wg);
    
    std::cout << "NWD(v, w+g) = " << gcd_v_wg << "\n";
    std::cout << "NWW(v, w+g) = " << lcm_v_wg << "\n";
}

int main() {
    int a = 2, b = 7, c = 9, d = 7, e = 5, f = 2;
    
    try {
        runTask2Tests(a, b, c, d, e, f);
    } catch (const std::exception& ex) {
        std::cerr << "Błąd: " << ex.what() << std::endl;
    }
    
    return 0;
}