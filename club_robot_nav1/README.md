# Club Robot Ardunio Mega Example Code #


----------

This repository contains code that provides basic functionality to the DPRG club robot (ver. 2016 - Arduino Mega). Routines are provided for:


-   Autonomous navigation using a path of way points and odometry


-   Motor control (PID / encoders)


-   Light-weight cooperative round-robin multitasking executive


-   Ultrasonic sensor


-   Bumper sensor


-   Hobby Servo (claw)

-  
**Navigation**  
The example code uses differential drive kinematics and the quadrature outputs from the club robot's motor encoders to provide position and heading information. A desired robot path is defined in an two array set of way points; one for x and one for y coordinates. The arrays allow up to 15 way points.

**Motor Control**  
Motor control is achieved by driving the robot's H-bridge board with a PI controller routine for each motor. The PI controller uses a setpoint signal that combines a desired count of ticks per period and a value derived from heading error (also a PI controller). The input signal is the actual count of encoder ticks per period.

**Cooperative Round-Robin Multitasking Executive (LMX)**  
LMX is a cooperative round-robin multitasking executive, developed by David Anderson (DPRG member) is used to allow multiple cooperative tasks to run in parallel.

**Ultrasonic Sensors**  
The club robot is configured with two ultrasonic sensors to measure obstacles in front of the robot. Routines in the code allow for several behaviors based on the sensor results:


-   Slow down and stop the robot when an obstacle is within a given distance and resume path when obstacle removed.

-   Slow down the robot when an obstacle is within a given distance and change path to go around the obstacle. **note:** This is to show behavior of using a "temporary way point". 

- Slow down and operate claw to capture can directly in front of robot.  

**Bumper sensor**  
The robot's bumper has 4 normally closed switches, 2 switches per side. The switches on each side are wired in series. When the bumper comes in contact with an obstacle, one or more of the switches will open. This signal is detected by the processor and a bumper routine is run. The example code's bumper routine simply cause the robot to stop when an obstacle is hit.

**Hobby Servo (claw)**  
The robot is fitted with a 3D printed claw that is driven by a hobby servo. The example code uses the softservo Arduino library to control the servo. Routines are provided to open and close the claw.

 
----------

**Acknowledgments**   

The teensy encoder library created by pcjr from
[http://www.pjrc.com/teensy/td_libs_Encoder.html](http://www.pjrc.com/teensy/td_libs_Encoder.html) is used in the programs.

The motor shield control functions from the MonsterMoto Shield Example Sketch coded by Jim Lindblom were used to drive the motors in the programs.

LMX, a cooperative round-robin multitasking executive, developed by David Anderson, is used in the example code. 

----------
**Version History** 
  
    0.1.0 - Beta 1: First public beta - October 11, 2016  - Author: Doug Paradis