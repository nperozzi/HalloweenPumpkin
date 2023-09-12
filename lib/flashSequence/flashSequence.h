/*FlashSequence
Description: Contains the class to generate flashing light sequences.
 
*/
#ifndef FLASHSEQUENCE_H
#define FLASHSEQUENCE_H

#include "Arduino.h"

class FlashSequence{
  public:
    FlashSequence(){}; //Constructor
    void runFlashSequence(CRGB color, uint32_t eventFrequency);  //Member function prototype
    
  private:
    bool isOn= false; //switching this variable on and off makes the flashing
    uint32_t lastEvent=0; //timestamp for the previous event  
};

#endif