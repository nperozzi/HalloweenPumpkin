/* TEST TEXT
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

  //FastLED Setup:
  FastLED.addLeds<WS2812, LED1_PIN, RGB> (Strip1, NUM_LED1); //Candles Strip setup
  FastLED.addLeds<WS2812, LED2_PIN, RGB> (Strip2, NUM_LED2); //MiniPumpkins Strip setup
  FastLED.setBrightness(50);
  fill_solid(Strip1, NUM_LED1, CRGB::Black);  //TODO: light do not start att off.
  fill_solid(Strip2, NUM_LED2, CRGB::Black);

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
  getSerialData();
  volume();
  if(playSdWav1.isPlaying() != true)
  {
    //move servos
    eyeBall.write(serialData[2]);
    eyeLid.write(serialData[3]);

    //Test to play song
    if(serialData[0] != 0 && serialData[0] <= songs_count)   //TODO: If the serialData would be reliable, I do not need to check for song count
    {
      //Save playing values and play
      for (int i = 0 ; i <= 2; i++)
      {
        playingData[i] = serialData[i];
        serialData[i] = 0;
      }
      playSdWav1.play(songs[playingData[0]-1]);
      delay(20);
    }
    else
    {
      Candles();
    }
  }
  //While playing song, play lights:
  if(playSdWav1.isPlaying() == true)
  {
    //move servos
    eyeBall.write(serialData[2]);
    eyeLid.write(serialData[3]);
    
    switch(playingData[1])
    {
      case 1:
        flash.runFlashSequence(CRGB::White,60);
        fill_solid(Strip2, NUM_LED2, CRGB::Black);
        break;
      case 2:
        flash.runFlashSequence(CRGB::Red,60);
        fill_solid(Strip2, NUM_LED2, CRGB::Black);
        break;
      case 3:
        flash.runFlashSequence(CRGB::Green,60);
        fill_solid(Strip2, NUM_LED2, CRGB::Black);
        break;
      case 4:
        flash.runFlashSequence(CRGB::Purple,60);
        fill_solid(Strip2, NUM_LED2, CRGB::Black);
        break;
      case 5:
        FlashMultiColor();
        break;
      case 6:
        mountainKing.runMountainKingSequence(500);
        break;
    }
  }
  delay(20);
  //cleanPlayingData();
}