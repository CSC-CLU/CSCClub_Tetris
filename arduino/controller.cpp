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

// controller buffer logic
struct BufferedCommand {
    string command;
    bool interrupt;
    BufferedCommand *next=nullptr;
} *headCmd,*tailCmd; // start and end of list, respectively
void queueCommand(string command, bool interrupt=false) {
    auto bc = new BufferedCommand({command,interrupt});
    if(headCmd == nullptr) { // the queue is empty
        headCmd = tailCmd = bc;
    }
    else {
        bc->next = tailCmd;
        tailCmd = bc;
    }
}

bool runningCommand = false, commandInterrupts = false;
int interrupts=0;
bool Controller::processing() { return runningCommand; }
bool Controller::interrupting() { return interrupts > 0; }
void Controller::respondToArduino() {
    if(headCmd == nullptr) return;
    char code = serial.writeString(headCmd->command.c_str());
    runningCommand = code == 1; // success on 1
    if(runningCommand) {
        // pop next command
        commandInterrupts = headCmd->interrupt;
        auto next = headCmd->next;
        delete headCmd;
        headCmd = next;
        // todo is last command handled properly?
    }
}

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

// now also sends commands after receiving.
bool Controller::refreshArduinoStatus()
{
    // todo wonder if this should be split up.
    unsigned char statusCode = serial.writeString("CT\n");
    char bytes[100];
    statusCode = serial.readString(bytes, '\n', 100, EXPECTED_RESPONSE_TIME);
    // fixme should populate the controller with default values if nothing is read.
    if (statusCode > 0) {
        runningCommand = false; // if it responded it's not running a command.
        if(commandInterrupts) {
            commandInterrupts = false;
            interrupts--;
        }
        decodeInputs(bytes);
        // send new command if applicable.
        respondToArduino();
        return true;
    }
    else cout << "Communication Error (CT), got" << statusCode << endl;
    return false;
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

void Controller::toggleNunchuck() {
    string command = "CFNC \n";
    command[4] = 'D' + nunchuckEnabled;
    nunchuckEnabled = !nunchuckEnabled;
    queueCommand(command);
}

void Controller::toggleNunchuck(bool enable) {
    nunchuckEnabled = !enable;
    toggleNunchuck();
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
    for(int i = 3; i < 9; i++) buffer>>command[i];
    queueCommand(command);
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
    queueCommand(command);
}

void Controller::playAnimation(Animation animation)
{
    // this bricks the arduino until the animation is complete, so we need to wait for it to be complete.
    queueCommand({'A', 'N', (char)animation, '\n'}, true);
}
