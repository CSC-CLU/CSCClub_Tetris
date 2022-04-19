//
// Created by Zrp200 on 4/19/2022.
//

#ifndef CSCCLUB_TETRIS_CONTROLLER_H
#define CSCCLUB_TETRIS_CONTROLLER_H

// Serial library status code decoding
#include "DecodeStatusCodes.h"

namespace arduino {
#include "lib/serialib.h"
    class ArduinoController
    {
    public:
        serialib serial;
        DecodeStatusCodes statusCodes;
        char buttons;
        bool moveLeft;
        bool moveRight;
        bool fastDrop;
        bool instantDrop;
        bool rotateLeft;
        bool rotateRight;
        bool startButton;
        bool selectButton;
        int joystick1X;
        int joystick1Y;
        bool joystickB1;
        int joystick2X;
        int joystick2Y;
        bool joystickB2;
        int joystickSX;
        int joystickSY;
        bool buttonC;
        bool buttonZ;
        int accelerometerAX;
        int accelerometerAY;
        int accelerometerAZ;
        int accelerometerAP;
        int accelerometerAR;
        bool unused1;
        bool unused2;
    public:
        ArduinoController();
        ArduinoController(char* serialPort);
        virtual ~ArduinoController();
        void refreshArduinoStatus();
        void enableNunchuck();
        void disableNunchuck();
        void setKeyLights(int R, int G, int B);
        void setTowerLights(char left, char right);
        void setTowerLights(bool LR, bool LG, bool LB, bool LZ, bool RR, bool RG, bool RB, bool RZ);
        void playAnimation(int animationNumber);
    private:
        char* serialPort;
    };
}

#endif //CSCCLUB_TETRIS_CONTROLLER_H
