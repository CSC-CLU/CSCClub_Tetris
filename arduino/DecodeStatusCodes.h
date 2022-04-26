/**
 * @file /SerialCommunications_Testing.cpp
 * @author Eric Heinke
 * @date February 7 2022
 * @brief File contains functions for displaying status message from the return status of Philippe Lucidarme's serial library.
 */

#ifndef DECODESTATUSCODES_H
#define DECODESTATUSCODES_H

#if defined (_WIN32) || defined(_WIN64)
#define SERIAL_PORT "\\\\.\\COM4"
#endif
#if defined (__linux__) || defined(__APPLE__)
#define SERIAL_PORT "/dev/ttyACM0"
#endif

namespace arduino {
    class DecodeStatusCodes {
    public:
        DecodeStatusCodes();

        DecodeStatusCodes(bool mode);

        ~DecodeStatusCodes();
        bool flushReceiver(char code);
        bool openDevice(char code);
        bool readBytes(int code);
        bool readChar(char code);
        bool readString(int code);
        bool writeBytes(char code);
        bool writeChar(char code);
        bool writeString(char code);
    private:
        bool showSuccess;
    };
}

#endif // DECODESTATUSCODES_H