//All this does is read the heading from HMC6352 and spit it out via serial

#include <Wire.h>
#include <Servo.h>

Servo myServo;  // create servo object to control a servo

int HMC6352SlaveAddress = 0x42;
int HMC6352ReadAddress = 0x41; //"A" in hex, A command is: 

int headingValue;

int servoPin = 9;    // Connect servo with pin 9
int MagnetPos; // variable to read the value from the analog pin
int ServoDegree; // variable to hold the angle for the servo motor

void setup(){
  // "The Wire library uses 7 bit addresses throughout. 
  //If you have a datasheet or sample code that uses 8 bit address, 
  //you'll want to drop the low bit (i.e. shift the value one bit to the right), 
  //yielding an address between 0 and 127."
  HMC6352SlaveAddress = HMC6352SlaveAddress >> 1; // I know 0x42 is less than 127, but this is still required

  Serial.begin(9600);
  myServo.attach(9); 
  Wire.begin();
}

void loop(){
  //"Get Data. Compensate and Calculate New Heading"
  Wire.beginTransmission(HMC6352SlaveAddress);
  Wire.write(HMC6352ReadAddress);              // The "Get Data" command
  Wire.endTransmission();

  //time delays required by HMC6352 upon receipt of the command
  //Get Data. Compensate and Calculate New Heading : 6ms
  delay(6);

  Wire.requestFrom(HMC6352SlaveAddress, 2); //get the two data bytes, MSB and LSB

  //"The heading output data will be the value in tenths of degrees
  //from zero to 3599 and provided in binary format over the two bytes."
  byte MSB = Wire.read();
  byte LSB = Wire.read();

  float headingSum = (MSB << 8) + LSB; //(MSB / LSB sum)
  float headingInt = headingSum / 10; 
  Serial.print(headingInt);
  Serial.println(" degrees");

  MagnetPos = analogRead(headingInt);// read the value of the magnet
  Serial.print(" MagnetPos: ");
  Serial.print(MagnetPos);
 
  ServoDegree = map(MagnetPos, 0, 380, 0, 179);
  // print out the angle for the servo motor
  Serial.print(", ServoDegree: ");
  Serial.println(ServoDegree);

  // set the servo position
  myServo.write (ServoDegree); 
  
 
  
  delay(500);
}
