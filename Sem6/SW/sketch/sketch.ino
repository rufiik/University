#include "Wheels.h"
#include "Encoder.h"
#include "Scanner.h"
#include "Receiver.h"
#include <Servo.h>
byte LCDAddress = 0x27;
LiquidCrystal_I2C lcd(LCDAddress, 16, 2);

int EnA = 6;  //left
int EnB = 5;  //right

int In1 = 2;
int In2 = 4;
int In3 = 7;
int In4 = 8;
#define IR_BTN_1 0x45
#define IR_BTN_2 0x46
#define IR_BTN_3 0x47
#define IR_BTN_4 0x44
#define IR_BTN_5 0x40
#define IR_BTN_6 0x43
#define IR_BTN_7 0x07
#define IR_BTN_8 0x15
#define IR_BTN_9 0x09
#define IR_BTN_0 0x19
#define IR_BTN_STAR 0x16
#define IR_BTN_HASH 0x0D

#define IR_BTN_UP 0x18
#define IR_BTN_DOWN 0x52
#define IR_BTN_LEFT 0x08
#define IR_BTN_RIGHT 0x5A
#define IR_BTN_ENTER 0x1C
const uint32_t PIN[4] = { IR_BTN_2, IR_BTN_3, IR_BTN_4, IR_BTN_5 };

#define TRIG 9
#define ECHO 10
#define SERVO 11

volatile char cmd;
Dashboard dashboard(LCDAddress, 16, 2);
Wheels w;
Scanner scanner;
Beeper beeper(13);
RemoteReceiver remote(3);

enum RobotState { DRIVE,
                  SCAN,
                  TURN };
RobotState currentState = DRIVE;

void timerISR() {
  beeper.toggle();
}

void setup() {
  Serial.begin(9600);

  //kola
  w.attach(In1, In2, EnA, In3, In4, EnB);
  w.attachDashboard(&dashboard);
  w.attachBeeper(&beeper);

  //ekran
  dashboard.init();

  //przerwania i encoder
  Timer1.initialize(500000);
  Timer1.attachInterrupt(timerISR);
  Encoder::init();

  //scanner
  scanner.attach(TRIG, ECHO, SERVO);
  scanner.attachDashboard(&dashboard);
  //pilot
  remote.begin();
  // remote.checkPIN(PIN, IR_BTN_ENTER);
}

unsigned long lastIRTime = 0;
const unsigned long IR_TIMEOUT = 250;
uint16_t lastValidCommand = 0;

void handleMovement(uint16_t command) {
  switch (command) {
    case IR_BTN_UP:
      w.forward();
      break;
    case IR_BTN_DOWN:
      w.back();
      break;
    case IR_BTN_LEFT:
      w.backLeft();
      w.forwardRight();
      break;
    case IR_BTN_RIGHT:
      w.forwardLeft();
      w.backRight();
      break;
  }
}

void applyHookesLaw(int currentDist) {
    const int targetDist = 50; 
    float k = 7.3;        
    const int minPWM = 130;      
    int error = currentDist - targetDist; 
    // if(error < 30){
    //   k=2.5;
    // }
    // else if(error < 60){
    //   k=2.8;
    // }
    // else if(error < 90){
    //   k=3;
    // }
    int speed = abs(error) * k; 
    
    if (speed > 255) speed = 255;

    if (speed < minPWM) speed = minPWM;

    w.setSpeed(speed);
    if( abs(error) <= 3) {
      w.stop();
      return;
    }
    if (error > 0) {
        // Przeszkoda z daleko 
        w.forward();
    } else {
        // Przeszkoda za blisko 
        w.back();
    }
}
void loop() {
    w.update();

    scanner.lookStraight(); 
    int currentDistance = scanner.ping();

    if (currentDistance > 0 && currentDistance < 300) {
        applyHookesLaw(currentDistance);
    } else {
        w.stop();
    }

    delay(50); 
}


// void loop() {
//   w.update();

//   bool isRepeat = false;
//   uint16_t currentCmd = remote.getCommandRepeat(isRepeat);

//   if (currentCmd != 0 || isRepeat) {
//     if (!isRepeat) {
//       lastValidCommand = currentCmd;
//     }

//     if (lastValidCommand != 0) {
//       handleMovement(lastValidCommand);
//       lastIRTime = millis(); 
//     }
//   }

//   if (millis() - lastIRTime > IR_TIMEOUT) {
//     if (w.isBusy()) {
//       w.stop();
//       lastValidCommand = 0;
//     }
//   }
// }




// void loop() {
//   w.update();

// if (currentState == DRIVE) {
//     if (w.isBusy() == false) {
//       w.forward();
//     }

//     unsigned int dist = scanner.activeScan();

//     if (dist > 0 && dist < 25) {
//       w.stop();
//       currentState = SCAN; //przejscie w tryb skanowania
//     }
//   }
//   else if (currentState == SCAN) {

//     unsigned int distRight = scanner.getDistance(20);
//     delay(200);
//     unsigned int distLeft = scanner.getDistance(160);
//     delay(200);

//     if (distLeft > distRight) {
//       dashboard.setSonar(150, distLeft);
//       w.goTurnLeft(8, 180);
//     } else {
//       dashboard.setSonar(30, distRight);
//       w.goTurnRight(8, 180);
//     }

//     currentState = TURN; // Przejście do trybu skręcania
//   }
//   else if (currentState == TURN) {
//     if (w.isBusy() == false) {
//       currentState = DRIVE; // Wracamy do jazdy prosto
//     }
//   }
// }
// void loop() {
//   w.update();
//   if (Serial.available()) {
//     cmd = Serial.read();

//     switch(cmd) {
//       case 'w': w.forward(); break;
//       case 'x': w.back(); break;

//       case 'f': w.goForward(100); break;
//       case 'b': w.goBack(100); break;

//       case 's': w.stop(); break;
//       case 't': runSpeedTest(); break;
//       case 'r': w.goTurnRight(50, 130); break;
//       case 'l': w.goTurnLeft(50, 130); break;


//     }
//   }
// }

void runSpeedTest() {
  Serial.println("PWM;Predkosc_cm_s");

  for (int pwm = 100; pwm <= 255; pwm += 5) {
    w.setSpeed(pwm);
    Encoder::reset();
    uint32_t startT = millis();

    w.forward();
    delay(2000);
    // w.stop();

    uint32_t duration = millis() - startT;
    uint32_t pulses = Encoder::getLeft();

    float distance = pulses / w.getPulsesPerCm();
    float speed = distance / (duration / 1000.0);

    Serial.print(pwm);
    Serial.print(";");
    Serial.println(speed);

    delay(1000);
  }
}
