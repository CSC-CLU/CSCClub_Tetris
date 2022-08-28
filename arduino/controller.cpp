/**
 * @file Controller.cpp
 * @author Eric Heinke
 * @date  April 20 2022
 * @brief A class for interfacing with an Arduino controller
 */

#include <iostream>
#include <bitset>
#include <string>
#include <sstream>
#include "controller.h"

using namespace arduino;
using namespace std;

Controller::Controller(char serialPort[])
    : serialPort(serialPort)
    , statusCodes(false)
    , serial() {
    // Connection to serial port
    char statusCode = serial.openDevice(this->serialPort, 57600, SERIAL_DATABITS_8, SERIAL_PARITY_NONE, SERIAL_STOPBITS_1);

    connected = statusCodes.openDevice(statusCode);
    if(!connected) cout << "WARNING: Arduino not found on port " << serialPort << endl;

    statusCode = serial.flushReceiver();

    // Display status code message
    statusCodes.flushReceiver(statusCode);
}

Controller::~Controller()
{

}

void Controller::refreshArduinoStatus()
{
    unsigned char statusCode = serial.writeString("CT\n");
    char bytes[100];
    statusCode = serial.readString(bytes, '\n', 100, 100);
    // fixme should populate the controller with default values if nothing is read.
    if (statusCode > 0)
        decodeInputs(bytes);
    else cout << "Communication Error (CT), got" << statusCode << endl;
}

void Controller::decodeInputs(char* bytes)
{
    this->buttons = ~(((bytes[2] & 0b00000011) << 6) | ((bytes[5] & 0b00000011) << 4) |
                      ((bytes[8] & 0b00000011) << 2) | (bytes[11] & 0b00000011));
    this->moveLeft =    this->buttons & 0b10000000;
    this->moveRight =   this->buttons & 0b00100000;
    this->fastDrop =    this->buttons & 0b01000000;
    this->instantDrop = this->buttons & 0b00010000;
    this->rotateLeft =  this->buttons & 0b00000010;
    this->rotateRight = this->buttons & 0b00000001;
    this->startButton = this->buttons & 0b00001000;
    this->selectButton =this->buttons & 0b00000100;
    this->joystick1X = ((bytes[0] & 0b00111111) << 2) | ((bytes[1] & 0b00110000) >> 4);
    this->joystick1Y = ((bytes[1] & 0b00001111) << 4) | ((bytes[2] & 0b00111100) >> 2);
    this->joystickB1 = !((bytes[13] & 0b00001000) >> 3);
    this->joystick2X = ((bytes[3] & 0b00111111) << 2) | ((bytes[4] & 0b00110000) >> 4);
    this->joystick2Y = ((bytes[4] & 0b00001111) << 4) | ((bytes[5] & 0b00111100) >> 2);
    this->joystickB2 = !((bytes[13] & 0b00000100) >> 2);
    nunchuck = {
            ((bytes[6] & 0b00111111) << 2) | ((bytes[7] & 0b00110000) >> 4),
            ((bytes[7] & 0b00001111) << 4) | ((bytes[8] & 0b00111100) >> 2),
            static_cast<bool>(((bytes[13] & 0b00000010) >> 1)),
            static_cast<bool>((bytes[13] & 0b00000001)),
            {
                    ((bytes[9] & 0b00111111) << 2) | ((bytes[10] & 0b00110000) >> 4),
                    ((bytes[10] & 0b00001111) << 4) | ((bytes[11] & 0b00111100) >> 2),
                    ((bytes[12] & 0b00111111) << 2) | ((bytes[13] & 0b00110000) >> 4),
                    ((bytes[14] & 0b00111111) << 2) | ((bytes[15] & 0b00110000) >> 4),
                    ((bytes[15] & 0b00001111) << 4) | ((bytes[16] & 0b00111100) >> 2)
            }
    };
    this->unused1 = ((bytes[16] & 0b00000010) >> 1);
    this->unused2 = (bytes[16] & 0b00000001);

    cout<<"Buttons: "<<bitset<8>(buttons).to_string()<<endl;
    cout<<"Joystick 1: ("<<to_string(joystick1X)<<", "<<to_string(joystick1Y)<<") Button: "<<to_string(joystickB1)<<endl;
    cout<<"Joystick 2: ("<<to_string(joystick2X)<<", "<<to_string(joystick2Y)<<") Button: "<<to_string(joystickB2)<<endl;
    cout << "Joystick N: (" << to_string(nunchuck.joyX) << ", " << to_string(nunchuck.joyY) << ")";
    cout<<" Button C: "<<to_string(nunchuck.btnC)<<" Button Z: "<<to_string(nunchuck.btnZ)<<endl;
    cout<<"Accelerometer A: ("<<to_string(nunchuck.accel.AX)<<", "<<to_string(nunchuck.accel.AY)<<", "<<to_string(nunchuck.accel.AZ)<<")"<<endl;
    cout<<"Rotations (P,R): ("<<to_string(nunchuck.accel.pitch)<<", "<<to_string(nunchuck.accel.roll)<<")"<<endl;
    // cout<<bytes<<endl;
}

void Controller::enableNunchuck()
{
    nunchuckEnabled = true;
    char statusCode = serial.writeString("CFNCE\n");
}

void Controller::disableNunchuck()
{
    nunchuckEnabled = false;
    char statusCode = serial.writeString("CFNCD\n");
}

void Controller::setKeyLights(int R, int G, int B)
{
    stringstream buffer;
    char command[10] = {'U', 'L', 'K', 0, 0, 0, 0, 0, 0, '\n'};
    if (R < 16)
        buffer<<'0';
    buffer<<hex<<R;
    if (G < 16)
        buffer<<'0';
    buffer<<hex<<G;
    if (B < 16)
        buffer<<'0';
    buffer<<hex<<B;
    buffer>>command[3];
    buffer>>command[4];
    buffer>>command[5];
    buffer>>command[6];
    buffer>>command[7];
    buffer>>command[8];
    char statusCode = serial.writeString(command);
}

void Controller::setTowerLights(char left, char right)
{
    setTowerLights(
            (left & 0b00001000) != 0,
            (left & 0b00000100) != 0,
            (left & 0b00000010) != 0,
            (left & 0b00000001) != 0,
            (right & 0b00001000) != 0,
            (right & 0b00000100) != 0,
            (right & 0b00000010) != 0,
            (right & 0b00000001) != 0
    );
}

void Controller::setTowerLights(bool LR, bool LG, bool LB, bool LZ, bool RR, bool RG, bool RB, bool RZ)
{
    stringstream buffer;
    char command[10] = {'U', 'L', 'T', '0', '0', '0', '0', '0', '0', '\n'};
    int tmp = 0;
    if (LR)
        tmp |= 0b10000000;
    if (LG)
        tmp |= 0b01000000;
    if (LB)
        tmp |= 0b00100000;
    if (LZ)
        tmp |= 0b00010000;
    if (RR)
        tmp |= 0b00001000;
    if (RG)
        tmp |= 0b00000100;
    if (RB)
        tmp |= 0b00000010;
    if (RZ)
        tmp |= 0b00000001;

    if (tmp < 16)
        buffer<<'0';
    buffer<<hex<<tmp;
    buffer>>command[3];
    buffer>>command[4];
    char statusCode = serial.writeString(command);
}

void Controller::playAnimation(Animation animation)
{
    stringstream buffer;
    char command[4] = {'A', 'N', 0, '\n'};
    buffer<<to_string((int)animation);
    buffer>>command[2];
    char statusCode = serial.writeString(command);
}
