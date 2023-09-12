/*MountainKingSequence
Description: Contains the class to generate the MountainKing light sequence.

*/
#ifndef MOUNTAINKING_H
#define MOUNTAINKING_H

#include "Arduino.h"

class MountainKingSequence{
  public:
    MountainKingSequence(){}
    void runMountainKingSequence(int eventFrequency);            //Member function prototype
};

#endif