/**
 * @file TetrisGameControllerV3
 * @author Eric Heinke
 * @date April 21 2022
 * @brief Code for using Arduino as a game controller
 */

#include <DIO2.h>
#include <WiiChuck.h>
#include <Adafruit_NeoPixel.h>

// This program requires the D2IO, WiiChuck, and Adafruit NeoPixel libraries. Make sure to install them.

// For information on Wii nunchuck use
// https://learn.adafruit.com/adafruit-wii-nunchuck-breakout-adapter/arduino-use

// Serial variables
boolean echoEnabled = false;

// Status for various LEDs
byte towerLEDState = 0;
long buttonColor = 0xFFFFFF;

// Nunchuck controller variables
Accessory nunchuck1;
boolean nunchuckEnabled;
int joystick1X;
int joystick1Y;
int joystick2X;
int joystick2Y;
int joystickSX;
int joystickSY;
int accelerometerAX;
int accelerometerAY;
int accelerometerAZ;
int accelerometerAP;
int accelerometerAR;
boolean buttonC;
boolean buttonZ;

// Declare our NeoPixel strip object:
Adafruit_NeoPixel buttonPixels(8, 32, NEO_GRB + NEO_KHZ800);

void setup() {
  // Builtin LED (L)
  pinMode2(LED_BUILTIN, OUTPUT);
  
  // Setting up button input pins
  pinMode2f(DP22, INPUT_PULLUP); // Up (instant fall)
  pinMode2f(DP23, INPUT_PULLUP); // Down (fast fall)
  pinMode2f(DP24, INPUT_PULLUP); // Left
  pinMode2f(DP25, INPUT_PULLUP); // Right
  pinMode2f(DP26, INPUT_PULLUP); // A (rotate clockwise)
  pinMode2f(DP27, INPUT_PULLUP); // B (rotate counterclockwise)
  pinMode2f(DP28, INPUT_PULLUP); // Select (hold piece)
  pinMode2f(DP29, INPUT_PULLUP); // Start (start/pause)
  
  // Setting up joystick and joystick button input pins
  pinMode(A0, INPUT);    // Joystick 1 X
  pinMode(A1, INPUT);    // Joystick 1 Y
  pinMode2f(DP30, INPUT_PULLUP); // Joystick 1 button
  pinMode(A2, INPUT);    // Joystick 2 X
  pinMode(A3, INPUT);    // Joystick 2 Y
  pinMode2f(DP31, INPUT_PULLUP); // Joystick 2 button

  // Setting up tower LED output pins
  pinMode2f(DP46, OUTPUT); // Tower 1 Green
  pinMode2f(DP45, OUTPUT); // Tower 1 Yellow
  pinMode2f(DP48, OUTPUT); // Tower 1 Red
  pinMode2f(DP49, OUTPUT); // Tower 1 Buzzer
  pinMode2f(DP50, OUTPUT); // Tower 2 Green
  pinMode2f(DP51, OUTPUT); // Tower 2 Yellow
  pinMode2f(DP52, OUTPUT); // Tower 2 Red
  pinMode2f(DP53, OUTPUT); // Tower 2 Buzzer
  
  // start serial port at 115200 bps, 8 bits per transfer, no parity, one stop bit:
  Serial.begin(57600, SERIAL_8N1);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // Initialize nunchuck values
  nunchuckEnabled = false;

  buttonPixels.begin();
  buttonPixels.show();
  buttonPixels.clear();
  
  // Tower lights startup animation
  towerLightAnimation();
}

void towerLightAnimation()
{
  digitalWrite2f(DP46, LOW);
  digitalWrite2f(DP50, LOW);
  digitalWrite2f(DP47, LOW);
  digitalWrite2f(DP51, LOW);
  digitalWrite2f(DP48, LOW);
  digitalWrite2f(DP52, LOW);
  digitalWrite2f(DP49, LOW);
  digitalWrite2f(DP53, LOW);
  delay(400);
  digitalWrite2f(DP46, HIGH);
  digitalWrite2f(DP50, HIGH);
  delay(400);
  digitalWrite2f(DP46, LOW);
  digitalWrite2f(DP50, LOW);
  digitalWrite2f(DP47, HIGH);
  digitalWrite2f(DP51, HIGH);
  delay(400);
  digitalWrite2f(DP47, LOW);
  digitalWrite2f(DP51, LOW);
  digitalWrite2f(DP48, HIGH);
  digitalWrite2f(DP52, HIGH);
  delay(400);
  digitalWrite2f(DP48, LOW);
  digitalWrite2f(DP52, LOW);
  digitalWrite2f(DP49, HIGH);
  digitalWrite2f(DP53, HIGH);
  delay(200);
  digitalWrite2f(DP49, LOW);
  digitalWrite2f(DP53, LOW);
}

void flatline()
{
  digitalWrite2f(DP49, LOW);
  digitalWrite2f(DP53, LOW);
  delay(200);
  digitalWrite2f(DP49, HIGH);
  digitalWrite2f(DP53, HIGH);
  delay(300);
  digitalWrite2f(DP49, LOW);
  digitalWrite2f(DP53, LOW);
  delay(700);
  digitalWrite2f(DP49, HIGH);
  digitalWrite2f(DP53, HIGH);
  delay(300);
  digitalWrite2f(DP49, LOW);
  digitalWrite2f(DP53, LOW);
  delay(700);
  digitalWrite2f(DP49, HIGH);
  digitalWrite2f(DP53, HIGH);
  delay(300);
  digitalWrite2f(DP49, LOW);
  digitalWrite2f(DP53, LOW);
  delay(700);
  digitalWrite2f(DP49, HIGH);
  digitalWrite2f(DP53, HIGH);
  delay(2000);
  digitalWrite2f(DP49, LOW);
  digitalWrite2f(DP53, LOW);
}

void sendControllerStatus()
{
  // Get analog readings
  joystick1X = map(analogRead(A0),0,1023,0,255);
  joystick1Y = map(analogRead(A1),0,1023,0,255);
  joystick2X = map(analogRead(A2),0,1023,0,255);
  joystick2Y = map(analogRead(A3),0,1023,0,255);

  if (nunchuckEnabled)
  {
    nunchuck1.readData(); // Read inputs and update maps for nunchuck
    joystickSX = nunchuck1.getJoyX();
    joystickSY = nunchuck1.getJoyY();
    accelerometerAX = nunchuck1.getAccelX();
    accelerometerAY = nunchuck1.getAccelY();
    accelerometerAZ = nunchuck1.getAccelZ();
    accelerometerAP = nunchuck1.getPitchAngle();
    accelerometerAR = nunchuck1.getRollAngle();
    buttonC = nunchuck1.getButtonC();
    buttonZ = nunchuck1.getButtonZ();
  }
  else
  {
    joystick1X = 0;;
    joystick1Y = 0;
    joystick2X = 0;
    joystick2Y = 0;
    joystickSX = 0;
    joystickSY = 0;
    accelerometerAX = 0;
    accelerometerAY = 0;
    accelerometerAZ = 0;
    accelerometerAP = 0;
    accelerometerAR = 0;
    buttonC = false;
    buttonZ = false;
  }
  
  //                          Joystick 1X (7-2)
  byte byte0 = 0b01000000 | ((joystick1X & 0b11111100) >> 2);
  
  //                           Joystick 1X (1-0)                  Joystick 1Y (7-4)
  byte byte1 = 0b01000000 | (((joystick1X & 0b00000011) << 4) | ((joystick1Y & 0b11110000) >> 4));

  //                          Joystick 1Y (3-0)                  B1 (instant fall)           B2 (fast fall)
  byte byte2 = 0b01000000 | (((joystick1Y & 0b00001111) << 2) | (digitalRead2f(DP22) << 1) | digitalRead2f(DP23));

  //                          Joystick 2X (7-2)
  byte byte3 = 0b01000000 | ((joystick2X & 0b11111100) >> 2);

  //                           Joystick 2X (1-0)                  Joystick 2Y (7-4)
  byte byte4 = 0b01000000 | (((joystick2X & 0b00000011) << 4) | ((joystick2Y & 0b11110000) >> 4));

  //                           Joystick 2Y (3-0)                 B3 (left)                   B4 (right)
  byte byte5 = 0b01000000 | (((joystick2Y & 0b00001111) << 2) | (digitalRead2f(DP24) << 1) | digitalRead2f(DP25));

  //                          Joystick SX (7-2)
  byte byte6 = 0b01000000 | ((joystickSX & 0b11111100) >> 2);

  //                           Joystick SX (1-0)                  Joystick SY (7-4)
  byte byte7 = 0b01000000 | (((joystickSX & 0b00000011) << 4) | ((joystickSY & 0b11110000) >> 4));

  //                           Joystick SY (3-0)                 B5 (A)                      B6 (B)
  byte byte8 = 0b01000000 | (((joystickSY & 0b00001111) << 2) | (digitalRead2f(DP26) << 1) | digitalRead2f(DP27));

  //                          Accelerometer AX (7-2)
  byte byte9 = 0b01000000 | ((accelerometerAX & 0b11111100) >> 2);

  //                            Accelerometer AX (1-0)                  Accelerometer AY (7-4)
  byte byte10 = 0b01000000 | (((accelerometerAX & 0b00000011) << 4) | ((accelerometerAY & 0b11110000) >> 4));

  //                            Accelerometer AY (3-0)                 B7 (Select)                 B8 (Start)
  byte byte11 = 0b01000000 | (((accelerometerAY & 0b00001111) << 2) | (digitalRead2f(DP28) << 1) | digitalRead2f(DP29));

  //                          Accelerometer AZ (7-2)
  byte byte12 = 0b01000000 | ((accelerometerAZ & 0b11111100) >> 2);

  //                            Accelerometer AZ (1-0)                 J1                           J2                           C               Z
  byte byte13 = 0b01000000 | (((accelerometerAZ & 0b00000011) << 4) | (digitalRead2f(DP30) << 3) | (digitalRead2f(DP31) << 2) | (buttonC << 1) | buttonZ);

  //                           Accelerometer AP (7-2)
  byte byte14 = 0b01000000 | ((accelerometerAP & 0b11111100) >> 2);

  //                            Accelerometer AP (1-0)                  Accelerometer AR (7-4)
  byte byte15 = 0b01000000 | (((accelerometerAP & 0b00000011) << 4) | ((accelerometerAR & 0b11110000) >> 4));

  //                            Accelerometer AR (3-0)                 Unused (1-0)
  byte byte16 = 0b01000000 | (((accelerometerAR & 0b00001111) << 2) | (false << 1) | false);

  Serial.write(byte0);
  Serial.write(byte1);
  Serial.write(byte2);
  Serial.write(byte3);
  Serial.write(byte4);
  Serial.write(byte5);
  Serial.write(byte6);
  Serial.write(byte7);
  Serial.write(byte8);
  Serial.write(byte9);
  Serial.write(byte10);
  Serial.write(byte11);
  Serial.write(byte12);
  Serial.write(byte13);
  Serial.write(byte14);
  Serial.write(byte15);
  Serial.write(byte16);
  Serial.write("\n");
}

void setTowerLEDs(byte state)
{
  digitalWrite2f(DP46, state & 0b10000000); // Tower 1 Green
  digitalWrite2f(DP47, state & 0b01000000); // Tower 1 Yellow
  digitalWrite2f(DP48, state & 0b00100000); // Tower 1 Red
  digitalWrite2f(DP49, state & 0b00010000); // Tower 1 Buzzer
  digitalWrite2f(DP50, state & 0b00001000); // Tower 2 Green
  digitalWrite2f(DP51, state & 0b00000100); // Tower 2 Yellow
  digitalWrite2f(DP52, state & 0b00000010); // Tower 2 Red
  digitalWrite2f(DP53, state & 0b00000001); // Tower 2 Buzzer
}

void loop() {
    if (Serial.available() > 0)
    {
      char* buffer = new char[10];
      int count = Serial.readBytesUntil('\n', buffer, 10);
      int marker = 0;

      // Serial Echo Enable/Disable
      if (buffer[0] == 'E')
      {
        if (buffer[1] == 'E')
          echoEnabled = true;
        else if (buffer[1] == 'D')
          echoEnabled = false;
      }

      if (echoEnabled)
      {
        Serial.write(buffer, count);
        Serial.println();
      }

      // Arduino Status
      if (buffer[0] == 'S' && buffer[1] == 'T')
      {
        // Print status to serial port
        Serial.println("Arduino Game Controller");
        Serial.println(" * Version: V3.1");
        Serial.println(" * Optimized for: Tetris");
        Serial.print(" * System status: ");
        if (echoEnabled) {Serial.print("EE ");} else {Serial.print("ED ");}
        if (nunchuckEnabled) {Serial.print("NE ");} else {Serial.print("ND ");}
        Serial.println();
      }

      // Controller Status
      if (buffer[0] == 'C' && buffer[1] == 'T')
        sendControllerStatus();

      // Configure
      if (buffer[0] == 'C' && buffer[1] == 'F')
      {
        // Nunchuck
        if (buffer[2] == 'N' && buffer[3] == 'C')
        {
          if (buffer[4] == 'E')
            nunchuckEnabled = true;
          if (buffer[4] == 'D')
            nunchuckEnabled = false;
          if (buffer[4] == 'T')
            nunchuckEnabled = !nunchuckEnabled;
          if (buffer[4] == 'R' && nunchuckEnabled)
            nunchuck1.reset();
          if (nunchuckEnabled && buffer[4] != 'R')
          {
            //Start nunchuck driver
            nunchuck1.begin();
            if (nunchuck1.type == Unknown) nunchuck1.type = NUNCHUCK; /** If the device isn't auto-detected, set the type explicatly */
          }
        }
      }

      // Update Lights
      if (buffer[0] == 'U' && buffer[1] == 'L')
      {
        // Key Lights
        if (buffer[2] == 'K')
        {
          char tmp[6] = {buffer[3], buffer[4], buffer[5], buffer[6], buffer[7], buffer[8]};
          buttonColor = strtol(tmp, NULL, 16);
//          Serial.println(buttonColor, HEX); // Dubug information
        }

        // Tower Lights
        if (buffer[2] == 'T')
        {
            char tmp[6] = {'0', '0', '0', '0', buffer[3], buffer[4]};
          long tmp2 = strtol(tmp, NULL, 16);
          char tmp3 = (char)(tmp2);
//          Serial.println(tmp3, HEX); // Dubug information
          setTowerLEDs(tmp3);
        }
      }

      // Animations
      if (buffer[0] == 'A' && buffer[1] == 'N')
      {
        if (buffer[2] == '1')
          towerLightAnimation();
        if (buffer[2] == '2')
          flatline();
      }

      // Debug Options
      if (buffer[0] == 'D' && buffer[1] == 'B')
      {
        // Add later
      }

      delete buffer;
      // Flush serial reciever
      while(Serial.available() > 0) {
        char t = Serial.read();
      }
    }

  digitalWrite2(LED_BUILTIN, nunchuckEnabled);
  
  if (digitalRead2f(DP22) == 0)
    buttonPixels.setPixelColor(7, buttonColor);
  else
    buttonPixels.setPixelColor(7, 0);
  if (digitalRead2f(DP23) == 0)
    buttonPixels.setPixelColor(6, buttonColor);
  else
    buttonPixels.setPixelColor(6, 0);
  if (digitalRead2f(DP24) == 0)
    buttonPixels.setPixelColor(5, buttonColor);
  else
    buttonPixels.setPixelColor(5, 0);
  if (digitalRead2f(DP25) == 0)
    buttonPixels.setPixelColor(4, buttonColor);
  else
    buttonPixels.setPixelColor(4, 0);
  if (digitalRead2f(DP26) == 0)
    buttonPixels.setPixelColor(3, buttonColor);
  else
    buttonPixels.setPixelColor(3, 0);
  if (digitalRead2f(DP27) == 0)
    buttonPixels.setPixelColor(2, buttonColor);
  else
    buttonPixels.setPixelColor(2, 0);
  if (digitalRead2f(DP28) == 0)
    buttonPixels.setPixelColor(1, buttonColor);
  else
    buttonPixels.setPixelColor(1, 0);
  if (digitalRead2f(DP29) == 0)
    buttonPixels.setPixelColor(0, buttonColor);
  else
    buttonPixels.setPixelColor(0, 0);
  buttonPixels.show();
}
