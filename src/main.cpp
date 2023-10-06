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

SerialData myData;
int savedLight = 0;

void setup()
{
  //Bluetooth Serial setup
  Serial1.begin(9600);  //Defining communication rate with the HC-06 bluetooth module. Boud rate is 9600
  Serial.begin(9600);   //SERIAL FOR DEBUGGING
  
  
  //Audio setup:
  AudioMemory(8);

  //FastLED Setup:
  FastLED.addLeds<WS2812, LED1_PIN, RGB> (Strip1, NUM_LED1); //Candles Strip setup
  FastLED.addLeds<WS2812, LED2_PIN, RGB> (Strip2, NUM_LED2); //MiniPumpkins Strip setup
  FastLED.setBrightness(50);
  FastLED.clear();

  //SD Card setup:
  SPI.setMOSI(SDCARD_MOSI_PIN);
  SPI.setSCK(SDCARD_SCK_PIN);
  SD.begin(BUILTIN_SD_CARD);

  //Save and send list of songs
  saveSongs();
  sendSongs();

  //Volume Setup:
  //attachInterrupt(digitalPinToInterrupt(0),getSerialData,CHANGE);
  amp1.gain(vol);   //vol is a float. vol=0 is not sound. 0.0<vol<1.0 atenuation.

  //Setup eye
  pinMode(EYEBALL_PIN, OUTPUT);
  pinMode(EYELID_PIN, OUTPUT);  
  eyeBall.attach(EYEBALL_PIN);  //TODO: I am not defining the PWM range, do I have to? 1000, 2000ms
  eyeLid.attach(EYELID_PIN);
}

void loop()
{
  if( readSerialData(myData) == true)
  {
    //Eye
    move(myData);
    
    //Volume
    volume(myData);

    //Light
    lights(savedLight);

    //Song
    if(myData.song != 0)
    {
      if(playSdWav1.isPlaying() == false)
      {
        savedLight = myData.light;
        playSdWav1.play(songs[myData.song-1]);
      }
      else
      {
        return;
      }
    }
    else
    {
      return;
    }
  }
  else
  {
    lights(savedLight);
  }
}