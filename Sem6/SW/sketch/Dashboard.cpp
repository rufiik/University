#include "Dashboard.h"

// Inicjalizacja znaków specjalnych
byte Dashboard::arrowRight[8] = {0b01000, 0b01100, 0b00110, 0b11111, 0b11111, 0b00110, 0b01100, 0b01000};
byte Dashboard::arrowLeft[8]  = {0b00010, 0b00110, 0b01100, 0b11111, 0b11111, 0b01100, 0b00110, 0b00010};

Dashboard::Dashboard(uint8_t address, uint8_t cols, uint8_t rows) : lcd(address, cols, rows) {
}

void Dashboard::setSonar(int angle, int distance){
    this->sonarAngle = angle;
    this->sonarDist = distance;
}

void Dashboard::init() {
    lcd.init();
    lcd.backlight();
    lcd.createChar(0, arrowRight);
    lcd.createChar(1, arrowLeft);
    
    lcd.setCursor(0, 0);
    lcd.print("Auto Gotowe!");
    delay(1000);
    lcd.clear();
}

void Dashboard::update(int remainingCm, String state, int speed) {
    lcd.setCursor(0, 0);
    lcd.print("D:");
    
    if(remainingCm == -1) {
        lcd.print("--  ");
    } else {
        lcd.print(remainingCm);
        //czyszczenie jesli mniejsze
        if (remainingCm < 10) lcd.print("   ");
        else if (remainingCm < 100) lcd.print("  ");
        else lcd.print(" ");
    }
    // odleglosc
    lcd.setCursor(7, 0); 
    lcd.print("S:");
    if (sonarDist >= 400) {
        lcd.print("MAX");
    } else {
        lcd.print(sonarDist);
        if(sonarDist < 10) lcd.print("  ");
        else if(sonarDist < 100) lcd.print(" ");
    }
    
    //kat serwa
    lcd.print("/");
    lcd.print(sonarAngle);
    if(sonarAngle < 10) lcd.print("  ");
    else if(sonarAngle < 100) lcd.print(" ");


    lcd.setCursor(0, 1);
    
    if (state == "FWD") {
        lcd.print("F  "); 
        if ((millis() / 750) % 2 == 0) lcd.write(0); else lcd.print(" ");
        lcd.print("      ");
        lcd.setCursor(11, 1);
        lcd.print("V:"); lcd.print(speed);
        if (speed < 100) lcd.print(" ");
    } 
    else if (state == "BCK") {
        lcd.print("B  ");
        if ((millis() / 750) % 2 == 0) lcd.write(1); else lcd.print(" ");
        lcd.print("      "); 
        lcd.setCursor(10, 1);
        lcd.print("V:"); lcd.print(-speed); 
        if (-speed < 100) lcd.print(" ");
    } 
    else if (state == "TRN_L") {
        lcd.print("L  ");
        if ((millis() / 750) % 2 == 0) lcd.write(1); else lcd.print(" ");
        lcd.print("     ");
        lcd.setCursor(11, 1);
        lcd.print("V:"); lcd.print(speed);
    }
    else if (state == "TRN_R") {
        lcd.print("R  ");
        if ((millis() / 750) % 2 == 0) lcd.write(0); else lcd.print(" ");
        lcd.print("    ");
        lcd.setCursor(11, 1);
        lcd.print("V:"); lcd.print(speed);
    }
    else {
        lcd.print("STOP            ");
    }
}