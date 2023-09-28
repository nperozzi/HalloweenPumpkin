#include "makeColor.h"
#include "helper.h"

//Variable Definitions and Initialization
float vol = 0.02;
CRGB Strip1[NUM_LED1];
CRGB Strip2[NUM_LED2];
uint8_t serialData[NUM_DATA_FIELDS] = {0, 0, 0, 0};
uint8_t playingData[NUM_DATA_FIELDS] = {0, 0, 0, 0};

// Audio Elements:
// GUItool: begin automatically generated code
AudioPlaySdWav           playSdWav1;
AudioAmplifier           amp1;
AudioOutputAnalog        dac1;
AudioConnection          patchCord1(playSdWav1, 0, amp1, 0);
AudioConnection          patchCord2(amp1, dac1);
// GUItool: end automatically generated code

int songs_count = 0;
char songs[MAX_NUM_SONGS][MAX_SONGS_TITLE_LENGTH];

char stateFlag = 'N';
bool fogIsOn = false;

//Create Objects for light sequences:
FlashSequence flash = FlashSequence();
MountainKingSequence mountainKing = MountainKingSequence();

PWMServo eyeBall = PWMServo();
PWMServo eyeLid = PWMServo();

void sendSongs()
{
  while(Serial1.available() == 0)
  {
  }
  if(Serial1.available() > 0)
  {
    stateFlag = Serial1.read();
  }
  if (stateFlag == 'S')
  {
    Serial.println("Songs transfer started");
    for(int i = 0; i < songs_count; i++)
    {
      Serial1.println(songs[i]);
    }
  }
}

void getSerialData()  //TEST FUNCTION
{
  int i = 0;
  int value[NUM_DATA_FIELDS] = {0,0,0,0};
  while(Serial1.available() > 0)
  {
    char ch = Serial1.read();
    //Serial.println(ch);
    if( ch == '+')
    {
      vol = constrain(vol + VOL_CHANGE, VOL_MIN, VOL_MAX);
      amp1.gain(vol);
      Serial1.println(vol);
    }
    else if(ch == '-')
    {
      vol = constrain(vol - VOL_CHANGE, VOL_MIN, VOL_MAX);
      amp1.gain(vol);
      Serial1.println(vol);
    }
    else if(ch == '*')
    {
      fogIsOn = !fogIsOn;
      digitalWrite(FOG_PIN, fogIsOn);
      Serial.println(fogIsOn);
    }
    else if(ch >= '0' && ch <= '9')             //Add up the chars camming into the serial to build each element of the array
    {
      value[i] = (value[i] * 10) + (ch - '0');
    }
    else if(ch == ',' && i < NUM_DATA_FIELDS)
    {
        //Serial.println(serialData[i]);
        i++;
    }
    else if (ch == 'F')
    {
      for (int j = 0; j < NUM_DATA_FIELDS; j++)
      {
        serialData[j] = value[j];
        Serial.print(serialData[j]); //SERIAL DEBUG
        Serial.print(',');
        value[j] = 0;
      }
      Serial.println(); //SERIAL DEBUG
    }
  }
}

/* getSerialData()   //BACK UP FUNCTION 
void getSerialData()   //BACK UP FUNCTION
{
  int i = 0;
  int value[NUM_DATA_FIELDS] = {0,0,0,0};
  while(Serial1.available() > 0)
  {
    char ch = Serial1.read();
    //Serial.println(ch);
    if( ch == '+')
    {
      vol = constrain(vol + VOL_CHANGE, VOL_MIN, VOL_MAX);
      amp1.gain(vol);
      Serial1.println(vol);
    }
    else if(ch == '-')
    {
      vol = constrain(vol - VOL_CHANGE, VOL_MIN, VOL_MAX);
      amp1.gain(vol);
      Serial1.println(vol);
    }
    else if(ch == '*')
    {
      fogIsOn = !fogIsOn;
      digitalWrite(FOG_PIN, fogIsOn);
      Serial.println(fogIsOn);
    }
    else if(ch >= '0' && ch <= '9')             //Add up the chars camming into the serial to build each element of the array
    {
      value[i] = (value[i] * 10) + (ch - '0');
    }
    else if(ch == ',' && i < NUM_DATA_FIELDS)
    {
        //Serial.println(serialData[i]);
        i++;
    }
    else if (ch == 'F')
    {
      for (int j = 0; j < NUM_DATA_FIELDS; j++)
      {
        serialData[j] = value[j];
        Serial.print(serialData[j]); //SERIAL DEBUG
        Serial.print(',');
        value[j] = 0;
      }
      Serial.println(); //SERIAL DEBUG
    }
  }
}
*/

void cleanPlayingData()
{
  for(int i = 0; i <= 2; i++)
  {
    playingData[i] = 0;

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

void move()
{
  eyeBall.write(serialData[2]);
  eyeLid.write(serialData[3]);
}