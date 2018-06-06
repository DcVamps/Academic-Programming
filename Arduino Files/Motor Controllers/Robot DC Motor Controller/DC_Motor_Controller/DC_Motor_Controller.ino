#include <Adafruit_MotorShield.h>
#include <Wire.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

void setup() {
  Adafruit_MotorShield AFMS = Adafruit_MotorShield();
  Adafruit_DCMotor *leftDrive = AFMS.getMotor(1);
  Adafruit_DCMotor *rightDrive = AFMS.getMotor(3);
  Adafruit_DCMotor *vacuumDrive = AFMS.getMotor(2);
  AFMS.begin();
  leftDrive->setSpeed(0);
  rightDrive->setSpeed(0);
  vacuumDrive->setSpeed(0);
  Wire.begin(50);
  Wire.onReceive(reciveEvent);
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(25);
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
    delay();//time it takes to turn a complete 180
    leftDrive->setSpeed(0);
    rightDrive->setSpeed(0);
    leftDrive->run(FORWARD);
    rightDrive->run(FORWARD);
    Wire.send(1)
  }
  //turn 90 left on the spot
  else if (rec == 0x05){
    leftDrive->setSpeed(150);
    rightDrive->setSpeed(150);
    leftDrive->run(BACKWARD);
    rightDrive->run(FORWARD);
    delay();//time it takes to turn 90 degrees
    leftDrive->setSpeed(0);
    rightDrive->setSpeed(0);
    leftDrive->run(FORWARD);
    rightDrive->run(FORWARD);
    Wire.send(1)
  }
  //turn 45 left on the spot
  else if (rec == 0x06){
    leftDrive->setSpeed(150);
    rightDrive->setSpeed(150);
    leftDrive->run(BACKWARD);
    rightDrive->run(FORWARD);
    delay();//time it takes to turn 45 degrees
    leftDrive->setSpeed(0);
    rightDrive->setSpeed(0);
    leftDrive->run(FORWARD);
    rightDrive->run(FORWARD);
    Wire.send(1)
  }
  //turn 90 right on the spot
  else if (rec == 0x07){
    leftDrive->setSpeed(150);
    rightDrive->setSpeed(150);
    leftDrive->run(FORWARD);
    rightDrive->run(BACKWARD);
    delay();//time it takes to turn 90 degrees
    leftDrive->setSpeed(0);
    rightDrive->setSpeed(0);
    leftDrive->run(FORWARD);
    rightDrive->run(FORWARD);
    Wire.send(1)
  }
  //turn 45 right on the spot
  else if (rec == 0x08){
    leftDrive->setSpeed(150);
    rightDrive->setSpeed(150);
    leftDrive->run(FORWARD);
    rightDrive->run(BACKWARD);
    delay();//time it takes to turn 45 degrees
    leftDrive->setSpeed(0);
    rightDrive->setSpeed(0);
    leftDrive->run(FORWARD);
    rightDrive->run(FORWARD);
    Wire.send(1)
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
}

