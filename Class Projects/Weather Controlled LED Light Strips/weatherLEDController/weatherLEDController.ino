#include <TimerOne.h>
#include <SPI.h>
#include "Neophob_LPD6803.h"
#include <SoftwareSerial.h>
#include <Wire.h>

/*

John Weiser, Andrew Cicchinelli
LED Controller with weather network input from rpi
designed for LPD6803 LED controller modules

Comment header from example code:

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
#define SLAVE_ADDRESS 0x04

Neophob_LPD6803 strip = Neophob_LPD6803(LED_MODULES);

int LEDBuffer[LED_MODULES];
int count = 0;
int color = 1;

int shift = 0;
int readmode = 0;

uint32_t RED;
uint32_t GREEN;
uint32_t BLUE;
uint32_t CYAN;
uint32_t MAGENTA;
uint32_t YELLOW;
uint32_t WHITE;
uint32_t CLEAR;

int temp;
bool up;
int weather;
/* encoding for weather descriptors
sunny = 0
cloudy = 1
rainy = 2
snowy = 3
clear = 4
foggy, misty, hazy = 5 
*/

void setup() {
  RED = Color(0,0,31);
  GREEN = Color(31,0,0);
  BLUE = Color(0,31,0);
  CYAN = Color(31,31,0);
  YELLOW = Color(31,0,31);
  MAGENTA = Color(0,31,31);
  WHITE = Color(31,31,31);
  CLEAR = Color(0,0,0);

  temp = 120;
  up = true;
  weather = 0;

 Serial.begin(9600);
  while (!Serial){;};
  Wire.begin(SLAVE_ADDRESS);
  //Serial3.begin(9600);
  strip.setCPUmax(50);
  strip.begin(SPI_CLOCK_DIV64);
  strip.show();

  Wire.onReceive(receiveData);
}


void loop()
{
  //test code for modules 
  //tempgrad();
  //solidColor(RED,true);
  //Sunny();
  //Snowy();
  //Rainy();
  //Cloudy();
  //Foggy();
  
  full_strip_temp_gradient(temp);
  
  if(weather == 0)
  {
    Sunny();
  }
  else if(weather == 1)
  {
    Cloudy();
  }
  else if(weather == 2)
  {
    Rainy();
  }
  
  else if(weather == 3)
  {
    Snowy();
  }
  else if(weather == 4)
  {
    Clear();
  }
  else
  {
    Foggy();
  }
  strip.show();
}

void receiveData(int byteCount)
{
  int definer = 0;
  while(Wire.available())
  {
    definer = Wire.read();//clear bad data
    definer = Wire.read();
    if(definer == 250 && readmode == 2)//weather
    {
      readmode = 3;
      delay(500);
    }
    else if(readmode == 3)
    {
      int defined = definer;
      if(defined == 100)//sunny
      {
        weather = 0;
      }
      else if(defined == 10)//cloudy
      {
        weather = 1;
      }
      else if(defined == 20)//rainy
      {
        weather = 2;
      }
      else if(defined == 30)//snowy
      {
        weather = 3;
      }
      else if(defined == 40)//clear
      {
        weather = 4;
      }
      else//other
      {
        weather = 5;
      }
      readmode = 0;
    }
    else if(definer == 240 && readmode == 0)//temp
    {
      delay(500);
      readmode = 1;
    }
    else if(readmode == 1)
    {
      temp = definer;
      readmode = 2;
    }
  }
}

void tempgrad()
{
  if(up)
  {
    temp++;
    if (temp == 128)
    {
      up = false;
    }
  } 
  else
  {
    temp--;
    if(temp == -64)
    {
      up = true;
    }
  }
}

void Cloudy()
{
  for (int i=0; i < strip.numPixels()/4; i+=4)
  {
      strip.setPixelColor(i, Color(0,0,0));
      strip.setPixelColor(i+1, Color(7,7,5));
      strip.setPixelColor(i+2, Color(15,15,9));
      strip.setPixelColor(i+3, Color(7,7,5));
  }
  for (int i=(strip.numPixels()*3)/4; i < strip.numPixels(); i+=4)
  {
      strip.setPixelColor(i, Color(0,0,0));
      strip.setPixelColor(i+1, Color(7,7,5));
      strip.setPixelColor(i+2, Color(15,15,9));
      strip.setPixelColor(i+3, Color(7,7,5));
  }
}

void Foggy()
{
  for (int i=0; i < strip.numPixels()/4; i+=4)
  {
      strip.setPixelColor(i, Color(7,7,5));
      strip.setPixelColor(i+1, Color(15,15,9));
      strip.setPixelColor(i+2, Color(23,23,15));
      strip.setPixelColor(i+3, Color(15,15,9));
  }
  for (int i=(strip.numPixels()*3)/4; i < strip.numPixels(); i+=4)
  {
      strip.setPixelColor(i, Color(7,7,5));
      strip.setPixelColor(i+1, Color(15,15,9));
      strip.setPixelColor(i+2, Color(23,23,15));
      strip.setPixelColor(i+3, Color(15,15,9));
  }
}

void Rainy()
{
  for (int i=0; i < strip.numPixels()/4; i+=4)
  {
    if(shift==0)
    {
      strip.setPixelColor(i,Color(0,0,0));
      strip.setPixelColor(i+1,Color(0,7,0));
      strip.setPixelColor(i+2,Color(0,15,0));
      strip.setPixelColor(i+3,Color(0,31,0));
    }
    else if(shift==1)
    {
      strip.setPixelColor(i,Color(0,7,0));
      strip.setPixelColor(i+1,Color(0,15,0));
      strip.setPixelColor(i+2,Color(0,31,0));
      strip.setPixelColor(i+3,Color(0,0,0));
    }
    else if(shift==2)
    {
      strip.setPixelColor(i,Color(0,15,0));
      strip.setPixelColor(i+1,Color(0,31,0));
      strip.setPixelColor(i+2,Color(0,0,0));
      strip.setPixelColor(i+3,Color(0,7,0));
    }
    else if(shift==3)
    {
      strip.setPixelColor(i,Color(0,31,0));
      strip.setPixelColor(i+1,Color(0,0,0));
      strip.setPixelColor(i+2,Color(0,7,0));
      strip.setPixelColor(i+3,Color(0,15,0));
    }
  }
  for (int i=(strip.numPixels()*3)/4; i < strip.numPixels(); i+=4)
  {
    if(shift==0)
    {
      strip.setPixelColor(i,Color(0,0,0));
      strip.setPixelColor(i+1,Color(0,7,0));
      strip.setPixelColor(i+2,Color(0,15,0));
      strip.setPixelColor(i+3,Color(0,31,0));
    }
    else if(shift==1)
    {
      strip.setPixelColor(i,Color(0,7,0));
      strip.setPixelColor(i+1,Color(0,15,0));
      strip.setPixelColor(i+2,Color(0,31,0));
      strip.setPixelColor(i+3,Color(0,0,0));
    }
    else if(shift==2)
    {
      strip.setPixelColor(i,Color(0,15,0));
      strip.setPixelColor(i+1,Color(0,31,0));
      strip.setPixelColor(i+2,Color(0,0,0));
      strip.setPixelColor(i+3,Color(0,7,0));
    }
    else if(shift==3)
    {
      strip.setPixelColor(i,Color(0,31,0));
      strip.setPixelColor(i+1,Color(0,0,0));
      strip.setPixelColor(i+2,Color(0,7,0));
      strip.setPixelColor(i+3,Color(0,15,0));
    }
  }
  shift++;
  if(shift==4)
  {
    shift=0;
  }
}

void Snowy()
{
  for (int i=0; i < strip.numPixels()/4; i+=4)
  {
    if(shift==0)
    {
      strip.setPixelColor(i,Color(0,0,0));
      strip.setPixelColor(i+1,Color(7,7,5));
      strip.setPixelColor(i+2,Color(31,31,15));
      strip.setPixelColor(i+3,Color(7,7,5));
    }
    else if(shift==3)
    {
      strip.setPixelColor(i,Color(7,7,5));
      strip.setPixelColor(i+1,Color(31,31,15));
      strip.setPixelColor(i+2,Color(7,7,5));
      strip.setPixelColor(i+3,Color(0,0,0));
    }
  }
  for (int i=(strip.numPixels()*3)/4; i < strip.numPixels(); i+=4)
  {
    if(shift==0)
    {
      strip.setPixelColor(i,Color(0,0,0));
      strip.setPixelColor(i+1,Color(7,7,5));
      strip.setPixelColor(i+2,Color(31,31,15));
      strip.setPixelColor(i+3,Color(7,7,5));
    }
    else if(shift==3)
    {
      strip.setPixelColor(i,Color(7,7,5));
      strip.setPixelColor(i+1,Color(31,31,15));
      strip.setPixelColor(i+2,Color(7,7,5));
      strip.setPixelColor(i+3,Color(0,0,0));
    }
  }
  shift++;
  if(shift==6)
  {
    shift=0;
  }
}

void Sunny()
{
  for (int i=0; i < strip.numPixels()/4; i++)
  {
      strip.setPixelColor(i, Color(15,0,31));
  }
  for (int i=(strip.numPixels()*3)/4; i < strip.numPixels(); i++)
  {
      strip.setPixelColor(i, Color(15,0,31));
  }
}

void Clear()
{
  for (int i=0; i < strip.numPixels()/4; i++)
  {
      strip.setPixelColor(i, Color(31,31,31));
  }
  for (int i=(strip.numPixels()*3)/4; i < strip.numPixels(); i++)
  {
      strip.setPixelColor(i, Color(31,31,31));
  }
}

void full_strip_temp_gradient(int temp)
{
  byte r = 0;
  byte g = 0;
  byte b = 0;
  if(temp<=-64)
  {
    b = 31;
  }
  else if(temp>-64&&temp<=0)
  {
    b = 31;
    g = (temp+64)/2;
  }
  else if(temp>0&&temp<=63)
  {
    g = 31;
    r = (temp)/2;
    b = (63-temp)/2;
  }
  else if(temp>63&&temp<127)
  {
    r = 31;
    g = (127-temp)/2;
  }
  else
  {
    r = 31;
  }
  for (int i=strip.numPixels()/4; i < (strip.numPixels()*3)/4; i++) {
      strip.setPixelColor(i, Color(g,b,r));
  }
}

void solidColor(uint32_t c, boolean disp)
{
  int i;
  
  for (i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
  }
  if(disp)
  strip.show();
}

// Create a 15 bit color value from R,G,B
unsigned int Color(byte r, byte g, byte b)
{
  //Take the lowest 5 bits of each value and append them end to end
  return( ((unsigned int)g & 0x1F )<<10 | ((unsigned int)b & 0x1F)<<5 | (unsigned int)r & 0x1F);
}


