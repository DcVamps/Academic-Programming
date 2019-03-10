#include <TimerOne.h>
#include <SPI.h>
#include "Neophob_LPD6803.h"

/*
Unlike software SPI which is configurable, hardware SPI works only on very specific pins. 

On the Arduino Uno, Duemilanove, etc., clock = pin 13 and data = pin 11. 
For the Arduino Mega, clock = pin 52, data = pin 51. 
For the ATmega32u4 Breakout Board and Teensy, clock = pin B1, data = B2. 

Alternately, on most boards you can use the 6-pin programming 
header for SPI output as well, in which case clock = pin 3 and data = pin 4.

src: http://forums.adafruit.com/viewtopic.php?f=47&t=24256 
thanks phil!
*/

#define LED_MODULES 50

Neophob_LPD6803 strip = Neophob_LPD6803(LED_MODULES);

int analogPin1 = 1;
int analogPin2 = 3;
int leftVal = 0;
int leftLevel;
int rightVal = 0;
int rightLevel;

uint32_t RED;
uint32_t GREEN;
uint32_t BLUE;
uint32_t CYAN;
uint32_t MAGENTA;
uint32_t YELLOW;
uint32_t WHITE;
uint32_t CLEAR;

void setup() {
  RED = Color(0,0,31);
  GREEN = Color(31,0,0);
  BLUE = Color(0,31,0);
  CYAN = Color(31,31,0);
  YELLOW = Color(31,0,31);
  MAGENTA = Color(0,31,31);
  WHITE = Color(31,31,31);
  CLEAR = Color(0,0,0);
  
  //just play with those values!
  Serial.begin(9600);
  strip.setCPUmax(50);
  strip.begin(SPI_CLOCK_DIV64);
  strip.show();
}


void loop() {

  //music analog read in
  leftVal = analogRead(analogPin1);
  rightVal = analogRead(analogPin2);
    
  leftLevel = leftVal/12;
  rightLevel = rightVal/12;

  switch(7)
  {
    case 4: equalizer(WHITE);delay(50); break;
    case 2: colorWipe(RED, 50);//g
            colorWipeRev(GREEN, 50);//b
            colorWipe(BLUE, 50);//r
            colorWipeRev(WHITE,50);//w
            break;
    case 3: strip.setPixelColor(45, CYAN);
            strip.setPixelColor(3, YELLOW);
            strip.show();
            break;
    case 5: rainbowCycle(50); break;
    case 6: rainbow(5); break;
    case 7: colorWheel(50); break;
  }
}


void equalizer (uint8_t c)
{

  int i;
  for(i=0;i<LED_MODULES;i++)
  {
     strip.setPixelColor(i,CLEAR);
  }
  for(i=0;i<leftLevel;i++)
  {
    strip.setPixelColor(i,c);
  }
  for(i=0;i<rightLevel;i++)
  {
    strip.setPixelColor(strip.numPixels()-1-i,c);
  }
  strip.show();
  //delay(75);
}

// Slightly different, this one makes the rainbow wheel equally distributed 
// along the chain
void rainbowCycle(uint8_t wait) {
  int i, j;
  
  for (j=0; j < 96 * 5; j++) {     // 5 cycles of all 96 colors in the wheel
    for (i=0; i < strip.numPixels(); i++) {
      // tricky math! we use each pixel as a fraction of the full 96-color wheel
      // (thats the i / strip.numPixels() part)
      // Then add in j which makes the colors go around per pixel
      // the % 96 is to make the wheel cycle around
      strip.setPixelColor(i, Wheel( ((i * 96 / strip.numPixels()) + j) % 96) );
    }  
    strip.show();   // write all the pixels out
    delay(wait);
  }
}


void solidColor(uint32_t c)
{
  int i;
  
  for (i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
  }
  strip.show();
}

// fill the dots one after the other with said color
// good for testing purposes
void colorWipe(uint32_t c, uint8_t wait) {
  int i;
  
  for (i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

void colorWipeRev(uint32_t c, uint8_t wait) {
  int i;
  
  for (i=strip.numPixels(); i > 0 ; i--) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

void colorWheel(uint8_t wait)
{
  int numpos = LED_MODULES/7;
  int i,j,pos;
  int color = 1;
  uint32_t nextColor;

  for(pos=0;pos<7;pos++)
  {
    for(i=0;i<numpos;i++)
    {
      color++;
      for(j=0;j<LED_MODULES;j++)
      {
        switch(j)
        {
          case 7: color++; break;
          case 14: color++; break;
          case 21: color++; break;
          case 28: color++; break;
          case 35: color++; break;
          case 42: color++; break;
          case 49: color++; break;
        }
        if(color>7)
        {
          color=1;
        }
        switch(color)
        {
          case 2: nextColor = RED; break;
          case 3: nextColor = YELLOW; break;
          case 4: nextColor = WHITE; break;
          case 5: nextColor = GREEN; break;
          case 6: nextColor = CYAN; break;
          case 7: nextColor = BLUE; break;
          case 1: nextColor = MAGENTA; break;
        }
        strip.setPixelColor(j,nextColor);
        strip.show();
        delay(wait);
      }
      delay(wait);
      //delay(300);
      //strip.show(); 
    }
    delay(wait);
  }
  //delay(wait);
}


void rainbow(uint8_t wait) {
  int i, j;
   
  for (j=0; j < 96 * 3; j++) {     // 3 cycles of all 96 colors in the wheel
    for (i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel( (i + j) % 96));
    }  
    strip.show();   // write all the pixels out
    delay(wait);
  }
}

// Create a 15 bit color value from R,G,B
unsigned int Color(byte r, byte g, byte b)
{
  //Take the lowest 5 bits of each value and append them end to end
  return( ((unsigned int)g & 0x1F )<<10 | ((unsigned int)b & 0x1F)<<5 | (unsigned int)r & 0x1F);
}


//Input a value 0 to 127 to get a color value.
//The colours are a transition r - g -b - back to r
unsigned int Wheel(byte WheelPos)
{
  byte r,g,b;
  switch(WheelPos >> 5)
  {
    case 0:
      r=31- WheelPos % 32;   //Red down
      g=WheelPos % 32;      // Green up
      b=0;                  //blue off
      break; 
    case 1:
      g=31- WheelPos % 32;  //green down
      b=WheelPos % 32;      //blue up
      r=0;                  //red off
      break; 
    case 2:
      b=31- WheelPos % 32;  //blue down 
      r=WheelPos % 32;      //red up
      g=0;                  //green off
      break; 
  }
  return(Color(r,g,b));
}
