# GTIL2LCD
This code will decode the LCD data stream on a SUN-1000GTIL2 inverter.
It will probably work on the 2000W model though I don't have one to check.
This code was compiled in PlatformIO on VSCode for the ESP8266. I am using a NodeMCU devkit 30 pin module which has the ESP-12. It will probably work on others.

It just prints the decoded data to the serial terminal. You will need to add to the code if you want to send it elsewhere.

Hook up the ground and the GPIO4 (D2) wires of the NodeMCU to the GND and RX IN pins on the LCD CN2 connector. Set your serial terminal program to receive at 9600 baud.

Be safe around the inverter as it has lethal voltages inside. Do not open the inverter if you don't understand these risks.

There is no warranty and no liability for the use of this code.

