# ArduinoWiFi-TVRemote #

During the sixth Victorian COVID-19 Lockdown, I ran out of AAA batteries for my TV remote. So with the necessary kit available and being unable to do much outside, I decided to build an overly-engineered Wi-Fi enabled replacement.

![Short clip showing an iPad controlling a TV using this project](example.gif)

Similar systems can be used to remotely control almost any device using infrared controls. The code would just need to be adapted to the purpose. This solution was built for use with a Kogan LED TV, and does not need the device to be a Smart TV

# Bill of Materials #

* XC4411 Arduino Uno with built-in ESP8266 (from Jaycar Electronics)
* HW-489 Infrared LED Module (KY-005)
* HW-490 Infrared Receiver Module (for reverse-engineering)
* Breadboard
* Jumper Leads

The XC4411 could be substituted for any similar unit, but the code would need to be changed.

# Deployment #

The XC4411 contains two separate microcontrollers on the one board: one running the Arduino Uno on the ATmega328P, and the ESP8266 as a separate MCU. The two communicate using the Serial channel. Because of this, the ESP8266 cannot directly use the `digitalWrite` function, and instead need to direct each other using messages.

File | Description
- | -
[`arduino.ino`](code/arduino.ino) | Sketch for the Arduino Uno
[`esp8266.ino`](code/esp8266.ino) | Sketch for the ESP8266

# Interface #

The web interface is accessible either at the root, or `/menu`. The buttons trigger function calls, like `/command/power`, which trigger the LED via the Arduino.

Extremely basic, and built using simple ASCII symbols

```
Power
  ▲
◀ ◉ ▶
  ▼
 ☖ ⚐
 ← ☰
CH VL
▲   ▲
    M
▼   ▼
```

Front-end and designer friends, plz don't disown me.

# Questions #

## How do you tell which codes are needed for a particular device? ##

Different devices use different encoding techniques for infrared signals, and different values in the encoding. The best way to identify which encoding and values you need is to reverse-engineer them with a working remote.

By using an IR Receiver component (HW-490), you can use the example [SimpleReceiver](https://github.com/Arduino-IRremote/Arduino-IRremote/blob/master/examples/SimpleReceiver/SimpleReceiver.ino) built as part of the `IRremote` library to read the signals to the Serial Monitor. From there, it's simply mapping each command to a signal.

This repository was built for use with a Kogan LED TV, and would likely need to be rebuilt for other brands.

## How did you get the codes without AAA batteries? ##

AA batteries, electrical tape, and spare wire. Could've swapped batteries with another device. Didn't think of that until later. Not recommended.

## Could this be extended to use additional functions? ##

Definitely, it just needs extra information about what infrared signals to send, and new buttons to be created for the web interface.

## How can you tell if it's working? ##

Part of the code for the Arduino includes a `blink` function, which causes the built-in LED to flash several times, depending on the operation.

\# | Operation
- | -
3 | Signal received from Serial
5 | Signal sent through IR Transmitter

When the light flashes in two sets of pulses, 3 + 5, a command has been received by the web interface, and then forwarded to the IR Transmitter. If it does not work, either the IR LED may be faulty, signal not reaching the IR LED, receiver on the destination device is faulty, or the codes are incorrect.

If the LED simply flashes three times, the command was not recognized, and not passed to the IR LED.

The ESP8266 also responds to `/data`, which prints the most recent Serial interactions with the Arduino. This can be useful in troubleshooting commands being passed.

## Why didn't you just buy more batteries? ##

Civic responsibility demands that during a lockdown in a pandemic that I take every step possible to minimize going out, even to the shops except for essential supplies, and batteries for a TV remote are not reasonably essential.

Plus, I needed to justify burning time during lockdown, and buying more stuff in future $_$