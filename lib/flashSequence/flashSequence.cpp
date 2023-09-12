

#include <Arduino.h>
#include <FastLED.h>

#include "CustomMethods.h"
#include "flashSequence.h"

void FlashSequence::runFlashSequence(CRGB color, uint32_t eventFrequency){
  uint32_t timeStamp= millis();

  if((timeStamp - lastEvent)>= eventFrequency){
    fill_solid(Strip1, NUM_LED1, isOn? CRGB::Black : color);
    isOn=!isOn;
    lastEvent=millis();
  }
  FastLED.show();  
}