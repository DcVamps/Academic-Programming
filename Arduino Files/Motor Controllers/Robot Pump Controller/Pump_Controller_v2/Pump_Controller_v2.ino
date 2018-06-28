#include <Stepper.h>
#include <Wire.h>
#include <Servo.h>

const int stepsPerRevolution = 200;
Stepper myStepper(stepsPerRevolution, 8, 9, 10, 11);
int stepCount = 0;
Servo padPosition;


void setup() {
  padPosition.attach(2);
  padPosition.write(0);
  Wire.begin(40);
  Wire.onReceive(reciveEvent);

}

void loop() {
  // Wait for message
  delay(25);
}

void reciveEvent(){
  // Read in byte sent from RPi controller
  byte rec = Wire.read();
  // Turn Pump on, extend cleaning pad
  if(rec == 0x1){
    padPosition.write(45);
    int motorSpeed = map(100, 0, 1023, 0, 100);
    myStepper.setSpeed(motorSpeed);
    myStepper.step(stepsPerRevolution / 100);
  }
  // Turn Pump off, retract cleaning pad
  else if(rec == 0x0){
    padPosition.write(0);
    int motorSpeed = map(0, 0, 1023, 0, 100);
    myStepper.setSpeed(motorSpeed);
  }
}

