#include "Scanner.h"

Scanner::Scanner() {
    dashboard = nullptr;
}

void Scanner::attach(int trig, int echo, int servo_pin) {
    trigPin = trig;
    echoPin = echo;
    servoPin = servo_pin;

    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);

    servo.attach(servoPin);
    lookStraight(); 
}

void Scanner::attachDashboard(Dashboard* dash) {
    dashboard = dash;
}

void Scanner::lookStraight() {
    servo.write(90);
    if (dashboard != nullptr) {
        dashboard->setSonar(90, ping());
    }
}

unsigned int Scanner::ping() {
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10); // 10 mikrosekund impulsu dla HC-SR04
    digitalWrite(trigPin, LOW);
    
    unsigned long tot = pulseIn(echoPin, HIGH, 30000); 
    unsigned long tot2 = pulseIn(echoPin, HIGH, 30000);
    unsigned long tot3 = pulseIn(echoPin, HIGH, 30000);
    unsigned long avg = (tot+tot2+tot3)/3;
    
    //  Serial.print(tot);
    //  Serial.print("\n");
    if (avg == 0) return 400; 
    
    return avg / 58; // Przeliczenie czasu na centymetry
}

unsigned int Scanner::getDistance(byte angle) {
    servo.write(angle);
    delay(400); //obrot
    
    unsigned int distance = ping();
    
    if (dashboard != nullptr) {
        dashboard->setSonar(angle, distance);
    }
    
    // debug
    // Serial.print("Kat: "); Serial.print(angle);
    // Serial.print(" | Odleglosc: "); Serial.println(distance);

    return distance;
}

unsigned int Scanner::activeScan(){
    if (millis()-lastChangeScanner> 100){
        currScannerAngle+=scannerChange;
   
    if(currScannerAngle>160 || currScannerAngle<20){
        scannerChange=-scannerChange;
    }
    servo.write(currScannerAngle);
    lastChangeScanner= millis();
    int dist = ping();
    if(dashboard!=nullptr){
        dashboard->setSonar(currScannerAngle,dist);
    }
    return dist;
 }
 return 400;
}