//
// Created by Zrp200 on 4/19/2022.
//

#include <iostream>
#include "DecodeStatusCodes.h"

using namespace arduino;
using namespace std;

DecodeStatusCodes::DecodeStatusCodes(bool showSuccess) : showSuccess(showSuccess)
{
    //mode true: Display success and failure messages
    //mode false: Display only failure messages
}
DecodeStatusCodes::~DecodeStatusCodes() {}

bool DecodeStatusCodes::flushReceiver(char code)
{
    if (code != 0) {
        if (showSuccess) {cout<<"Receiver succesfully flushed"<<endl;}           return true;
    } else {              cout<<"Error: reciever not flushed succesfully"<<endl; return false;}
}

bool DecodeStatusCodes::openDevice(char code)
{
    if      (code ==  1) {
        if (true)        {printf("Successful connection to %s\n",SERIAL_PORT);}                     return true;}
    else if (code == -1) {printf("Error: device not found %s\n",SERIAL_PORT);                       return false;}
    else if (code == -2) {printf("Error: error while opening the device %s\n",SERIAL_PORT);         return false;}
    else if (code == -3) {printf("Error: error while getting port parameters %s\n",SERIAL_PORT);    return false;}
    else if (code == -4) {printf("Error: Speed (Bauds) not recognized %s\n",SERIAL_PORT);           return false;}
    else if (code == -5) {printf("Error: error while writing port parameters %s\n",SERIAL_PORT);    return false;}
    else if (code == -6) {printf("Error: error while writing timeout parameters %s\n",SERIAL_PORT); return false;}
    else if (code == -7) {printf("Error: Databits not recognized %s\n",SERIAL_PORT);                return false;}
    else if (code == -8) {printf("Error: Stopbits not recognized %s\n",SERIAL_PORT);                return false;}
    else if (code == -9) {printf("Error: Parity not recognized %s\n",SERIAL_PORT);                  return false;}
    else                 {cout<<"Error: an unknown error has occured"<<endl;                        return false;}
}

bool DecodeStatusCodes::readBytes(int code)
{
    if      (code >  -1) {
        if (showSuccess) {cout<<"Bytes read succesfully"<<endl;}                return true;}
    else if (code == -1) {cout<<"Error: error while setting the Timeout"<<endl; return false;}
    else if (code == -2) {cout<<"Error: error while reading the byte"<<endl;    return false;}
    else                 {cout<<"Error: an unknown error has occured"<<endl;    return false;}
}

bool DecodeStatusCodes::readChar(char code)
{
    if      (code ==  1) {
        if (showSuccess) {cout<<"Character read succesfully"<<endl;}            return true;}
    else if (code ==  0) {cout<<"Error: Timeout reached"<<endl;                 return false;}
    else if (code == -1) {cout<<"Error: error while setting the Timeout"<<endl; return false;}
    else if (code == -2) {cout<<"Error: error while reading the byte"<<endl;    return false;}
    else                 {cout<<"Error: an unknown error has occured"<<endl;    return false;}
}

bool DecodeStatusCodes::readString(int code)
{
    if      (code >   0) {
        if (showSuccess) {cout<<"String read succesfully"<<endl;}               return true;}
    else if (code ==  0) {cout<<"Error: timeout reached"<<endl;                 return false;}
    else if (code == -1) {cout<<"Error: error while setting the Timeout"<<endl; return false;}
    else if (code == -2) {cout<<"Error: error while reading the byte"<<endl;    return false;}
    else if (code == -3) {cout<<"Error: MaxNbBytes is reached"<<endl;           return false;}
    else                 {cout<<"Error: an unknown error has occured"<<endl;    return false;}
}

bool DecodeStatusCodes::writeBytes(char code)
{
    if      (code ==  1) {
        if (showSuccess) {cout<<"Bytes written succesfully"<<endl;}             return true;}
    else if (code == -1) {cout<<"Error: error while writting data"<<endl;       return false;}
    else                 {cout<<"Error: an unknown error has occured"<<endl;    return false;}
}

bool DecodeStatusCodes::writeChar(char code)
{
    if      (code ==  1) {
        if (showSuccess) {cout<<"Character written succesfully"<<endl;}         return true;}
    else if (code == -1) {cout<<"Error: error while writting data"<<endl;       return false;}
    else                 {cout<<"Error: an unknown error has occured"<<endl;    return false;}
}

bool DecodeStatusCodes::writeString(char code)
{
    if      (code ==  1) {
        if (showSuccess) {cout<<"String written succesfully"<<endl;}            return true;}
    else if (code == -1) {cout<<"Error: error while writting data"<<endl;       return false;}
    else                 {cout<<"Error: an unknown error has occured"<<endl;    return false;}
}