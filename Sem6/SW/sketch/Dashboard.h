#ifndef Dashboard_h
#define Dashboard_h

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

class Dashboard {
  public:
    // Konstruktor przyjmujący adres I2C i wymiary wyświetlacza
    Dashboard(uint8_t address, uint8_t cols, uint8_t rows);
    
    // Inicjalizacja wyświetlacza (wywoływana w setup)
    void init();
    
    // Główna funkcja aktualizująca ekran
    void update(int remainingCm, String state, int speed);
    int sonarDist = 0;
    int sonarAngle = 90;
    void setSonar(int angle, int distance);

  private:
    LiquidCrystal_I2C lcd;
    
    static byte arrowRight[8];
    static byte arrowLeft[8];
};

#endif