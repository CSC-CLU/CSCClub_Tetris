/**
 * @file Controller.h
 * @author Eric Heinke
 * @date  April 20 2022
 * @brief A class for interfacing with an Arduino controller
 */

#ifndef ARDUINO_CONTROLLER_H
#define ARDUINO_CONTROLLER_H

// Serial library
#include "lib/serialib.h"
// Serial library status code decoding
#include "DecodeStatusCodes.h"
#include "../Color.h"

namespace arduino {

    constexpr auto EXPECTED_RESPONSE_TIME = 6;

    class Controller {
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
        struct {
            int joyX;
            int joyY;
            bool btnC;
            bool btnZ;
            struct { int AX,AY,AZ,pitch,roll; } accel;
        } nunchuck;
        bool unused1;
        bool unused2;

        // disregard inputs if not connected.
        bool connected;
        bool nunchuckEnabled;

        struct {
            bool green=false, yellow=false, red=false, buzzer=false;
        } towerLights; // both are currently aligned.

    public:
        explicit Controller(char serialPort[]=SERIAL_PORT);
        virtual ~Controller();
        void refreshArduinoStatus();

        void enableNunchuck();
        void disableNunchuck();
        void setKeyLights(Color color) { setKeyLights(color.r,color.g,color.b); };
        void setKeyLights(int R, int G, int B);
        void setTowerLights(char left, char right);
        void updateTowerLights() { setTowerLights(towerLights.green,towerLights.yellow,towerLights.red,towerLights.buzzer); }
        void setTowerLights(bool G, bool Y, bool R, bool Z)
        { setTowerLights(
                towerLights.green = G,towerLights.yellow = Y,towerLights.red = R,towerLights.buzzer = Z,
                G,Y,R,Z);
        }
        void setTowerLights(bool LR, bool LG, bool LB, bool LZ, bool RR, bool RG, bool RB, bool RZ);
        enum class Animation {TOWER_LIGHT=1, FLATLINE=2};
        void playAnimation(Animation);
    private:
        char* serialPort;
        void decodeInputs(char *bytes);
    };
}
#endif //ARDUINO_CONTROLLER_H