import std.stdio;
import dh;
import user;
import ring;

void main() {
    enum n = 1234567891;
    
    auto setup = new DHSetup!(Ring!n)();
    auto user1 = new User!(Ring!n)(setup);
    auto user2 = new User!(Ring!n)(setup);

    auto user1PublicKey = user1.getPublicKey();
    auto user2PublicKey = user2.getPublicKey();

    user1.setKey(user2PublicKey);
    user2.setKey(user1PublicKey);

    auto message = Ring!n(12345);
    auto encrypted = user1.encrypt(message);
    auto decrypted = user2.decrypt(encrypted);

    writeln("Generator: ", setup.getGenerator());
    writeln("User1 Public Key: ", user1PublicKey);
    writeln("User2 Public Key: ", user2PublicKey);
    writeln("Original Message: ", message);
    writeln("Encrypted Message: ", encrypted);
    writeln("Decrypted Message: ", decrypted);
}