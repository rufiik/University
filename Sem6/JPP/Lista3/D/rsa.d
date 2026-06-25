module rsa;

import ring;

struct RSA(T) {
    private size_t modulo;
    private size_t phi;
    private T publicKey;
    private T privateKey;

    this(T p, T q) {
        modulo = p.get() * q.get();
        phi = (p.get() - 1) * (q.get() - 1);
        publicKey = choosePublicKey(phi);
        privateKey = calculatePrivateKey(publicKey.get(), phi);
    }

    size_t getModulo() const {
        return modulo;
    }

    T getPublicKey() const {
        return publicKey;
    }

    T encrypt(T m) const {
        return modExp(m, publicKey);
    }

    T decrypt(T s) const {
        return modExp(s, privateKey);
    }

    private:
    
     T choosePublicKey(size_t phi) {
        if (gcd(65537, phi) == 1)
            return T(65537);
        size_t e = 3;
        while (gcd(e, phi) != 1)
            e += 2;
        return T(e);
    }

    T calculatePrivateKey(size_t e, size_t phi) const {
        auto sol = solveLinearDiophantine(e, phi, 1);
        assert(sol.hasSolution, "Nie można znaleźć klucza prywatnego");
        return T((sol.x % phi + phi) % phi);
    }

    T modExp(T base, T exp) const {
        T result =T(1);
        T b = base;
        size_t e = exp.get();
        
        while (e > 0) {
            if (e & 1)
                result = result * b;
            b = b * b;
            e >>= 1;
        }
        return result;
    }
}