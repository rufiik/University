#ifndef ENCODER_H
#define ENCODER_H

#include <Arduino.h>

class Encoder {
public:
    static void init();         
    static uint32_t getLeft(); 
    static uint32_t getRight(); 
    static void reset();        
};

#endif