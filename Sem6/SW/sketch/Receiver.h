#ifndef RECEIVER_H
#define RECEIVER_H

#include <Arduino.h>


class RemoteReceiver {
public:
    RemoteReceiver(int pin);

    void begin();

    uint16_t getCommand();
    uint16_t getCommandRepeat(bool &isRepeat);

    void debugPrint();

    bool checkPIN(const uint32_t correctPIN[4], uint32_t enterKey);

private:
    int _irPin;
};

#endif