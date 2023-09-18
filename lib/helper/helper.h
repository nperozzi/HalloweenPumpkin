#ifndef CUSTOMMETHODS_H
#define CUSTOMMETHODS_H

#include <Audio.h>
#include <SerialFlash.h>
#include <SPI.h>
#include <SD.h>
#include <FastLED.h>

//Variable Declarations
extern float vol;

//FastLED elements:
  //Strip1 elements: Candles Strip
    #define LED1_PIN 2
    #define NUM_LED1 20
    extern CRGB Strip1[NUM_LED1];

  //Strip2 elements: Mini Pumpkings Strip
    #define LED2_PIN 3
    #define NUM_LED2 3
    extern CRGB Strip2[NUM_LED2];  //Mini Pumpkings Strip

// Audio Elements:
// GUItool: begin automatically generated code
extern AudioPlaySdWav           playSdWav1;
extern AudioAmplifier           amp1;
extern AudioOutputAnalog        dac1;
extern AudioConnection          patchCord1;
extern AudioConnection          patchCord2;
// GUItool: end automatically generated code

//Bluetooth Elements:
const int NUM_FIELDS = 4;
extern int fieldIndex;
extern uint8_t values[NUM_FIELDS];
extern uint8_t serialData[NUM_FIELDS];
//uint8_t storedSerialData[NUM_FIELDS];
//bool flag=true;  [REMOVE]

//Classes:
class MountainKingSequence{
  public:
    MountainKingSequence(){}
    void runMountainKingSequence(int eventFrequency);   //Member function prototype
};

class FlashSequence{
  public:
    FlashSequence(){}; //Constructor
    void runFlashSequence(CRGB color, uint32_t eventFrequency);  //Member function prototype
    
  private:
    bool isOn= false; //switching this variable on and off makes the flashing
    uint32_t lastEvent=0; //timestamp for the previous event  
};


void getSerialData();

void cleanSerialData();

void Play(int track, CRGB::HTMLColorCode color);

void Play(int value, uint8_t light);

void Candles();

void FlashMultiColor();

void Volume(int value);

#endif