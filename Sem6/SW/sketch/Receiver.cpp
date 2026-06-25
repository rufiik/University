#include "Receiver.h"
#include <IRremote.h>
RemoteReceiver::RemoteReceiver(int pin) {
    _irPin = pin;
}

void RemoteReceiver::begin() {
    IrReceiver.begin(_irPin, ENABLE_LED_FEEDBACK);
    // Serial.print(F("Odbiornik IR zainicjalizowany na pinie: "));
    // Serial.println(_irPin);
}

uint16_t RemoteReceiver::getCommand() {
    if (IrReceiver.decode()) {
        if (IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT) {
            IrReceiver.resume();
            return 0;
        }

        uint16_t cmd = IrReceiver.decodedIRData.command;
        IrReceiver.resume();
        
        return (cmd == 0) ? 0 : cmd;
    }
    return 0; 
}

uint16_t RemoteReceiver::getCommandRepeat(bool &isRepeat) {
    if (IrReceiver.decode()) {
        uint16_t cmd = IrReceiver.decodedIRData.command;
        isRepeat = (IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT);
        
        IrReceiver.resume();
        
        if (cmd == 0 && !isRepeat) return 0;
        return cmd;
    }
    return 0;
}

void RemoteReceiver::debugPrint() {
    if (IrReceiver.decode()) {
        Serial.print(F("Odebrano kod: 0x"));
        Serial.println(IrReceiver.decodedIRData.command, HEX);
    }
}

bool RemoteReceiver::checkPIN(const uint32_t correctPIN[4], uint32_t enterKey) {
    uint32_t enteredDigits[4] = {0, 0, 0, 0};
    int count = 0;
    
    Serial.println(F("SYSTEM ZABLOKOWANY. WPISZ 4 CYFRY I NACISNIJ ENTER."));
    
    while (true) {
        uint32_t cmd = getCommand(); 
        if (cmd != 0) {
            Serial.print(F("Odebrano: 0x"));
            Serial.println(cmd, HEX);

            if (cmd == enterKey) {
                if (count == 4) {
                    bool match = true;
                    for (int i = 0; i < 4; i++) {
                        if (enteredDigits[i] != correctPIN[i]) match = false;
                    }
                    if (match) {
                        Serial.println(F(">>> PIN OK! <<<"));
                        return true;
                    }
                }
                Serial.println(F(">>> BŁĄD PINU! Spróbuj ponownie. <<<"));
                count = 0; 
            } 
            else if (count < 4) {
                enteredDigits[count] = cmd;
                count++;
                Serial.print(F("Cyfra ")); 
                Serial.print(count);
                Serial.println(F("/4 zapisana."));
            }
            
            delay(300); 
        }
        delay(10); 
    }
}