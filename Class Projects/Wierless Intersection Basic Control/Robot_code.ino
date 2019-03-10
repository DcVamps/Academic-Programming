#include <Motor.h>


//initialize left motor attached to Arduino pins 2 and 3
//pin 2 is for direction control
//pin 3 is for speed control
Motor motorLeft = Motor(2, 3);
//initialize left motor attached to Arduino pins 4 and 5
//pin 4 is for direction control
//pin 5 is for speed control
Motor motorRight = Motor(4, 5);
#define lightRightPin A0 // Analog 0
#define lightMidPin A1 // Analog 1
#define lightLeftPin A2 // Analog 2
//average value for each of the three photoresistors
//found in calibration mode,
//used to compare with newly read photoresistor
//values to determine the color
int leftAvg;
int midAvg;
int rightAvg;

void setup() {
 //Initialize Serial communications
 //If needed
 Serial.begin(9600);

 //Set up communication pins for communicating with the reciver arduino
 pinMode(12, OUTPUT);
 pinMode(13, INPUT);
 digitalWrite(12, LOW);
 //wait for the initial run command before looking at the light sensors
 while(digitalRead(13) == LOW){
  delay(5);
 }
 //move off of the starting black line
 motorLeft.go(50);
 motorRight.go(50);
 delay(500);
 motorLeft.stop();
 motorRight.stop();
}

int lightLeft;
int lightMid;
int lightRight;

void loop() {
  //hard coded vales for the average across all sensors
  leftAvg = 800;
  midAvg = 800;
  rightAvg = 800;
  //line following algorithm
  //read the sensor values
  lightLeft = analogRead(lightLeftPin);
  lightMid = analogRead(lightMidPin);
  lightRight = analogRead(lightRightPin);
 
    if (lightRight >= rightAvg and lightLeft >= leftAvg and lightMid >= midAvg){
      motorLeft.stop();
      motorRight.stop();
      //let the reciver know we have finished moving
      digitalWrite(12, HIGH);
      //wait until the reciver acknowledges the robot has stopped moving
      while(digitalRead(13) == HIGH){
        delay(5);
      }
      digitalWrite(12, LOW);
      //Wait for the next run command
      while(digitalRead(13) == LOW){
        delay(5);
      }
      //move off the blacktape
      motorLeft.go(50);
      motorRight.go(50);
      delay(500);
    }
    //if mid sensor detect black color,
    //drive straight forward
    else if (lightMid >= midAvg) { 
      motorLeft.go(60);
      motorRight.go(60);
      delay(5);
    }
    //if left sensor detect black color,
    //turn left a little bit
    else if (lightLeft >= leftAvg) {
      motorLeft.go(40);
      motorRight.go(60);
      delay(5);
    }
    //if right sensor detect black color,
    //turn right a little bit
    else if (lightRight >= rightAvg) {
      motorLeft.go(60);
      motorRight.go(40);
      delay(5);
    }
}


