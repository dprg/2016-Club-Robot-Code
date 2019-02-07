# Club Robot - 2016 Code for 16x2 Display Shield 

![16x2 display shield](images/16x2_display_shield.jpg)

### Contents:
This part of the repository contains code that can be used with an Arduino Uno and a 16x2 character display shield. There are three sub-directories: source code for the UNO, source code to be added to the code on the main processor (Arduino Mega), and examples of what the necessary code would look like for ultrasonic sensors and a servo driven gate. The communication between the Uno and Mega board is conducted via USART using a baud rate is 115200. Data is transferred by the single serial port on the UNO and the Serial3 port on the Mega. Four wires connect the UNO to the robot: 5V, Gnd, TX, and RX.

The code will need to be modified to match the sensors and sub-systems of the individual robot. 

The code was authored by Doug Paradis (paradug).

|Directory  | Purpose|
| :-------------: | :------------- |
| Uno pgm  | The program for the Arduino UNO. |
| comm funcs on mega | The communication functions that are part of the Mega's program. |
| Ex func for sensors  | Example funcs for ultrasonic sensors and a gate sub-system. |


 **Note:** The UNO program requires the LiquidCrystal library.

