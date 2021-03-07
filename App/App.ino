#include "CJMCUS6814.hpp"

#define PIN_CO A0
#define PIN_NO2 A2
#define PIN_NH3 A1

CJMCUS6814 gas(PIN_CO, PIN_NO2, PIN_NH3);

void setup()
{
  Serial.begin(115200);

  Serial.println("CJMCUS-6814 Sensor Sample");
  Serial.print("Calibrating Sensor");

  gas.calibrate();

  Serial.println("OK!");
}

void loop()
{

  gas.print(CO);
  delay(50);
  gas.print(NO2);
  delay(50);
  gas.print(NH3);
  delay(50);


  delay(3000);
}
