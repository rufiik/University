#include <Arduino.h>

#include "Wheels.h"
extern LiquidCrystal_I2C lcd;
void updateDashboard(int remainingCm, String state, int speed);
#define SET_MOVEMENT(side,f,b) digitalWrite( side[0], f);\
                               digitalWrite( side[1], b)


void Wheels::attachRight(int pF, int pB, int pS)
{
    pinMode(pF, OUTPUT);
    pinMode(pB, OUTPUT);
    pinMode(pS, OUTPUT);
    this->pinsRight[0] = pF;
    this->pinsRight[1] = pB;
    this->pinsRight[2] = pS;
}


void Wheels::attachLeft(int pF, int pB, int pS)
{
    pinMode(pF, OUTPUT);
    pinMode(pB, OUTPUT);
    pinMode(pS, OUTPUT);
    this->pinsLeft[0] = pF;
    this->pinsLeft[1] = pB;
    this->pinsLeft[2] = pS;
}

void Wheels::setSpeedRight(uint8_t s)
{
    analogWrite(this->pinsRight[2], s);
}

void Wheels::setSpeedLeft(uint8_t s)
{
    analogWrite(this->pinsLeft[2], s);
}

void Wheels::setSpeed(uint8_t s)
{
    this->currentSpeed = s; 
    setSpeedLeft(s);
    setSpeedRight(s);
}

void Wheels::attach(int pRF, int pRB, int pRS, int pLF, int pLB, int pLS)
{
    this->attachRight(pRF, pRB, pRS);
    this->attachLeft(pLF, pLB, pLS);
}

void Wheels::forwardLeft() {
    SET_MOVEMENT(pinsLeft, LOW, HIGH);
    this->isMoving = true;
    this->remainingCm = -1;
    this->currentDirection = "F_L";
}

void Wheels::forwardRight() {
    SET_MOVEMENT(pinsRight, LOW, HIGH);
    this->isMoving = true;
    this->remainingCm = -1;
    this->currentDirection = "F_R";
}

void Wheels::backLeft() {
    SET_MOVEMENT(pinsLeft, HIGH, LOW);
    this->isMoving = true;
    this->remainingCm = -1;
    this->currentDirection = "B_L";
}

void Wheels::backRight() {
    SET_MOVEMENT(pinsRight, HIGH, LOW);
    this->isMoving = true;
    this->remainingCm = -1;
    this->currentDirection = "B_R";
}

void Wheels::forward() {
    this->forwardLeft();
    this->forwardRight();
    this->setSpeed(this->currentSpeed);
    this->isMoving = true;
    this->remainingCm = -1;
    this->currentDirection = "FWD";
    
    if (this->dashboard) {
        this->dashboard->update(0, this->currentDirection, this->currentSpeed);
    }
}

void Wheels::back() {
    this->backLeft();
    this->backRight();
    this->setSpeed(this->currentSpeed);
    this->isMoving = true;
    this->remainingCm = -1;
    this->currentDirection = "BCK";
    if (this->beeper) {
        this->beeper->beep(110); 
    }
    if (this->dashboard) {
        this->dashboard->update(0, this->currentDirection, this->currentSpeed);
    }
}

void Wheels::stopLeft()
{
    SET_MOVEMENT(pinsLeft, LOW, LOW);
}

void Wheels::stopRight()
{
    SET_MOVEMENT(pinsRight, LOW, LOW);
}

void Wheels::stop() {
    this->stopLeft();
    this->stopRight();
    this->isMoving = false;
    this->remainingCm = 0; 
    this->currentDirection = "STOP";
    if (this->beeper) {
        this->beeper->stop();
    }
    if (this->dashboard) {
        this->dashboard->update(0, "STOP", 0);
    }
}

void Wheels::attachDashboard(Dashboard* db) {
    this->dashboard = db;
}

void Wheels::goForward(int cm, uint8_t speed) {
    this->currentSpeed = speed;
    this->setSpeed(speed);    
    Encoder::reset(); 
    this->targetUnits = (uint32_t)(cm * this->pulsesPerCm); 
    this->forward(); 
    this->remainingCm = cm;
    this->isMoving = true;
}

void Wheels::goBack(int cm, uint8_t speed) {
    this->currentSpeed = speed;
    this->setSpeed(speed);     
    Encoder::reset(); 
    this->targetUnits = (uint32_t)(cm * this->pulsesPerCm); 
    this->back();
    this->remainingCm = cm;
    this->isMoving = true;
}

void Wheels::goTurnLeft(int cm, uint8_t speed) {
    this->currentSpeed = speed;
    this->setSpeed(speed);
    Encoder::reset();
    this->targetUnits = (uint32_t)(cm * this->pulsesPerCm);
    
    this->backLeft();    
    this->forwardRight();
    
    this->remainingCm = cm; 
    this->currentDirection = "TRN_L"; 
    this->isMoving = true;
}

void Wheels::goTurnRight(int cm, uint8_t speed) {
    this->currentSpeed = speed; // Zapamiętaj prędkość dla dashboardu
    this->setSpeed(speed);
    Encoder::reset();
    this->targetUnits = (uint32_t)(cm * this->pulsesPerCm);
    
    // Skręt w prawo: lewe koło do przodu, prawe do tyłu
    this->forwardLeft(); 
    this->backRight();   
    
    this->remainingCm = cm;
    this->currentDirection = "TRN_R";
    this->isMoving = true;
}


void Wheels::update() {
    if (!isMoving) return;
    if (this->remainingCm == -1) {
            if (this->dashboard) {
                this->dashboard->update(-1, currentDirection, this->currentSpeed);
            }
            return; 
        }
    uint32_t currentL = Encoder::getLeft();
    uint32_t currentR = Encoder::getRight();
    uint32_t avg = (currentL+currentR)/2;
    int remainingPulses = (targetUnits > avg) ? (targetUnits - avg) : 0;

    int cm = (int)(remainingPulses / this->pulsesPerCm);

    if (this->dashboard) {
            this->dashboard->update(cm, currentDirection, this->currentSpeed);
        }

    if (avg >= targetUnits) {
        this->stop();
        this->isMoving = false; 
        
        if (this->dashboard) {
            this->dashboard->update(0, "STOP", 0);
        }
    }
}

