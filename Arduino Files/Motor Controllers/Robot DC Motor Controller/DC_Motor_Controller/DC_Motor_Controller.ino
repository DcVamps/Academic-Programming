#include <Adafruit_MotorShield.h>
#include <Wire.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

//Analog pins for reading battery voltage
int batteryGroup1 = A0;
int batteryGroup2 = A1;
int battery1Level = 0;
int battery2Level = 0;

//global turn times in ms
int TIME180 = 25;
int TIME90150 = 25;
int TIME45150 = 25;
int TIME15 = 25;

 Adafruit_MotorShield AFMS = Adafruit_MotorShield();
 Adafruit_DCMotor * leftDrive = AFMS.getMotor(1);
 Adafruit_DCMotor * rightDrive = AFMS.getMotor(3);
 Adafruit_DCMotor * vacuumDrive = AFMS.getMotor(2);


void setup() {
   AFMS.begin();
  leftDrive->setSpeed(0);
  rightDrive->setSpeed(0);
  vacuumDrive->setSpeed(0);
  Wire.begin(50);
  Wire.onReceive(reciveEvent);

  //interupt pin
  pinMode(1, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(25);

  battery1Level = analogRead(batteryGroup1);
  battery1Level = analogRead(batteryGroup2);
  if(battery1Level <= 707 || battery2Level <= 707 ){
    digitalWrite(1, HIGH);
    delay(15);
    Wire.write("LOWBAT");
    digitalWrite(1,LOW);
  }
}

void reciveEvent(){
  // Read in byte sent from RPi controller
  byte rec = Wire.read();
  // turn on the vacuum
  if (rec == 0x00){
    vacuumDrive->setSpeed(150);
    vacuumDrive->run(FORWARD);
  }
  //turn off the vacuum
  else if (rec == 0x01){
    vacuumDrive->setSpeed(0);
    vacuumDrive->run(FORWARD);
  }
  //go straight at normal speed
  else if (rec == 0x02){
    leftDrive->setSpeed(150);
    rightDrive->setSpeed(150);
    leftDrive->run(FORWARD);
    rightDrive->run(FORWARD);
  }
  //got straight at half speed
  else if (rec == 0x03){
    leftDrive->setSpeed(75);
    rightDrive->setSpeed(75);
    leftDrive->run(FORWARD);
    rightDrive->run(FORWARD);
  }
  //turn 180 on the spot
  else if (rec == 0x04){
    leftDrive->setSpeed(150);
    rightDrive->setSpeed(150);
    leftDrive->run(FORWARD);
    rightDrive->run(BACKWARD);
    delay(TIME180);//time it takes to turn a complete 180
    leftDrive->setSpeed(0);
    rightDrive->setSpeed(0);
    leftDrive->run(FORWARD);
    rightDrive->run(FORWARD);
    Wire.write(1);
  }
  //turn 90 left on the spot
  else if (rec == 0x05){
    leftDrive->setSpeed(150);
    rightDrive->setSpeed(150);
    leftDrive->run(BACKWARD);
    rightDrive->run(FORWARD);
    delay(TIME90150);//time it takes to turn 90 degrees
    leftDrive->setSpeed(0);
    rightDrive->setSpeed(0);
    leftDrive->run(FORWARD);
    rightDrive->run(FORWARD);
    Wire.write(1);
  }
  //turn 45 left on the spot
  else if (rec == 0x06){
    leftDrive->setSpeed(150);
    rightDrive->setSpeed(150);
    leftDrive->run(BACKWARD);
    rightDrive->run(FORWARD);
    delay(TIME45150);//time it takes to turn 45 degrees
    leftDrive->setSpeed(0);
    rightDrive->setSpeed(0);
    leftDrive->run(FORWARD);
    rightDrive->run(FORWARD);
    Wire.write(1);
  }
  //turn 90 right on the spot
  else if (rec == 0x07){
    leftDrive->setSpeed(150);
    rightDrive->setSpeed(150);
    leftDrive->run(FORWARD);
    rightDrive->run(BACKWARD);
    delay(TIME90150);//time it takes to turn 90 degrees
    leftDrive->setSpeed(0);
    rightDrive->setSpeed(0);
    leftDrive->run(FORWARD);
    rightDrive->run(FORWARD);
    Wire.write(1);
  }
  //turn 45 right on the spot
  else if (rec == 0x08){
    leftDrive->setSpeed(150);
    rightDrive->setSpeed(150);
    leftDrive->run(FORWARD);
    rightDrive->run(BACKWARD);
    delay(TIME45150);//time it takes to turn 45 degrees
    leftDrive->setSpeed(0);
    rightDrive->setSpeed(0);
    leftDrive->run(FORWARD);
    rightDrive->run(FORWARD);
    Wire.write(1);
  }
  //move reverse at normal speed
  else if (rec == 0x09){
    leftDrive->setSpeed(150);
    rightDrive->setSpeed(150);
    leftDrive->run(BACKWARD);
    rightDrive->run(BACKWARD);
  }
  //move reverse at half speed
  else if (rec == 0x0a){
    leftDrive->setSpeed(75);
    rightDrive->setSpeed(75);
    leftDrive->run(BACKWARD);
    rightDrive->run(BACKWARD);
  }
  //stop
  else if (rec == 0x0b){
    leftDrive->setSpeed(0);
    rightDrive->setSpeed(0);
    leftDrive->run(FORWARD);
    rightDrive->run(FORWARD);
  }
  else if(rec == 0xff){
    leftDrive->setSpeed(150);
    rightDrive->setSpeed(150);
    leftDrive->run(BACKWARD);
    rightDrive->run(FORWARD);
    delay(TIME15);//time it takes to turn 15 degrees
    leftDrive->setSpeed(0);
    rightDrive->setSpeed(0);
    leftDrive->run(FORWARD);
    rightDrive->run(FORWARD);
    Wire.write(1);
  }
}

