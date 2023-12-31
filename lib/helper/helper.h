#ifndef CUSTOMMETHODS_H
#define CUSTOMMETHODS_H

#include <Audio.h>
#include <SerialFlash.h>
#include <SPI.h>
#include <SD.h>
#include <FastLED.h>
#include <PWMServo.h>

//Variables declaration for savingSongs
extern char stateFlag;

//Volume variables declarations
extern float vol;
#define VOL_CHANGE 0.06
#define VOL_MIN 0.0
#define VOL_MAX 0.6

//Songs variables declarations
#define MAX_NUM_SONGS 255
#define MAX_SONGS_TITLE_LENGTH 30
extern int songs_count;
extern char songs [MAX_NUM_SONGS][MAX_SONGS_TITLE_LENGTH];

//FastLED elements:
//Strip1 elements: Candles Strip
#define LED1_PIN 2
#define NUM_LED1 20
extern CRGB Strip1[NUM_LED1];

//Strip2 elements: Mini Pumpkings Strip
#define LED2_PIN 3
#define NUM_LED2 2
extern CRGB Strip2[NUM_LED2];

//Eye variables declarations:
#define EYEBALL_PIN 29
#define EYELID_PIN 30
extern PWMServo eyeBall;
extern PWMServo eyeLid;
extern int ballLeft;
extern int ballRight;
extern int lidOpen;
extern int lidClosed;


// Audio Elements:
// GUItool: begin automatically generated code
extern AudioPlaySdWav           playSdWav1;
extern AudioAmplifier           amp1;
extern AudioOutputAnalog        dac1;
extern AudioConnection          patchCord1;
extern AudioConnection          patchCord2;
extern AudioAnalyzePeak         peak;
extern AudioConnection          patchCord3;
// GUItool: end automatically generated code

//Bluetooth Elements:
const int NUM_DATA_FIELDS = 6;
extern  uint8_t serialData[NUM_DATA_FIELDS];
//extern uint8_t playingData[NUM_DATA_FIELDS];
struct SerialData 
{
  int song;
  int light;
  int eyeball;
  int eyelid;
  int vol_up;
  int vol_down;
};

void saveSongs();
void sendSongs();
bool readSerialData(SerialData &data);
void lights(int value);
void volume(SerialData &data);
bool smothEyeLid(SerialData &data, float moveRate);
void smoothEyeBall(SerialData &data, float moveRate);
void soundSyncLight();
void Candles();
void flashThreeColors(CRGB color0, CRGB color1, CRGB color2, uint16_t frequency);
void rainbow(uint16_t rainbowCycleTime);
void mountainKing(int frequency);

#endif