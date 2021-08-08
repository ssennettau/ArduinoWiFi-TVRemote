#include <Arduino.h>
#include <IRremote.h>

// Pin connected to the IR Transmitter for Signalling
const int IRTMIT_PIN = 3;

// NEC Transmission Protocol is used in this code
// Address and Commands are specific to the TV I used in this project. If used
// on any other type of TV, the necessary codes would need to be identified.

// Device Address
const uint16_t DEV_ADDRESS = 0x4;

// List of Commands
const uint8_t CMD_POWER = 0x8;
const uint8_t CMD_CH_UP = 0x0;
const uint8_t CMD_CH_DN = 0x1;
const uint8_t CMD_VL_UP = 0x2;
const uint8_t CMD_VL_DN = 0x3;
const uint8_t CMD_MUTE = 0x9;

const uint8_t CMD_MENU_L = 0x7;
const uint8_t CMD_MENU_R = 0x6;
const uint8_t CMD_MENU_U = 0x40;
const uint8_t CMD_MENU_D = 0x41;
const uint8_t CMD_MENU_OK = 0x44;

const uint8_t CMD_NAV_HOME = 0x92;
const uint8_t CMD_NAV_SRC = 0xB;
const uint8_t CMD_NAV_BACK = 0x47;
const uint8_t CMD_NAV_SUB = 0x43;

// Blink Function
// Uses LED flashes to visually debug the device
void blink(int q = 1)
{
  for (int i=0; i<q; i++)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(50);
    digitalWrite(LED_BUILTIN, LOW);
    delay(50);
  }

  // Ensures a short delay between each of the signal pulses
  delay(200);
}

// Consolidating all the code for sending IR commands
void sendIrSignal(uint8_t code)
{
  // Debug output, used to verify what commands are being sent
  Serial.print("+ SEND: dev(0x");
  Serial.print(DEV_ADDRESS, HEX);
  Serial.print("), cmd(0x");
  Serial.print(code, HEX);
  Serial.println(")");

  IrSender.sendNEC(DEV_ADDRESS, code, 0);

  // Any time a command is successfully identified and sent, the LED blinks 5 times
  blink(5);
}

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  
  Serial.begin(115200);
  Serial.println();
  IrSender.begin(IRTMIT_PIN, ENABLE_LED_FEEDBACK);
}

void loop()
{
  if (Serial.available())
  {
    // When any Serial input is detected, the LED flashes three times
    // If the LED only flashes three times, and the following five are not detected, the
    // command was received by the Arduino, but not sent to the IR Transmitter
    blink(3);

    // Sanatizes the command received from the Serial channel
    String input = Serial.readStringUntil('\n');
    input.toUpperCase();
    input.replace("\n\n","\n");
    input.replace("\r","");
    
    Serial.print("> DEBUG: ");
    Serial.println(input);

    // Okay, really this should've been a much cleaner structure, but it works. If anyone
    // else decides to use this, implement something cleaner.
    //
    // "Don't do what Donny Don't does"
    if (input == "POWER")
    {
      sendIrSignal(CMD_POWER);
    }
    else if (input == "CH_UP")
    {
      sendIrSignal(CMD_CH_UP);
    }
    else if (input == "CH_DN")
    {
      sendIrSignal(CMD_CH_DN);
    }
    else if (input == "VL_UP")
    {
      sendIrSignal(CMD_VL_UP);
    }
    else if (input == "VL_DN")
    {
      sendIrSignal(CMD_VL_DN);
    }
    else if (input == "MUTE")
    {
      sendIrSignal(CMD_MUTE);
    }
    else if (input == "MENU_L")
    {
      sendIrSignal(CMD_MENU_L);
    }
    else if (input == "MENU_R")
    {
      sendIrSignal(CMD_MENU_R);
    }
    else if (input == "MENU_U")
    {
      sendIrSignal(CMD_MENU_U);
    }
    else if (input == "MENU_D")
    {
      sendIrSignal(CMD_MENU_D);
    }
    else if (input == "NAV_HOME")
    {
      sendIrSignal(CMD_NAV_HOME);
    }
    else if (input == "NAV_SRC")
    {
      sendIrSignal(CMD_NAV_SRC);
    }
    else if (input == "NAV_BACK")
    {
      sendIrSignal(CMD_NAV_BACK);
    }
    else if (input == "NAV_SUB")
    {
      sendIrSignal(CMD_NAV_SUB);
    }    
    else if (input == "MENU_OK")
    {
      sendIrSignal(CMD_MENU_OK);
    }
    else
    {
      Serial.print("- NSEND: ");
      Serial.println(input);
    }

    // Flushes the Serial buffer, awaiting the next command
    Serial.flush();
  }
  delay(100);
}
