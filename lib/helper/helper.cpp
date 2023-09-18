#include "makeColor.h"
#include "helper.h"

//Variable Definitions and Initialization
float vol = 0.02;
CRGB Strip1[NUM_LED1];
CRGB Strip2[NUM_LED2];
int fieldIndex = 0;
uint8_t values[NUM_FIELDS];
uint8_t serialData[NUM_FIELDS];

// Audio Elements:
// GUItool: begin automatically generated code
AudioPlaySdWav           playSdWav1;
AudioAmplifier           amp1;
AudioOutputAnalog        dac1;
AudioConnection          patchCord1(playSdWav1, 0, amp1, 0);
AudioConnection          patchCord2(amp1, dac1);
// GUItool: end automatically generated code

//Create Objects for light sequences:
FlashSequence flash= FlashSequence();
MountainKingSequence mountainKing= MountainKingSequence();

#define num_tracks 9
char PlayList[num_tracks][20] = {"Boo.wav",
                                "Scream.wav",
                                "EvilLaugh.wav",
                                "Witch.wav",
                                "Gotcha.wav",
                                "AGoodLook.wav",
                                "Thriler01.wav",
                                "MountainKing.wav",
                                "HarryPotter.wav"};

void getSerialData()  //TODO: Why the serial data is a array of 4?
{
  while(Serial1.available())
  {
    char ch = Serial1.read();
    if(ch >= '0' && ch <= '9')             //Add up the chars camming into the serial to build each element of the array
    {
      values[fieldIndex] = (values[fieldIndex] * 10) + (ch - '0');
    }
    else if(ch == ',')                  //If there is a "," it means that one emenet has ended and it is time to sart adding up the next elment
    {
      if(fieldIndex < NUM_FIELDS - 1){
        fieldIndex++;
      }
    }
    else if(ch == 'F')                   //If there is an "F", that means that one array has ended and it is time to start reading the next one.
    {
      for(int i = 0; i <= NUM_FIELDS - 1; i++)
      {
        serialData[i] = values[i];
        values[i] = 0;
      }
      fieldIndex = 0;
    }
  }
}

//Function to make the serialData array (0,0,0,0)
void cleanSerialData()
{
  for(int i = 0; i <= NUM_FIELDS - 1; i++)
  {
    serialData[i] = 0; 
  }
}

void Play(int track, CRGB::HTMLColorCode color)
{
  if(playSdWav1.isPlaying() != true)
  {
    Serial1.println("PlayingBoo");  //indicates the app that something is playing
    playSdWav1.play(PlayList[track - 1]);
    delay(20);
    while(playSdWav1.isPlaying() == true)
    {
      flash.runFlashSequence(color, 60);    //60 is the frequency of the fliquering, which seems right for the song.
      fill_solid(Strip2, NUM_LED2, CRGB::Black); //Turn off Strip2
    }
    Serial1.println("Available");  //signals the app that nothing is playing
  }
}

void Play(int track, uint8_t light)
{
  if(playSdWav1.isPlaying() != true)
  {
    Serial1.println("PlayingTrack");
    playSdWav1.play(PlayList[track - 1]);
    delay(20);
    while(playSdWav1.isPlaying() == true)
    {
      switch(light)
      {
        case 0:
          FlashMultiColor();
          break;
        case 1:
          mountainKing.runMountainKingSequence(500);
          //MountainKing boo flash
          while(playSdWav1.positionMillis() >= 6858)
          {
            flash.runFlashSequence(CRGB::White, 60);
            fill_solid(Strip2, NUM_LED2, CRGB::White);
          }
          break;
        case 2:
          mountainKing.runMountainKingSequence(1000);
          break;
      }
    }
    Serial1.println("Available");
  }
}

void Candles(){
  if(playSdWav1.isPlaying() == false)
  {
    for(int i = 0; i < NUM_LED1; i++)
    {
      float f = sin(millis() * 0.002f + (i * 0.628));
      int c = max(0, (int)(f * f * f * 50));
      Strip1[i]= CRGB(makeColor(10, 100, c));
      FastLED.show(); 
    }
    for(int i = 0; i < NUM_LED2; i++)
    {
      float f = sin(millis() * 0.002f + (i * 0.628));
      int c = max(0, (int)(f * f * f * 50));
      Strip2[i]= CRGB(makeColor(10, 100, c));
      FastLED.show(); 
    }
  }
}

void FlashMultiColor()
{
  #define RED    0xFF0000
  #define GREEN  0x00FF00
  #define BLUE   0x0000FF
  int numColors = 3;
  int colors[] = {RED, BLUE, GREEN};
  int tempo = 250;
  while(playSdWav1.isPlaying() == true)
  {
    for(int j = 0; j<numColors;j++)
    {
      for(int i = 0; i < NUM_LED1; i++)
      {
        Strip1[i] = colors[j];
        FastLED.show(); 
      }
      for (int i = 0; i < NUM_LED2; i++) {
        Strip2[i] = colors[j];
        FastLED.show(); 
      }
      delay(tempo);
    }
  }
}

void Volume(int value)
{
  switch (value)  //TODO: Maybe this can be an if instead of a switch?
  {
    case 101:
      vol = vol + 0.02; //TODO: No hardcoded values
      if(vol >= 0.6)   //TODO: Can I use a "constrain" function here?
      {
        vol=0.6;
      }
      amp1.gain(vol);
      Serial1.println(vol);
      Serial.println(vol);  //DEBUGGING LINE
      break;
      
    case 100:
      vol=vol-0.02;
      if(vol>=0.6)           //TODO: Test if you can push the limit higher
      {
        vol=0.6;
      }
      amp1.gain(vol);
      Serial1.println(vol);
      Serial.println(vol); //DEBUGGING LINE
      break;
  }
}

void MountainKingSequence::runMountainKingSequence(int eventFrequency)
{
  fill_solid(Strip1, NUM_LED1, CRGB::Black);
  //MiniPumpkins Dance
  EVERY_N_MILLISECONDS(eventFrequency)
  {
    //Create a new pixel for led[0]:
    Strip2[0]=CHSV(random8(), random8(), random8(100, 255));
  
    //Copy each pixel to the next one
    for( int i=3-1; i>0; i--)
    {
      Strip2[i]= Strip2[i-1];
    }
    FastLED.show();
  }
}

void FlashSequence::runFlashSequence(CRGB color, uint32_t eventFrequency)
{
  uint32_t timeStamp = millis();
  if((timeStamp - lastEvent) >= eventFrequency)
  {
    fill_solid(Strip1, NUM_LED1, isOn? CRGB::Black : color);
    isOn =! isOn;
    lastEvent = millis();
  }
  FastLED.show();  
}

