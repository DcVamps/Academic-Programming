#include <Adafruit_MotorShield.h>
#include <Wire.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

//Analog pins for reading battery voltage
int batteryGroup1 = A0;
int batteryGroup2 = A1;
int battery1Level = 0;
int battery2Level = 0;
bool sentLOW = false;
bool sentHIGH = false;

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *leftDrive = AFMS.getMotor(1);
Adafruit_DCMotor *rightDrive = AFMS.getMotor(3);
Adafruit_DCMotor *vacuumDrive = AFMS.getMotor(2);

int TIME180 = 25;
int TIME90 = 25;
int TIME45 = 25;
int TIME15 = 25;

void setup() {
  AFMS.begin();
  leftDrive->setSpeed(0);
  rightDrive->setSpeed(0);
  vacuumDrive->setSpeed(0);
  Wire.begin(50);
  Wire.onReceive(reciveEvent);

  //interupt pin
  pinMode(12, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(25);
  //Test code for determing turn times
  /*turnLeft(TIME90,150);
  delay(10000);*/
///*
  battery1Level = analogRead(batteryGroup1);
  battery1Level = analogRead(batteryGroup2);
  if((battery1Level <= 707 || battery2Level <= 707) && !sentLOW){
    digitalWrite(12, HIGH);
    delay(15);
    Wire.write("LB");
    digitalWrite(12,LOW);
    sentLOW = true;
    sentHIGH = false;
  }
  if((battery1Level >= 860 || battery2Level >= 860) && !sentHIGH){
    digitalWrite(12, HIGH);
    delay(15);
    Wire.write("HB");
    digitalWrite(12,LOW);
    sentLOW = false;
    sentHIGH = true;
    //*/
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
    movement(150,true);
    
  }
  //got straight at half speed
  else if (rec == 0x03){
    movement(75,true);
  }
  //turn 180 on the spot
  else if (rec == 0x04){
    turnLeft(TIME180,150);
  }
  //turn 90 left on the spot
  else if (rec == 0x05){
    turnLeft(TIME90,150);
  }
  //turn 45 left on the spot
  else if (rec == 0x06){
    turnLeft(TIME45,150);
  }
  //turn 90 right on the spot
  else if (rec == 0x07){
    turnRight(TIME90,150);
  }
  //turn 45 right on the spot
  else if (rec == 0x08){
    turnRight(TIME45, 150);
  }
  //move reverse at normal speed
  else if (rec == 0x09){
   movement(150,false);;
  }
  //move reverse at half speed
  else if (rec == 0x0a){
    movement(75,false);
  }
  //stop
  else if (rec == 0x0b){
    movement(0,true);
  }
  //special turn 15 for mapping
  else if(rec == 0xff){
    turnLeft(TIME15, 150);
  }
}

void turnLeft (int seconds, int desiredSpeed){
   leftDrive->setSpeed(desiredSpeed);
   rightDrive->setSpeed(desiredSpeed);
   leftDrive->run(BACKWARD);
   rightDrive->run(FORWARD);
   delay(seconds);//time it takes to turn 
   movement(0,true);
   Wire.write(1);
}

void turnRight (int seconds, int desiredSpeed){
   leftDrive->setSpeed(desiredSpeed);
   rightDrive->setSpeed(desiredSpeed);
   leftDrive->run(FORWARD);
   rightDrive->run(BACKWARD);
   delay(seconds);//time it takes to turn 
   movement(0,true);
   Wire.write(1);
}

void movement(int desiredSpeed, bool dir){
  if (dir){ // forward novement is true
    leftDrive->setSpeed(desiredSpeed);
    rightDrive->setSpeed(desiredSpeed);
    leftDrive->run(FORWARD);
    rightDrive->run(FORWARD);
  }
  else{ //reverse movement is false
    leftDrive->setSpeed(desiredSpeed);
    rightDrive->setSpeed(desiredSpeed);
    leftDrive->run(BACKWARD);
    rightDrive->run(BACKWARD);
  }
}

