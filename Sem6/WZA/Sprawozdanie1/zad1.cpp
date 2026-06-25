#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <set>
#include <string>

class GaussianInteger {
private:
    int real;
    int imag;

public:
    GaussianInteger(int r = 0, int i = 0) : real(r), imag(i) {}

    int getReal() const { return real; }
    int getImag() const { return imag; }
    
    void setReal(int r) { real = r; }
    void setImag(int i) { imag = i; }

    int norm() const {
        return real * real + imag * imag;
    }

    GaussianInteger operator*(const GaussianInteger& other) const {
        return GaussianInteger(
            real * other.real - imag * other.imag,
            real * other.imag + imag * other.real
        );
    }

    GaussianInteger operator+(const GaussianInteger& other) const {
        return GaussianInteger(real + other.real, imag + other.imag);
    }

    GaussianInteger operator-(const GaussianInteger& other) const {
        return GaussianInteger(real - other.real, imag - other.imag);
    }
    
    bool operator==(const GaussianInteger& other) const {
        return real == other.real && imag == other.imag;
    }
    
    bool operator<(const GaussianInteger& other) const {
        if (real != other.real) return real < other.real;
        return imag < other.imag;
    }

    std::pair<GaussianInteger, GaussianInteger> divideWithRemainder(const GaussianInteger& divisor) const {
        if (divisor.real == 0 && divisor.imag == 0) {
            throw std::invalid_argument("Dzielenie przez zero");
        }

        int denominator = divisor.norm();
        
        int numeratorReal = real * divisor.real + imag * divisor.imag;
        int numeratorImag = imag * divisor.real - real * divisor.imag;
        
        int quotientReal = static_cast<int>(std::round(static_cast<double>(numeratorReal) / denominator));
        int quotientImag = static_cast<int>(std::round(static_cast<double>(numeratorImag) / denominator));
        
        GaussianInteger quotient(quotientReal, quotientImag);
        GaussianInteger remainder = *this - (quotient * divisor);
        
        if (remainder.norm() >= divisor.norm()) {
            GaussianInteger bestQuotient = quotient;
            GaussianInteger bestRemainder = remainder;
            
            for (int dr = -1; dr <= 1; dr++) {
                for (int di = -1; di <= 1; di++) {
                    if (dr == 0 && di == 0) continue;
                    GaussianInteger testQuotient(quotientReal + dr, quotientImag + di);
                    GaussianInteger testRemainder = *this - (testQuotient * divisor);
                    if (testRemainder.norm() < bestRemainder.norm()) {
                        bestQuotient = testQuotient;
                        bestRemainder = testRemainder;
                    }
                }
            }
            return {bestQuotient, bestRemainder};
        }
        
        return {quotient, remainder};
    }
    
    static std::vector<GaussianInteger> getUnits() {
        return {
            GaussianInteger(1, 0),
            GaussianInteger(-1, 0),
            GaussianInteger(0, 1),
            GaussianInteger(0, -1)
        };
    }
    
    GaussianInteger multiplyByUnit(int unitIndex) const {
        auto units = getUnits();
        return *this * units[unitIndex];
    }
    
    static std::set<GaussianInteger> getAllPossibleGCD(const GaussianInteger& a, const GaussianInteger& b) {
        std::set<GaussianInteger> results;
        
        if (a.norm() == 0 && b.norm() == 0) {
            results.insert(GaussianInteger(0, 0));
            return results;
        }
        
        GaussianInteger base = gcdBase(a, b);
        
        auto units = getUnits();
        for (const auto& unit : units) {
            GaussianInteger variant = base * unit;
            results.insert(variant);
        }
        
        return results;
    }
    
    static GaussianInteger gcdBase(const GaussianInteger& a, const GaussianInteger& b) {
        if (a.norm() == 0) return b;
        if (b.norm() == 0) return a;
        
        GaussianInteger x = a;
        GaussianInteger y = b;
        
        while (y.norm() != 0) {
            auto [quotient, remainder] = x.divideWithRemainder(y);
            x = y;
            y = remainder;
        }
        
        return x;
    }
    
    static std::set<GaussianInteger> gcdListAll(const std::vector<GaussianInteger>& numbers) {
        if (numbers.empty()) {
            std::set<GaussianInteger> result;
            result.insert(GaussianInteger(0, 0));
            return result;
        }
        if (numbers.size() == 1) {
            return getAllPossibleGCD(numbers[0], numbers[0]);
        }
        
        auto currentSet = getAllPossibleGCD(numbers[0], numbers[1]);
        for (size_t i = 2; i < numbers.size(); i++) {
            std::set<GaussianInteger> newSet;
            for (const auto& g : currentSet) {
                auto tempSet = getAllPossibleGCD(g, numbers[i]);
                newSet.insert(tempSet.begin(), tempSet.end());
            }
            currentSet = newSet;
        }
        return currentSet;
    }
    
    static std::set<GaussianInteger> getAllPossibleLCM(const GaussianInteger& a, const GaussianInteger& b) {
        std::set<GaussianInteger> results;
        
        if (a.norm() == 0 || b.norm() == 0) {
            results.insert(GaussianInteger(0, 0));
            return results;
        }
        
        auto gcdSet = getAllPossibleGCD(a, b);
        
        for (const auto& g : gcdSet) {
            if (g.norm() == 0) continue;
            
            GaussianInteger product = a * b;
            auto [quotient, remainder] = product.divideWithRemainder(g);
            
            if (remainder.norm() == 0) {
                auto units = getUnits();
                for (const auto& unit : units) {
                    results.insert(quotient * unit);
                }
            }
        }
        
        return results;
    }
    
    static std::set<GaussianInteger> lcmListAll(const std::vector<GaussianInteger>& numbers) {
        if (numbers.empty()) {
            std::set<GaussianInteger> result;
            result.insert(GaussianInteger(1, 0));
            return result;
        }
        if (numbers.size() == 1) {
            return getAllPossibleLCM(numbers[0], numbers[0]);
        }
        
        auto currentSet = getAllPossibleLCM(numbers[0], numbers[1]);
        for (size_t i = 2; i < numbers.size(); i++) {
            std::set<GaussianInteger> newSet;
            for (const auto& l : currentSet) {
                auto tempSet = getAllPossibleLCM(l, numbers[i]);
                newSet.insert(tempSet.begin(), tempSet.end());
            }
            currentSet = newSet;
        }
        return currentSet;
    }

    std::string toString() const {
        if (real == 0 && imag == 0) return "0";
        if (real == 0) {
            if (imag == 1) return "i";
            if (imag == -1) return "-i";
            return std::to_string(imag) + "i";
        }
        if (imag == 0) return std::to_string(real);
        
        std::string result = std::to_string(real);
        if (imag > 0) {
            if (imag == 1) result += "+i";
            else result += "+" + std::to_string(imag) + "i";
        } else {
            if (imag == -1) result += "-i";
            else result += std::to_string(imag) + "i";
        }
        return result;
    }
    
    friend std::ostream& operator<<(std::ostream& os, const GaussianInteger& gi) {
        os << gi.toString();
        return os;
    }
};

void runTests(int a, int b, int c, int d, int e, int f) {
    std::cout << "===== TESTY DLA NUMERU INDEKSU =====\n";
    std::cout << "Zakładając, że abcdef to cyfry indeksu:\n";
    std::cout << "a = " << a << ", b = " << b << ", c = " << c << ", d = " << d << ", e = " << e << ", f = " << f << "\n\n";
    
    GaussianInteger num1(a, b);
    GaussianInteger num2(c, d);
    GaussianInteger num3(e, d);
    
    std::cout << "1a) Norma liczby " << num1 << " = " << num1.norm() << "\n\n";
    
    GaussianInteger dividend(c + a, d + b);
    GaussianInteger divisor(e, f);
    std::cout << "1b) Dzielenie: (" << dividend << ") / (" << divisor << ")\n";
    
    auto [quotient, remainder] = dividend.divideWithRemainder(divisor);
    std::cout << "    Jeden z możliwych wyników:\n";
    std::cout << "    Iloraz: " << quotient << "\n";
    std::cout << "    Reszta: " << remainder << "\n";
    std::cout << "    Sprawdzenie: " << quotient << " * " << divisor << " + " << remainder 
              << " = " << (quotient * divisor + remainder) << "\n";
    
    std::cout << "\n    Wszystkie możliwe wyniki (różne ilorazy):\n";
    int denom = divisor.norm();
    int numReal = dividend.getReal() * divisor.getReal() + dividend.getImag() * divisor.getImag();
    int numImag = dividend.getImag() * divisor.getReal() - dividend.getReal() * divisor.getImag();
    
    std::set<std::pair<GaussianInteger, GaussianInteger>> uniqueResults;
    
    for (int dr = -1; dr <= 1; dr++) {
        for (int di = -1; di <= 1; di++) {
            int qr = static_cast<int>(std::round(static_cast<double>(numReal) / denom)) + dr;
            int qi = static_cast<int>(std::round(static_cast<double>(numImag) / denom)) + di;
            GaussianInteger testQuotient(qr, qi);
            GaussianInteger testRemainder = dividend - (testQuotient * divisor);
            if (testRemainder.norm() < divisor.norm()) {
                uniqueResults.insert({testQuotient, testRemainder});
            }
        }
    }
    
    int idx = 1;
    for (const auto& [q, r] : uniqueResults) {
        std::cout << "      " << idx++ << ". Iloraz: " << q << ", Reszta: " << r 
                  << " (norma reszty: " << r.norm() << ")\n";
    }
    
    std::cout << "\n1c) NWD i NWW dla trójki liczb: " << num1 << ", " << num2 << ", " << num3 << "\n";
    
    std::vector<GaussianInteger> numbers = {num1, num2, num3};
    
    std::cout << "\n    WSZYSTKIE MOŻLIWE WYNIKI NWD:\n";
    auto gcdResults = GaussianInteger::gcdListAll(numbers);
    idx = 1;
    for (const auto& g : gcdResults) {
        std::cout << "      " << idx++ << ". " << g << "\n";
    }
    
    std::cout << "\n    WSZYSTKIE MOŻLIWE WYNIKI NWW:\n";
    auto lcmResults = GaussianInteger::lcmListAll(numbers);
    idx = 1;
    for (const auto& l : lcmResults) {
        std::cout << "      " << idx++ << ". " << l << "\n";
    }
    
    std::cout << "\n    Dodatkowe przypadki:\n";
    
    std::vector<GaussianInteger> emptyList;
    std::vector<GaussianInteger> singleList = {num1};
    
    std::cout << "    Lista pusta:\n";
    auto emptyGCD = GaussianInteger::gcdListAll(emptyList);
    auto emptyLCM = GaussianInteger::lcmListAll(emptyList);
    std::cout << "      NWD = ";
    for (const auto& g : emptyGCD) std::cout << g << " ";
    std::cout << "\n      NWW = ";
    for (const auto& l : emptyLCM) std::cout << l << " ";
    std::cout << "\n";
    
    std::cout << "    Lista 1-elementowa (" << num1 << "):\n";
    auto singleGCD = GaussianInteger::gcdListAll(singleList);
    auto singleLCM = GaussianInteger::lcmListAll(singleList);
    std::cout << "      NWD = ";
    for (const auto& g : singleGCD) std::cout << g << " ";
    std::cout << "\n      NWW = ";
    for (const auto& l : singleLCM) std::cout << l << " ";
    std::cout << "\n";
    
}

int main() {
    int a = 2, b = 7, c = 9, d = 7, e = 5, f = 2;
    
    try {
        runTests(a, b, c, d, e, f);
    } catch (const std::exception& ex) {
        std::cerr << "Błąd: " << ex.what() << std::endl;
    }
    
    return 0;
}