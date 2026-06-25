module user;

import std.random;
import std.exception;
import dh;
import ring;

class User(T) {
    private T secret;
    private T publicKey;
    private T sharedKey;
    private bool keySet = false;
    private DHSetup!T setup;

    this(DHSetup!T dhSetup) {
        setup = dhSetup;
        secret = T(uniform(1, T.getModulo()));
        publicKey = setup.power(setup.getGenerator(), secret.get());
    }

    T getPublicKey() const {
        return publicKey;
    }

    void setKey(T otherPublicKey) {
        sharedKey = setup.power(otherPublicKey, secret.get());
        keySet = true;
    }

    T encrypt(T message) const {
        enforce(keySet, "Klucz szyfrujący nie został ustawiony!");
        return message * sharedKey;
    }

    T decrypt(T encryptedMessage) const {
        enforce(keySet, "Klucz szyfrujący nie został ustawiony!");
        return encryptedMessage / sharedKey;
    }
}