/*
 Example sketch for the PS4 Bluetooth library - developed by Kristian Lauszus
 For more information visit my blog: http://blog.tkjelectronics.dk/ or
 send me an e-mail:  kristianl@tkjelectronics.com
 */

#include <PS4BT.h>
#include <usbhub.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

USB Usb;
//USBHub Hub1(&Usb); // Some dongles have a hub inside
BTD Btd(&Usb);  // You have to create the Bluetooth Dongle instance like so

/* You can create the instance of the PS4BT class in two ways */
// This will start an inquiry and then pair with the PS4 controller - you only have to do this once
// You will need to hold down the PS and Share button at the same time, the PS4 controller will then start to blink rapidly indicating that it is in pairing mode
// PS4BT PS4(&Btd);
PS4BT PS4(&Btd, PAIR);

// After that you can simply create the instance like so and then press the PS button on the device
//PS4BT PS4(&Btd);

bool printAngle, printTouch;
uint8_t oldL2Value, oldR2Value;

void setup() {
  Serial.begin(115200);
#if !defined(__MIPSEL__)
  while (!Serial)
    ;  // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1)
      ;  // Halt
  }
  Serial.print(F("\r\nPS4 Bluetooth Library Started"));
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
}

void loop() {
  Usb.Task();

  if (PS4.connected()) {
    int right_y = 128 - PS4.getAnalogHat(LeftHatY);
    int left_y = 128 - PS4.getAnalogHat(RightHatY);
    Serial.print(F("\r\nRightY: "));
    Serial.print(right_y);
    Serial.print(F("\tLeftY: "));
    Serial.print(left_y);

    int right_go, left_go, right_back, left_back;

    if (right_y > 0) {
      right_go = 0;
      right_back = right_y;
    } else {
      right_go = -right_y;
      right_back = 0;
    }
    if (left_y > 0) {
      left_go = left_y;
      left_back = 0;
    } else {
      left_go = 0;
      left_back = abs(left_y);
    }

    Serial.print(F("\tright_go: "));
    Serial.print(right_go);
    Serial.print(F("\tright_back: "));
    Serial.print(right_back);
    Serial.print(F("\tleft_go: "));
    Serial.print(left_go);
    Serial.print(F("\tleft_back: "));
    Serial.print(left_back);

    analogWrite(2, right_go);
    analogWrite(3, right_back);
    analogWrite(5, left_go);
    analogWrite(6, left_back);
  }
}
