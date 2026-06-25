#ifndef Wheels_h
#define Wheels_h

#include <Arduino.h>
#include "Dashboard.h"
#include "Beeper.h"
#include "Encoder.h"

class Wheels {
    public: 
        void attachRight(int pinForward, int pinBack, int pinSpeed);
        void attachLeft(int pinForward, int pinBack, int pinSpeed);
        void attach(int pinRightForward, int pinRightBack, int pinRightSpeed,
                    int pinLeftForward, int pinLeftBack, int pinLeftSpeed);
        
        void attachDashboard(Dashboard* db);
        void attachBeeper(Beeper* beeper) { this->beeper = beeper; }
        
        // Ruchy ciągłe
        void forward();
        void forwardLeft();
        void forwardRight();
        void back();
        void backLeft();
        void backRight();
        void stop();
        void stopLeft();
        void stopRight();

        // Ruchy na dystans/kąt z prędkością
        void goForward(int cm, uint8_t speed = 110);
        void goBack(int cm, uint8_t speed = 110);
        void goTurnLeft(int cm, uint8_t speed = 110);
        void goTurnRight(int cm, uint8_t speed = 110);
        
        void update();
        
        void setSpeed(uint8_t);
        void setSpeedRight(uint8_t);
        void setSpeedLeft(uint8_t);

        bool isBusy() { return isMoving; }
        float getPulsesPerCm() { return pulsesPerCm; }

    private: 
        uint32_t targetUnits;
        int remainingCm = -1;
        const float pulsesPerCm = 2.5;             
        bool isMoving = false;           
        String currentDirection = "STOP"; 
        uint8_t currentSpeed = 200; // Przechowuje aktualnie ustawiony PWM
        int pinsRight[3];
        int pinsLeft[3];
        
        Dashboard* dashboard = nullptr; 
        Beeper* beeper = nullptr;
};

#endif