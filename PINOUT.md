# Club Robot Ardunio Mega Pinout #


----------

This files contains the pinout and connections for the DPRG club robot (ver. 2016 - Arduino Mega).Pinouts are provided for:
-   Motors / Encoders
-   Ultrasonic sensors
-   Hobby Servo (claw)
-   Bumper sensor
-   Power connections

----------
**Motors / Encoders**  
Arduino pin definitions are used.   

| Connection | Wire Color | Left Motor | Right Motor |   
|:---:|:---:|:---:|:---:|   
| M1 motor (-) | Black | B2 | B1|  
| 5V or 3.3V | Red | 5V buss | 5V buss|
| Enc A phase | White | 20 | 3 |
| Enc B phase | Yellow | 21 | 2 |
| Gnd | Orange | Gnd buss | Gnd buss|
| M1 motor (+) | Green | A2 | A1 |


**Ultrasonic Sensors**  
| Connection | L | R |
|:---:|:---:|:---:|
| 5V | 5V buss | 5V buss |
| Gnd | Gnd buss | Gnd buss |
| Trigger | 34 | 36 |
| Echo | A9 | A8 |


**Hobby Servo (claw)**  
| Connection | Wire Color | Claw |
|:---:|:---:|:---:|
| Gnd | Brown | Gnd buss |
| 5V | Red | 5V buss |
| Signal | Yellow | 28 |

**Bumper Sensor**  
The robot's bumper sensor consists of 4 normally closed switches, 2 switches per side. The switches on each side are wired in series. In the table below the the two switch on a side are designated as upper (U) and lower (B). Each switch has two terminals. It is assumed that the switches are mounted so that the two terminals are horizontal. The left most terminal is designated as 1, and the right most terminal is designated as 2.

|Switch | Side | 1 | 2 |
|:---:|:---:|:---:|:---:|
|Upper|Left|jumpered to B1|Gnd and right side U1|
|Lower|Left|jumpered to U1|30|
|Upper|Right|Gnd and left side U2|jumpered to B1|
|Lower|Right|32|jumpered to U1|
The input pins 30 and 32 must have the internal pull-up resistors activated.
 

**Power Connections**  
It is assumed that the switch and fuse module are wired as shown in the DPRG Club Robot construction videos playlist: https://www.youtube.com/playlist?list=PLXixJXO-dNbpC9WK1pDCww6C4rDCkuDN3. See videos: DPRG Club Robot: Switch Wiring Harness, DPRG Club Robot: Fuse Module, and DPRG Club Robot: Preparing the UBEC.

|Module|Connection|
|:---:|:---:|
|Battery|Fuse Module|
|Power Switch - Red| Fuse Module, Motor Shield Pwr (+),UEB input (+)|
|Fuse Module - Black|Motor Shield Pwr (-),UEB input (-)|
|UBEC - output| 5v/Gnd buss module|
----------
**Version History** 
  
    0.1.0 - Beta 1: First public beta - October 14, 2016  - Author: Doug Paradis

