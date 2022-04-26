/**
 * @file /SerialCommunications_Testing.cpp
 * @author Eric Heinke
 * @date April 21 2022
 * @brief File containing a test of serial port communication
 */

#include <iostream>
#include <bitset>
#include <string>
#include <sstream>

// Serial library
#include "lib/serialib.h"
// Serial library status code decoding
#include "../DecodeStatusCodes.h"

#include <chrono>
using namespace std::chrono;

#if defined (_WIN32) || defined(_WIN64)
    #include<windows.h>
#endif
#if defined (__linux__) || defined(__APPLE__)
    #include<unistd.h>
#endif

#if defined (_WIN32) || defined(_WIN64)
    //for serial ports above "COM9", we must use this extended syntax of "\\.\COMx".
    //also works for COM0 to COM9.
    #define SERIAL_PORT "\\\\.\\COM4"
#endif
#if defined (__linux__) || defined(__APPLE__)
    #define SERIAL_PORT "/dev/ttyACM0"
#endif

using namespace std;

void decodeInputs(char* bytes)
{
    // for (int i = 0; i < 14; ++i)
    // {
    //     cout<<"byte"<<to_string(i)<<": "<<bitset<8>(bytes[i]).to_string()<<endl;
    // }

    char buttons = ~(((bytes[2] & 0b00000011) << 6) | ((bytes[5] & 0b00000011) << 4) | 
                    ((bytes[8] & 0b00000011) << 2) | (bytes[11] & 0b00000011));
    int joystick1X = ((bytes[0] & 0b00111111) << 2) | ((bytes[1] & 0b00110000) >> 4);
    int joystick1Y = ((bytes[1] & 0b00001111) << 4) | ((bytes[2] & 0b00111100) >> 2);
    bool joystickB1 = !((bytes[13] & 0b00001000) >> 3);
    int joystick2X = ((bytes[3] & 0b00111111) << 2) | ((bytes[4] & 0b00110000) >> 4);
    int joystick2Y = ((bytes[4] & 0b00001111) << 4) | ((bytes[5] & 0b00111100) >> 2);
    bool joystickB2 = !((bytes[13] & 0b00000100) >> 2);
    int joystickSX = ((bytes[6] & 0b00111111) << 2) | ((bytes[7] & 0b00110000) >> 4);
    int joystickSY = ((bytes[7] & 0b00001111) << 4) | ((bytes[8] & 0b00111100) >> 2);
    bool buttonC = ((bytes[13] & 0b00000010) >> 1);
    bool buttonZ = (bytes[13] & 0b00000001);
    int accelerometerAX = ((bytes[9] & 0b00111111) << 2) | ((bytes[10] & 0b00110000) >> 4);
    int accelerometerAY = ((bytes[10] & 0b00001111) << 4) | ((bytes[11] & 0b00111100) >> 2);
    int accelerometerAZ = ((bytes[12] & 0b00111111) << 2) | ((bytes[13] & 0b00110000) >> 4);
    int accelerometerAP = ((bytes[14] & 0b00111111) << 2) | ((bytes[15] & 0b00110000) >> 4);
    int accelerometerAR = ((bytes[15] & 0b00001111) << 4) | ((bytes[16] & 0b00111100) >> 2);
    bool unused1 = ((bytes[16] & 0b00000010) >> 1);
    bool unused2 = (bytes[16] & 0b00000001);

    cout<<"Buttons: "<<bitset<8>(buttons).to_string()<<endl;
    cout<<"Joystick 1: ("<<to_string(joystick1X)<<", "<<to_string(joystick1Y)<<") Button: "<<to_string(joystickB1)<<endl;
    cout<<"Joystick 2: ("<<to_string(joystick2X)<<", "<<to_string(joystick2Y)<<") Button: "<<to_string(joystickB2)<<endl;
    cout<<"Joystick N: ("<<to_string(joystickSX)<<", "<<to_string(joystickSY)<<")";
    cout<<" Button C: "<<to_string(buttonC)<<" Button Z: "<<to_string(buttonZ)<<endl;
    cout<<"Accelerometer A: ("<<to_string(accelerometerAX)<<", "<<to_string(accelerometerAY)<<", "<<to_string(accelerometerAZ)<<")"<<endl;
    cout<<"Rotations (P,R): ("<<to_string(accelerometerAP)<<", "<<to_string(accelerometerAR)<<")"<<endl;
    // cout<<bytes<<endl;
}

int main(int argc, char *argv[])
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    // Serial Communication
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    // Serial object
    serialib serial;
    DecodeStatusCodes statusCodes(false);

    // Connection to serial port
    char statusCode = serial.openDevice(SERIAL_PORT, 57600, SERIAL_DATABITS_8, SERIAL_PARITY_NONE, SERIAL_STOPBITS_1);

    // If connection fails, display error message and exit. Otherwise, display a success message
    if (statusCodes.openDevice(statusCode) == false) return 1;

    statusCode = serial.flushReceiver();

    // Display status code message
    // statusCodes.flushReceiver(statusCode);

    cout<<"Select operation to perform:"<<endl;
    cout<<"\t1: Get controller status (x1)"<<endl;
    cout<<"\t2: Get controller status (x10)"<<endl;
    cout<<"\t3: Get controller status (infinite)"<<endl;
    cout<<"\t4: Play tower light animation"<<endl;
    cout<<"\t5: Play flatline sound effect"<<endl;
    cout<<"\t6: Nunchuck Controller Settings"<<endl;
    cout<<"\t7: Set button color"<<endl;
    cout<<"> ";
    int mode;
    cin>>mode;
    // mode = 2;

    if (mode == 1)
    {
        auto start = high_resolution_clock::now();
        serial.writeString("CT\n");
        char* bytes = new char[100];
        statusCode = serial.readString(bytes, '\n', 100, 16);
        auto stop = high_resolution_clock::now();
        statusCodes.readString(statusCode);
        if (statusCode > 0)
            decodeInputs(bytes);
        delete bytes;
        auto duration = duration_cast<microseconds>(stop - start);
        cout << "Responce time: " << duration.count() << "us" << endl;
    }
    else if (mode == 2)
    {
        long timeCount;
        int reps = 10;
        for (int i=0; i<reps; i++)
        {
            auto start = high_resolution_clock::now();
            serial.writeString("CT\n");
            char* bytes = new char[100];
            statusCode = serial.readString(bytes, '\n', 100, 16);
            auto stop = high_resolution_clock::now();
            statusCodes.readString(statusCode);
            // if (statusCode > 0)
            //     decodeInputs(bytes);
            delete bytes;
            auto duration = duration_cast<microseconds>(stop - start);
            timeCount += duration_cast<microseconds>(stop - start).count();
            Sleep(10);
        }
        cout<<"Average responce time: "<<to_string(timeCount/reps)<<"us"<<endl;;
    }
    else if (mode == 3)
    {
        while (true)
        {
            serial.writeString("CT\n");
            char* bytes = new char[100];
            statusCode = serial.readString(bytes, '\n', 100, 16);
            statusCodes.readString(statusCode);
            // if (statusCode > 0)
                // cout<<"Read succesfull"<<endl;
            //     decodeInputs(bytes);
            delete bytes;
            // Sleep(10);
        }
    }
    else if (mode == 4)
    {
        // Tower light animation
        serial.writeString("AN1\n"); // Finish later
    }
    else if (mode == 5)
    {
        // Flatline sound
        serial.writeString("AN2\n"); // Finish later
    }
    else if (mode == 6)
    {
        cout<<"Select Nunchuck operation to perform:"<<endl;
        cout<<"\t1: Enable Nunchuck"<<endl;
        cout<<"\t2: Disable Nunchuck"<<endl;
        cout<<"\t3: Reset Nunchuck"<<endl;
        cout<<"\t4: Toggle Nunchuck"<<endl;
        cout<<"> ";
        mode;
        cin>>mode;
        if (mode == 1)
        {
            serial.writeString("CFNCE\n");
        }
        else if (mode == 2)
        {
            serial.writeString("CFNCD\n");
        }
        else if (mode == 3)
        {
            serial.writeString("CFNCR\n");
        }
        else if (mode == 4)
        {
            serial.writeString("CFNCT\n");
        }
    }
    else if (mode == 7)
    {
        char R[3];
        char G[3];
        char B[3];
        cout<<"R: ";
        cin.ignore();
        cin.getline(R, 3);
        cout<<"G: ";
        cin.getline(G, 3);
        cout<<"B: ";
        cin.getline(B, 3);
        cout<<"Setting button color to ("<<R<<", "<<G<<", "<<B<<")"<<endl;
        char command[10] = {'U', 'L', 'K', R[0], R[1], G[0], G[1], B[0], B[1], '\n'};
        serial.writeString(command);
        
        // while (!serial.available()) { }
        // char* bytes = new char[100];
        // statusCode = serial.readString(bytes, '\n', 100, 16);
        // statusCodes.readString(statusCode);
        // cout<<bytes;
        // delete bytes;
    }
    else if (mode == -1)
    {
        int R, G, B;
        cout<<"R: ";
        cin>>R;
        cout<<"G: ";
        cin>>G;
        cout<<"B: ";
        cin>>B;
        char command[10] = {'U', 'L', 'K', 0, 0, 0, 0, 0, 0, '\n'};
        stringstream buffer;
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
        cout<<"Setting button color to ("<<R<<", "<<G<<", "<<B<<")"<<endl;
        serial.writeString(command);
    }
    else
    {
        cout<<"You have entered an invalid mode."<<endl;
    }

    // Close the serial device
    serial.closeDevice();

    return 0 ;
}
