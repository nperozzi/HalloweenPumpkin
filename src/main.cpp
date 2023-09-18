/*
HalloweenPumpkinV3
Decription:
  A HalloweenPumpkin that reproduces "tracks" (sound and a light sequence) when triggered via bluetooth.
  The bluetooth transmitor the MIT App Inventory app "HalloweenPumpkinAppV2"

Hardware:
  This code has been made to work on a Teensy 3.5 according to the schematics in the project files.
Version:
  Code migrated to PlatformIO

  TODO: Add "U can´t touch this by MC Hummer"
*/ 
#include "helper.h"

//Desabling Interrupts
//#define FASTLED_ALLOW_INTERRUPTS 0  //NOTE: seems to work without desabling interrupts.

//SD Card elements:
#define BUILTIN_SD_CARD BUILTIN_SDCARD
#define SDCARD_MOSI_PIN 11
#define SDCARD_SCK_PIN 13

void setup()
{
  //Bluetooth Serial setup
  Serial1.begin(9600);  //Defining communication rate with the HC-06 bluetooth module. Boud rate is 9600
  Serial.begin(9600);   //SERIAL FOR DEBUGGING
  
  //Audio setup:
  AudioMemory(8);

  //SD Card setup:
  SPI.setMOSI(SDCARD_MOSI_PIN);
  SPI.setSCK(SDCARD_SCK_PIN);
  if (!SD.begin(BUILTIN_SD_CARD))                                                 //TODO: This message has no effect on the app.
  {
    Serial1.println("Unable to access the SD card");
    delay(500);
  }
  
  //FastLED Setup:
  FastLED.addLeds<WS2812, LED1_PIN, RGB> (Strip1, NUM_LED1); //Candles Strip setup
  FastLED.addLeds<WS2812, LED2_PIN, RGB> (Strip2, NUM_LED2); //MiniPumpkins Strip setup
  FastLED.setBrightness(50);

  //Volume Setup:
  attachInterrupt(digitalPinToInterrupt(0), Vol, FALLING);
  amp1.gain(vol);   //vol is a float. vol=0 is not sound. 0.0<vol<1.0 atenuation.
  Serial1.println(vol);                                                          //TODO: make sure that the app does something with this
}

void loop()
{
  while(Serial1.available())
  {
    getSerialData();
    switch(serialData[0])
    {
    //Track: Boo!
    case 1:
      Play(serialData[0], CRGB::White);
      break;
    //Track: Thriler01
    case 7:
      Play(serialData[0], 0);
      break;
    }
  }
  Candles();
  cleanSerialData();
}