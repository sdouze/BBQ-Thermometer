#include "SevSeg.h"
SevSeg sevseg;
// the value of the 'other' resistor
#define resistor 50000    
 
// What pin to connect the sensor to
#define thermistorPin A0 
// Room Temp
#define room_Temp 25
// Beta
#define beta 4036
// Room temp res
#define room_temp_res 50000

void setup(void) {
  //Setup for 7SegDisplay
  byte numDigits = 4;
  byte digitPins[] = {10, 11, 12, 13};
  byte segmentPins[] = {9, 2, 3, 5, 6, 8, 7, 4};
  bool resistorsOnSegments = true;
  byte hardwareConfig = COMMON_CATHODE;
  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments);
  Serial.begin(9600);
  analogReference(EXTERNAL);
}
 
void loop(void) {
  float reading;
 
  reading = analogRead(thermistorPin);
  // convert the value to resistance
  reading = (1023 / reading)  - 1;     // (1023/ADC - 1) 
  reading = resistor / reading;  // 50K / (1023/ADC - 1) 
  float stein;
  stein = steinhartEq(reading);
  
  static unsigned long timer = millis();

  if (millis() >= timer) {
    timer += 1000; //change back to 300 for use 
    sevseg.setNumber(stein, 1);
    Serial.print(stein);
    Serial.print("\n");
  }
  
  sevseg.refreshDisplay();
}


//Converting thermistor reading to temperature using Steinhart Equation
float steinhartEq(float reading){
  float steinhart;
  steinhart = reading / room_temp_res;
  steinhart = log(steinhart);
  steinhart = steinhart / beta;
  steinhart = steinhart + (1.0 / (room_Temp + 273.15));
  steinhart = 1.0 / steinhart;
  steinhart = steinhart - 273.15;
  return steinhart;

}
