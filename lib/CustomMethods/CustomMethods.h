#ifndef CUSTOMMETHODS_H
#define CUSTOMMETHODS_H

#include "Arduino.h"
#include <Audio.h>
#include <SerialFlash.h>


// Audio Elements:
// GUItool: begin automatically generated code
AudioPlaySdWav           playSdWav1;
AudioAmplifier           amp1;
AudioOutputAnalog        dac1;
AudioConnection          patchCord1(playSdWav1, 0, amp1, 0);
AudioConnection          patchCord2(amp1, dac1);
// GUItool: end automatically generated code

//Bluetooth Elements:
const int NUM_FIELDS=4;
int fieldIndex=0;
uint8_t values[NUM_FIELDS];
uint8_t serialData[NUM_FIELDS];
uint8_t storedSerialData[NUM_FIELDS];
bool flag=true;


  char* PlayList[] = {"Boo.wav",
                      "Scream.wav",
                      "EvilLaugh.wav",
                      "Witch.wav",
                      "Gotcha.wav",
                      "AGoodLook.wav",
                     "Thriler01.wav",
                      "MountainKing.wav",
                      "HarryPotter.wav"};

//FastLED elements:
  //Strip1 elements: Candles Strip
    #define LED1_PIN 2
    #define NUM_LED1 20
    extern CRGB Strip1[NUM_LED1];  

  //Strip2 elements: Mini Pumpkings Strip
    #define LED2_PIN 3
    #define NUM_LED2 3
    extern CRGB Strip2[NUM_LED2];  //Mini Pumpkings Strip

//Volume elements:
float vol=0.1;

void getSerialData();

void cleanSerialData();

void Play(int value);

void Candles();

void FlashMultiColor();

void Volume(int value);

#endif