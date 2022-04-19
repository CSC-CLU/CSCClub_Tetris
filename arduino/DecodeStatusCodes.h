/**
 * @file /SerialCommunications_Testing.cpp
 * @author Eric Heinke
 * @date February 7 2022
 * @brief File contains functions for displaying status message from the return status of Philippe Lucidarme's serial library.
 */

#ifndef DECODESTATUSCODES_H
#define DECODESTATUSCODES_H

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