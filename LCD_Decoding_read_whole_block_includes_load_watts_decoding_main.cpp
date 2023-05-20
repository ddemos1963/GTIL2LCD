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


byte myData[300];

float VAC;
float Output;
float Load;
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

    byte y0 = SSerial.read();
    byte y1 = SSerial.read();

    unsigned int y = y0 << 8 | y1;

    if (y == 0x0707) { // Sync found

      SSerial.readBytes(myData, 296);

      for (int i = 0; i < 296; i++)

//        Serial.print(myData[i], HEX);
//         Serial.print(' ');
//         Serial.println();
 

      VAC = (myData[278]*256+myData[279])*0.1;
      Output = (myData[280]*256+myData[281])*0.1;
      VDC = (myData[282]*256+myData[283])*0.125;
      TempConv = (myData[290]*256+myData[291]);
      Load = (myData[294]*256+myData[295])*0.1;


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
        Serial.print(F("Line Voltage: "));
        Serial.print(VAC, 1);
        Serial.println(F(" VAC"));

        Serial.print(F("Output: "));
        Serial.print(Output, 1);
        Serial.println(F(" W"));

        Serial.print(F("AC Load: "));
        Serial.print(Load, 1);
        Serial.println(" W");

        Serial.print(F("Solar Voltage: "));
        Serial.print(VDC, 1);
        Serial.println(F(" VDC"));

        Serial.print(F("Thermistor resistance: ")); 
        Serial.print((TempConv) / 1000, 3);
        Serial.println(F(" K Ohms")); 

        Serial.print(F("Temperature: "));
        Serial.print(steinhart, 1);
        Serial.println(F(" C"));

        Serial.print(F("Temperature: "));
        Serial.print(TempF, 1);
        Serial.println(F(" F"));
        Serial.println("");

      }
    }
  }

void loop() {

  recvBytesWithStartMarker();

}