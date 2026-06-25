#include <iostream>
#include "ring.hpp"
#include "dh.hpp"
#include "user.hpp"

int main() {
    constexpr std::size_t n = 1234567891;
    
    DHSetup<Ring<n>> setup;
    User<Ring<n>> user1(setup);
    User<Ring<n>> user2(setup);

    auto user1PublicKey = user1.getPublicKey();
    auto user2PublicKey = user2.getPublicKey();

    user1.setKey(user2PublicKey);
    user2.setKey(user1PublicKey);

    Ring<n> message(12345);
    Ring<n> encrypted = user1.encrypt(message);
    Ring<n> decrypted = user2.decrypt(encrypted);

    std::cout << "Generator: " << setup.getGenerator() << std::endl;
    std::cout << "User1 Public Key: " << user1PublicKey << std::endl;
    std::cout << "User2 Public Key: " << user2PublicKey << std::endl;
    std::cout << "Original Message: " << message << std::endl;
    std::cout << "Encrypted Message: " << encrypted << std::endl;
    std::cout << "Decrypted Message: " << decrypted << std::endl;

    return 0;
}