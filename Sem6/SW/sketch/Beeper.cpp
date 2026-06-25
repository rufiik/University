#include "Beeper.h"

Beeper::Beeper(int pin) {
    this->pin = pin;
    this->isBeeping = false;
    this->pinState = LOW;
    
    pinMode(this->pin, OUTPUT);
    digitalWrite(this->pin, LOW);
}

// Funkcja włączająca tryb pikania
void Beeper::beep(int speed) {
    this->isBeeping = true;
}

// Funkcja zatrzymująca pikanie i wymuszająca stan niski na pinie
void Beeper::stop() {
    this->isBeeping = false;
    this->pinState = LOW;
    digitalWrite(this->pin, LOW);
}

void Beeper::toggle() {
    if (this->isBeeping) {
        this->pinState = !this->pinState; 
        digitalWrite(this->pin, this->pinState);
    } else {
        if (this->pinState == HIGH) {
            this->pinState = LOW;
            digitalWrite(this->pin, LOW);
        }
    }
}

// Zwraca informację dla reszty programu
bool Beeper::isActive() {
    return this->isBeeping;
}