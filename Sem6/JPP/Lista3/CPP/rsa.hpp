#ifndef RSA_HPP
#define RSA_HPP

#include "ring.hpp"

template <typename T>
class RSA
{
public:
    RSA(T p, T q)
        : modulo(p.get() * q.get()), phi((p.get() - 1) * (q.get() - 1)),
          publicKey(choosePublicKey(phi)),
          privateKey(calculatePrivateKey(publicKey.get(), phi)) {}

    T encrypt(const T& m) const
    {
        T result(1);
        T base = m;
        std::size_t exp = publicKey.get(); 

        while (exp > 0)
        {
            if (exp % 2 == 1)
            {
                result *= base;
            }
            base *= base;
            exp /= 2;
        }

        return result;
    }

    T decrypt(const T& s) const
    {
        T result(1);
        T base = s;
        std::size_t exp = privateKey.get(); 

        while (exp > 0)
        {
            if (exp % 2 == 1)
            {
                result *= base;
            }
            base *= base;
            exp /= 2;
        }

        return result;
    }

    unsigned getModulo() const
    {
        return modulo;
    }

    T getPublicKey() const
    {
        return publicKey;
    }

private:
    std::size_t modulo;
    std::size_t phi;
    T publicKey;
    T privateKey;

    T choosePublicKey(std::size_t phi)
    {
        std::size_t best = 65537;
        if (gcd(best, phi) == 1)
        {
            return T(best);
        }
        std::size_t e = 3;
        while (gcd(e, phi) != 1)
        {
            e += 2;
        }
        return T(e);
    }

    T calculatePrivateKey(std::size_t e, std::size_t phi)
    {
        auto solution = solve_linear_diophantine(e, phi, 1);
        if (!solution.has_solution)
        {
            throw std::runtime_error("Nie można znaleźć klucza prywatnego");
        }
        return T((solution.x % phi + phi) % phi); 
    }
};

#endif // RSA_HPP