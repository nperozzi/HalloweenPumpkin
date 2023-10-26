/*
HalloweenPumpkinV3
*/
#include "helper.h"

//SD Card elements:
#define BUILTIN_SD_CARD BUILTIN_SDCARD
#define SDCARD_MOSI_PIN 11
#define SDCARD_SCK_PIN 13

SerialData myData;
int savedLight = 0;
bool eyeIsOpen = false;

void setup()
{
  //Bluetooth Serial setup
  Serial1.begin(9600);  //Defining communication rate with the HC-06 bluetooth module. Boud rate is 9600
  Serial.begin(9600);   //SERIAL FOR DEBUGGING
  
  //Audio setup:
  AudioMemory(16);

  //FastLED Setup:
  FastLED.addLeds<WS2812, LED1_PIN, GRB> (Strip1, NUM_LED1); //Candles Strip setup
  FastLED.addLeds<WS2812, LED2_PIN, GRB> (Strip2, NUM_LED2); //MiniPumpkins Strip setup
  FastLED.setBrightness(70);
  FastLED.clear();

  //SD Card setup:
  SPI.setMOSI(SDCARD_MOSI_PIN);
  SPI.setSCK(SDCARD_SCK_PIN);
  SD.begin(BUILTIN_SD_CARD);

  //Save and send list of songs
  saveSongs();
  sendSongs();

  //Volume Setup:
  amp1.gain(vol);   //vol is a float. vol=0 is not sound. 0.0<vol<1.0 atenuation.

  //Setup eye
  pinMode(EYEBALL_PIN, OUTPUT);
  pinMode(EYELID_PIN, OUTPUT);  
  eyeBall.attach(EYEBALL_PIN);
  eyeLid.attach(EYELID_PIN);
  eyeBall.write(ballLeft);
  eyeLid.write(lidClosed);
}

void loop()
{
  if( readSerialData(myData) == true)
  {
    //Eye
    eyeIsOpen = smothEyeLid(myData, 0.60);
    if (myData.eyeball != 0 && eyeIsOpen)
    {
      smoothEyeBall(myData, 0.35);
    }
    
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
    }
  }
  else
  {
    lights(savedLight);
  }
}