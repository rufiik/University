// main.d
import std.stdio;
import ring;
import rsa;

void main() {
    alias RingType = Ring!(10007 * 10009);
    
    auto rsa = RSA!RingType(RingType(10007), RingType(10009));
    
    auto message = RingType(12345);
    auto encrypted = rsa.encrypt(message);
    auto decrypted = rsa.decrypt(encrypted);
    
    writeln("Public Key: ", rsa.getPublicKey().get());
    writeln("Modulo: ", rsa.getModulo());
    writeln("Original: ", message.get());
    writeln("Encrypted: ", encrypted.get());
    writeln("Decrypted: ", decrypted.get());
}