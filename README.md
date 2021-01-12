# Wireless-Master-Key
Exploiting Software-Defined Radio - Common attacks and a device to exploit them (Proof of Concept).


Software-Defined Radio is used in a broad variety of applications. One of them is a wireless remote, like your garage door remote or your car-key fob. These devices can be hacked easily. Some basic, but effective, attacks are presented in this project. The Wireless Master Key is  a small DIY device, based on an ESP8266 and the Arduino platform, which serves as a proof of concept.

![alt text](https://github.com/code-byter/Wireless-Master-Key/blob/master/images/333_device.jpg "Wireless master key")

Functionality:
This device has the following features:
- Receiving, Decoding and Displaying Signals
- Replaying Signals
- Transmitting custom signals

A detailed description can be found in ```paper.pdf```

# Disclaimer
This project is is only for educational purposes and serves as a proof of concept.
Neither the ESP8266, nor the other parts were meant or built for such purposes. Bugs can occur!

Use it only against your own devices!
Please check the legal regulations in your country before using it.
I don't take any responsibility for what you do with this project.

# Hardware
The Wireless Master Key operates within the 433MHz frequency band. It uses a reciever and two transmitters (But the second transmitter is optional). The user interface consists of an OLED-Display and a Rotary Encoder.
![alt text](https://github.com/code-byter/Wireless-Master-Key/blob/master/images/310_schematic.png "Schematic")

Device Pin  | NodeMCU Pin
----------- | ------------
Receiver    | D5
Transmitter | D6
Jammer      | D8
SCL         | D3
SDA         | D4
CLK         | D1
DT          | D2
SW          | D7

![alt text](https://github.com/code-byter/Wireless-Master-Key/blob/master/images/332_assembly.jpg "Electronics")

# Software
The following libraries need to be installed.
```
Adafruit_GFX.h
Adafruit_SSD1306.h
RCSwitch.h
Encoder.h
```


The ```code.ino``` files contains all the necessary code to programm the ESP8266. This can be done by using the Arduino IDE. The code, that's necessary for transmitting the signals is left out.


