#ifndef BEEPER_H
#define BEEPER_H

#include <Arduino.h>
#include <TimerOne.h>
class Beeper {
private:
    int pin;          
    bool isBeeping;   
    bool pinState;    

public:
    Beeper(int pin);

    void beep(int speed);

    void stop();

    void toggle();
    
    bool isActive();
};

#endif