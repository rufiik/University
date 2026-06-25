#include "Encoder.h"
volatile uint32_t countL = 0;
volatile uint32_t countR = 0;

void Encoder::init() {
    pinMode(A0, INPUT);
    pinMode(A1, INPUT);
    
    PCICR |= (1 << PCIE1);    
    PCMSK1 |= (1 << PCINT8) | (1 << PCINT9); 
}

void Encoder::reset() {
    noInterrupts(); 
    countL = 0;
    countR = 0;
    interrupts();
}

uint32_t Encoder::getLeft() { return countL; }
uint32_t Encoder::getRight() { return countR; }

ISR(PCINT1_vect) {
    if (PINC & (1 << PC0)) countL++;
    if (PINC & (1 << PC1)) countR++;
}