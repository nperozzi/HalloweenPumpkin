/*MountainKingSequence
Description: Contains the class to generate the MountainKing light sequence.

*/

#include "Arduino.h"

class MountainKingSequence{
  public:
    MountainKingSequence(){};          //Constructor
    void runMountainKingSequence(int eventFrequency);            //Member function prototype
    
  private:
};

void MountainKingSequence::runMountainKingSequence(int eventFrequency){
    fill_solid(Strip1, NUM_LED1, CRGB::Black);
    
    //MiniPumpkins Dance
    EVERY_N_MILLISECONDS(eventFrequency){
      //Create a new pixel for led[0]:
      Strip2[0]=CHSV(random8(), random8(), random8(100, 255));
    
      //Copy each pixel to the next one
      for( int i=3-1; i>0; i--){
        Strip2[i]= Strip2[i-1];
      }
      FastLED.show();
    }
} 