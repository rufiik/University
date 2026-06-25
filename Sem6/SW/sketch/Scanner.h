#ifndef SCANNER_H
#define SCANNER_H

#include <Arduino.h>
#include <Servo.h>
#include "Dashboard.h"

class Scanner {
  private:
    int trigPin;
    int echoPin;
    int servoPin;
    Servo servo;
    Dashboard* dashboard; 

  public:
    Scanner();
    int currScannerAngle=90;
    int scannerChange=40;
    unsigned long lastChangeScanner=0;
    
    // Funkcja inicjalizująca piny
    void attach(int trig, int echo, int servo_pin);
    
    // Przekazanie wskaźnika do ekranu LCD
    void attachDashboard(Dashboard* dash);
    
    // Ustawia serwo prosto (90 stopni)
    void lookStraight();
    
    // Szybki pomiar odległości bez ruszania serwem (do używania w trakcie jazdy)
    unsigned int ping();
    
    // Obraca serwo pod podany kąt, mierzy odległość i wysyła dane na Dashboard
    unsigned int getDistance(byte angle);

    unsigned int activeScan();
};

#endif