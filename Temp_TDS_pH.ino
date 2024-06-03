#include <EEPROM.h>
#include "GravityTDS.h"
#include <OneWire.h>
#include <DallasTemperature.h>

#define TdsSensorPin A1
#define TempSensorPin 7
#define PHPin A0

GravityTDS gravityTds;
OneWire oneWire(TempSensorPin);
DallasTemperature tempSensors(&oneWire);

float temperature = 25,tdsValue = 0;

int buf[10];
float ph(float voltage) {
  return 7 + ((2.5 - voltage) / 0.18);
}

void setup() {
  Serial.begin(9600);

  gravityTds.setPin(TdsSensorPin);
  gravityTds.setAref(5.0); 
  gravityTds.setAdcRange(1024);  
  gravityTds.begin();  /

  pinMode(PHPin, INPUT);

  tempSensors.begin();
}

void loop() {
  //
  tempSensors.requestTemperatures();
  temperature = tempSensors.getTempCByIndex(0); 
  gravityTds.setTemperature(temperature); 
  gravityTds.update();  
  tdsValue = gravityTds.getTdsValue(); 

  for (int i = 0; i < 10; i++) {
    buf[i] = analogRead(PHPin);
    delay(100);
  }

  float avgValue = 0;
  for (int i = 0; i < 10; i++)
    avgValue += buf[i];

  float pHVol = (float)avgValue * 5.0 / 1023 / 10;
  float pHValue = -7.917 * pHVol + 34.65; //alat-1
  // float pHValue = -6.333 * pHVol + 26.24; //alat-2

  Serial.print("suhu:");
  Serial.println(temperature);
  Serial.print("tds:");
  Serial.println(tdsValue,0);
  Serial.print("ph:");
  Serial.println(pHValue);

  delay(1000);
}
