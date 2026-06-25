#ifndef USER_HPP
#define USER_HPP

#include <stdexcept>
#include <random>
#include "dh.hpp"

template <typename T>
class User {
private:
    T secret;
    T publicKey;
    T sharedKey;
    bool keySet;
    DHSetup<T>& setup;

    std::size_t randomInRange(std::size_t min, std::size_t max) {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<std::size_t> dist(min, max);
        return dist(gen);
    }

public:
    User(DHSetup<T>& dhSetup) : setup(dhSetup), keySet(false) {
        secret = T(randomInRange(1, T::getModulo() - 1));
        publicKey = setup.power(setup.getGenerator(), secret.get());
    }

    T getPublicKey() const {
        return publicKey;
    }

    void setKey(T a) {
        sharedKey = setup.power(a, secret.get());
        keySet = true;
    }

    T encrypt(T m) const {
        if (!keySet) {
            throw std::logic_error("Klucz szyfrujący nie został ustawiony!");
        }
        return m * sharedKey;
    }

    T decrypt(T c) const {
        if (!keySet) {
            throw std::logic_error("Klucz szyfrujący nie został ustawiony!");
        }
        return c / sharedKey;
    }
};

#endif