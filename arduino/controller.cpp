//
// Created by Zrp200 on 4/19/2022.
//

#include <iostream>
#include <bitset>
#include <string>

// Serial library
#include "lib/serialib.h"

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

#include "controller.h"