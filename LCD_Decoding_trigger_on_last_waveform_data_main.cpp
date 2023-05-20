// Thermistor resistance at 25 degrees C
#define THERMISTORNOMINAL 102000      
// Temperature for nominal resistance (almost always 25 C)
#define TEMPERATURENOMINAL 25   
// The beta coefficient of the thermistor (usually 3000-4000)
#define BCOEFFICIENT 3950
// the value of the 'other' resistor
#define SERIESRESISTOR 100000

#include <Arduino.h>
#include <SoftwareSerial.h>
SoftwareSerial SSerial(4, 5); // RX (GPIO4), TX (GPIO5)


byte myData[14];

float VAC;
float Output;
float VDC;
float TempConv;
float TempF;
float Resistance;

void setup() {

  Serial.begin(9600);
  SSerial.begin(9600);

}

void recvBytesWithStartMarker() {

  byte n = SSerial.available();

  if (n >= 2) {

    byte y = SSerial.read();

    if ((y == 0x92) || (y == 0x93) || (y == 0x94) || (y == 0x95) || (y == 0x96)) { // Sync found
    
      SSerial.readBytes(myData, 14);

      for (int i = 0; i < 14; i++) {

        Serial.print(myData[i], HEX);
        Serial.print(' ');

      }

      Serial.println();

      VAC = (myData[0]*256+myData[1])*0.1;
      Output = (myData[2]*256+myData[3])*0.1;
      VDC = (myData[4]*256+myData[5])*0.125;
      TempConv = (myData[12]*256+myData[13]);
  
      // Convert the value to resistance
      TempConv = 1023 / TempConv - 1;
      TempConv = SERIESRESISTOR / TempConv;

      float steinhart;
      steinhart = TempConv / THERMISTORNOMINAL;         // (R/Ro)
      steinhart = log(steinhart);                       // ln(R/Ro)
      steinhart /= BCOEFFICIENT;                        // 1/B * ln(R/Ro)
      steinhart += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
      steinhart = 1.0 / steinhart;                      // Invert
      steinhart -= 273.15;                              // Convert absolute temp to C
      TempF = ((steinhart)*1.8)+32;                     // Convert C to F

        Serial.println("");
        Serial.print("Line Voltage: ");
        Serial.print(VAC, 1);
        Serial.println(" VAC");

        Serial.print("Output: ");
        Serial.print(Output, 1);
        Serial.println(" W");

        Serial.print("Solar Voltage: ");
        Serial.print(VDC, 1);
        Serial.println(" VDC");

        Serial.print("Thermistor resistance: "); 
        Serial.print((TempConv) / 1000, 3);
        Serial.println(" K Ohms"); 

        Serial.print("Temperature: ");
        Serial.print(steinhart, 1);
        Serial.println(" C");

        Serial.print("Temperature: ");
        Serial.print(TempF, 1);
        Serial.println(" F");
        Serial.println("");
      }
    }
  }

void loop() {

  recvBytesWithStartMarker();

}