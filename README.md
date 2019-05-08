# ginny-arduino-control
Wireless control programs for Ginny 1.0

Very rough wireless manual control program for Ginny 1.0
- Laptop runs Controller program which reads XBox controller input and sends Serial packets to Transmitter Arduino.
- Transmitter Arduino is connected to laptop and receives serial packets from controller program and transmits to receiver via XBee board.
- Receiver Arduino is on robot and receives serial packets from Transmitter via XBee board and generates PWM signals to Talon SRXs.


On the robot:
- Left Drive with 2 CIM motors, 2 Talons
- Right Drive with 2 CIM motors, 2 Talons
- Lift with 2 Linear Actuators, 1 Talon
- Dumper with 2 Linear Actuators, 1 Talon
- Bucket-Ladder Digger with 1 CIM motor, 1 Talon
