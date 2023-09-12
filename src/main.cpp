/* HalloweenPumpkinV2.1Code-CURRENT
What is changing:
- Adding the ColorWheel functionality

Decription:
  A HalloweenPumpkin that reproduces "tracks" (sound and a light sequence) when triggered via bluetooth.
  The bluetooth transmitor the MIT App Inventory app "HalloweenPumpkinAppV2"

Hardware:
  This code has been made to work on a Teensy 3.5 according to the schematics in the project files.

VersionTracker:
V1.0: 
  - Sequences with audio and flashing LEDs
  - IR sensor
  - PIR sensor
V2.0:
  - MountainKing sequence has been added but not fully developed.
  - LED flash sequence has been added.
  - PIR sensor functionality has been removed.
  - Bluetooth has been added instead of IR sensor
  - App on MIT Inventor
  - Harry Potter Track
V2.1:
  - ColorWheel functionality
  - Changed the app to "HalloweenPumpkinAppV2"
*/ 

#include <Audio.h>
#include <SerialFlash.h>

#include <SPI.h>
#include <SD.h>

#include <FastLED.h>

#include "CustomMethods.h"

//Desabling Interrupts
  #define FASTLED_ALLOW_INTERRUPTS 0  //TODO: Do I need to desable interrupts?
//SD Card elements:
  #define SDCARD_CS_PIN    BUILTIN_SDCARD
  #define SDCARD_MOSI_PIN 11
  #define SDCARD_SCK_PIN 13

void setup() {
  //Bluetooth Serial setup
  Serial1.begin(9600);  //Defining communication rate with the HC-06 bluetooth module. Boud rate is 9600
  Serial.begin(9600);   //Serial for debugging
  
  //Audio setup:
  AudioMemory(8);

  //SD Card setup:
  SPI.setMOSI(SDCARD_MOSI_PIN);
  SPI.setSCK(SDCARD_SCK_PIN);

  if (!SD.begin(SDCARD_CS_PIN)) {
      Serial1.println("Unable to access the SD card");
      delay(500);
  }
  
  //FastLED Setup:
  //Candles Strip setup:
  FastLED.addLeds<WS2812, LED1_PIN, RGB> (Strip1, NUM_LED1);
  //MiniPumpkins Strip setup:
  FastLED.addLeds<WS2812, LED2_PIN, RGB> (Strip2, NUM_LED2);
  FastLED.setBrightness(50);

  //Volume Setup:
    amp1.gain(vol);                      //vol is a float. vol=0 is not sound. 0<vol<1.0 atenuation. 1.0<vol<32767.0
    Serial1.println(vol);

  delay(500);     //TODO: Do I need this delay? why?
}

void loop() {
  getSerialData();                      //Function to read the serial data coming from the app via bluetooth
  
  //Select track
  switch(serialData[0]){
    //Track: Candles. This is the default track since every time a track finished the serialData array is reset to (0,0,0,0)
    case 0:
      Candles();
      break;
    
    //Track: Boo!
    case 1:                             //When serialData[0] is 1, then the track is triggered
      Play(serialData[0]);
      Serial1.println("Available");     //Teensy sends "Available" back to the app so it enables all bottons
      break;
  
    //Track: Scream
    case 2:
      Play(serialData[0]);
      Serial1.println("Available");
      break;
        
    //Track: EvilLaugh
    case 3:
      Play(serialData[0]);
      Serial1.println("Available");
      break;

    //Track: Witch
    case 4:
      Play(serialData[0]);
      Serial1.println("Available");
      break;
  
    //Track: Gotcha
    case 5:
      Play(serialData[0]);
      Serial1.println("Available");
      break;
  
    //Track: AGoodLook
    case 6:
      Play(serialData[0]);
      Serial1.println("Available");
      break;
  
    //Track: Thriler01
    case 7:
      Play(serialData[0]);
      Serial1.println("Available");
      break;
  
    //Track MountainKing
    case 8:
      Play(serialData[0]);
      Serial1.println("Available");
      break;
        
    //Track: HarryPotter
    case 9:
      Play(serialData[0]);
      Serial1.println("Available");
      break;
  
    //Track: ColorWheel
    case 99:
      while(true){                   //While loop so the ColorWheel keeps going until the button "back" is pressed
        getSerialData();            //Teensy keeps on reading the serial1 port during the while loop
        fill_solid(Strip1, NUM_LED1, CRGB(serialData[1],serialData[2],serialData[3]));    //Strip1 follows the wheel color
        fill_solid(Strip2, NUM_LED2, CRGB(serialData[1],serialData[2],serialData[3]));    //Strip2 follows the wheel color
        FastLED.show();                                                                   //FastLED shows the color on the strips
        if(serialData[0]==98){      //Button "Back" is pressed
          break;
        }
      }
      break;

    //Volumn +:
    case 101:
      Volume(serialData[0]);
      break;
      
    //Volumn -:
    case 100:
      Volume(serialData[0]);
      break;
  }
  cleanSerialData();              //serialData array is set to zero so it continues playing Candles. 
}