
#include <Wire.h>
#include <Servo.h>
#include "StepperMotor.h"

#define ADDRESS 0x60                                   // Defines address of CMPS10

// Pin Definitions
#define STEPPER_PIN_STEP  9
#define STEPPER_PIN_DIR 8

#define stepperDelayTime  1000


// object initialization
StepperMotor stepper(STEPPER_PIN_STEP,STEPPER_PIN_DIR);

int lastBearing;


void setup(){
  Serial.begin(9600);
  Wire.begin();                                               // Conects I2C
  stepper.enable();                                           // enable the stepper motor, use .disable() to disable the motor
  stepper.setStepDelay(stepperDelayTime);                     // set stepper motor speed by changing the delay value, the higher the delay the slower the motor will turn
  lastBearing = getBearing();
}

void loop(){
  /*
  Serial.println(lastBearing);
  int currentBearing = getBearing();
  int diference = currentBearing - lastBearing;
  int diferenceAbs = abs(diference);
  lastBearing = currentBearing;
  Serial.println(diferenceAbs);
  if(abs(diference) > 0) {
    stepper.step(1, diferenceAbs);
  }*/
  //stepper.step(1, 1600);
  
  
  //stepper.step(1, currentBearing);  // move motor 1000 steps in one direction
  //delay(1000);            // short stop
  //stepper.step(0, 1000);  // move motor 1000 steps in the other dirction
  //delay(1000);            //short stop
  //Serial.println(currentBearing);
  
  delay(100);
}

int getBearing() {
  byte highByte, lowByte, fine;              // highByte and lowByte store high and low bytes of the bearing and fine stores decimal place of bearing
  char pitch, roll;                          // Stores pitch and roll values of CMPS10, chars are used because they support signed value
  int bearing;                               // Stores full bearing
  
  Wire.beginTransmission(ADDRESS);           //starts communication with CMPS10
  Wire.write(2);                              //Sends the register we wish to start reading from
  Wire.endTransmission();
  
  Wire.requestFrom(ADDRESS, 4);              // Request 4 bytes from CMPS10
  while(Wire.available() < 4);               // Wait for bytes to become available
  highByte = Wire.read();           
  lowByte = Wire.read();            
  pitch = Wire.read();              
  roll = Wire.read();               
  
  bearing = ((highByte<<8)+lowByte)/10;      // Calculate full bearing
  fine = ((highByte<<8)+lowByte)%10;         // Calculate decimal place of bearing
  
  Serial.print("bearing = ");
  Serial.println(bearing);
  
  return bearing;
}
