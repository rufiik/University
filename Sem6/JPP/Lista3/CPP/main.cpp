#include <iostream>
#include "rsa.hpp"

int main()
{
    constexpr std::size_t p = 10007;
    constexpr std::size_t q = 10009;
    constexpr std::size_t n = p * q;

    // Tworzenie obiektu RSA z Ring<n>
    RSA<Ring<n>> rsa{Ring<n>(p), Ring<n>(q)};

    // Wiadomość do zaszyfrowania
    Ring<n> message(12345);

    // Szyfrowanie i deszyfrowanie
    Ring<n> encrypted = rsa.encrypt(message);
    Ring<n> decrypted = rsa.decrypt(encrypted);

    // Wyświetlanie wyników
    std::cout << "Public Key: " << rsa.getPublicKey().get() << std::endl;
    std::cout << "Modulo: " << rsa.getModulo() << std::endl;
    std::cout << "Original: " << message.get() << std::endl;
    std::cout << "Encrypted: " << encrypted.get() << std::endl;
    std::cout << "Decrypted: " << decrypted.get() << std::endl;

    return 0;
}