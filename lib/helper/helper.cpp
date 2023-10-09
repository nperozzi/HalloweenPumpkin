#include "makeColor.h"
#include "helper.h"

char stateFlag = 'N';
float vol = 0.02;

//Serial data initialization:
uint8_t serialData[NUM_DATA_FIELDS] = {0, 0, 0, 0, 0, 0};
uint8_t playingData[NUM_DATA_FIELDS] = {0, 0, 0, 0, 0, 0};

// Audio Elements:
// GUItool: begin automatically generated code
AudioPlaySdWav           playSdWav1;
AudioAmplifier           amp1;
AudioOutputAnalog        dac1;
AudioConnection          patchCord1(playSdWav1, 0, amp1, 0);
AudioConnection          patchCord2(amp1, dac1);
// GUItool: end automatically generated code

//Songs initialization
CRGB Strip1[NUM_LED1];
CRGB Strip2[NUM_LED2];
int songs_count = 0;
char songs[MAX_NUM_SONGS][MAX_SONGS_TITLE_LENGTH];

//Lights initialization:
FlashSequence flash = FlashSequence();
MountainKingSequence mountainKing = MountainKingSequence();

//Eye initialization
PWMServo eyeBall = PWMServo();
PWMServo eyeLid = PWMServo();
int ballLeft = 47;
int ballRight = 112;
int lidOpen = 110;
int lidClosed = 170;

void saveSongs()
{
  File root = SD.open("/");
  if (root)
  {
    while(File file = root.openNextFile())
    {
      strcpy(songs[songs_count], file.name());
      songs_count++;
      file.close();
    }
    root.close();
  }
}

void sendSongs()
{
  while(Serial1.available() == 0)
  {
  }
  if(Serial1.available() > 0)
  {
    stateFlag = Serial1.read();
  }
  if (stateFlag == 'S')   //TODO: The device should be checking every x amount of time if there is a connection established and if not, go back to a disconnected mode.
  {
    Serial.println("Songs transfer started");
    for(int i = 0; i < songs_count; i++)
    {
      Serial1.println(songs[i]);
    }
  }
}

bool readSerialData(SerialData &data) {
  static char serialBuffer[15]; //QUESTION: why buffer 15bytes?
  static unsigned int bufferIndex = 0;
  char endMarker = '\n';

  while (Serial1.available()) 
  {
    char receivedChar = Serial1.read();
    if (receivedChar != endMarker) 
    {
      if (bufferIndex < sizeof(serialBuffer) - 1) 
      {
        serialBuffer[bufferIndex++] = receivedChar;
      } else {
        // Buffer overflow, clear the buffer
        bufferIndex = 0;
      }
    } 
    else {
      // Null-terminate the buffer
      serialBuffer[bufferIndex] = '\0';  //QUESTION: why do we need to add the '\0' char to the buffer?

      int result = sscanf(serialBuffer, "%d,%d,%d,%d,%d,%d", &data.song, &data.light, &data.eyeball, &data.eyelid, &data.vol_up, &data.vol_down);
      bufferIndex = 0; // Reset buffer index

      if (result == 6) {
        Serial.print(data.song);Serial.print(", ");
        Serial.print(data.light);Serial.print(", ");
        Serial.print(data.eyeball);Serial.print(", ");
        Serial.print(data.eyelid);Serial.print(", ");
        Serial.print(data.vol_up);Serial.print(", ");
        Serial.println(data.vol_down);
        return true;
      } 
      else {
        return false;
      }
    }
  }
  return false;
}

void lights(int value)
{
  if(playSdWav1.isPlaying() == true)
  {
    switch(value)
    {
      case 1:
        flash.runFlashSequence(CRGB::White,60);
        fill_solid(Strip2, NUM_LED2, CRGB::Black);
        break;
      case 2:
        flash.runFlashSequence(CRGB::Red,60);
        fill_solid(Strip2, NUM_LED2, CRGB::Black);
        break;
      case 3:
        rainbow(5000);
        break;
      case 4:
        flash.runFlashSequence(CRGB::Purple,60);
        fill_solid(Strip2, NUM_LED2, CRGB::Black);
        break;
      case 5:
        flashThreeColors(CRGB::Red, CRGB::Green, CRGB::Blue, 250);
        break;
      case 6:
        flashThreeColors(CRGB::Orange, CRGB::Yellow, CRGB::Violet, 250);
        break;
      case 7:
        rainbow(5000);
        break;
      case 8:
        mountainKing.runMountainKingSequence(500);
        break;
    }
  }
  else
  {
    Candles();
  }
}

void volume(SerialData &data)
{
  if(data.vol_up == 1)
  {
    vol = constrain(vol + VOL_CHANGE, VOL_MIN, VOL_MAX);
    amp1.gain(vol);
    Serial1.println(vol);
  }
  else if(data.vol_down == 1)
  {
    vol = constrain(vol - VOL_CHANGE, VOL_MIN, VOL_MAX);
    amp1.gain(vol);
    Serial1.println(vol);
  }
}

bool smothEyeLid(SerialData &data, float moveRate)
{
  static float currentPosEyeLid = lidClosed;
  if (data.eyelid == 1)
  {
    currentPosEyeLid = (lidOpen * moveRate) + (currentPosEyeLid * (1.0 - moveRate));
    currentPosEyeLid = constrain(currentPosEyeLid, lidOpen, lidClosed);
    eyeLid.write(currentPosEyeLid);
    return true;
  }
  else 
  {
    currentPosEyeLid = (lidClosed * moveRate) + (currentPosEyeLid * (1.0 - moveRate));
    currentPosEyeLid = constrain(currentPosEyeLid, lidOpen, lidClosed);
    eyeLid.write( currentPosEyeLid);
    return false;
  }
}

void smoothEyeBall(SerialData &data, float moveRate)
{
  static float currentPosEyeBall = ballLeft + (ballRight - ballLeft) / 2;
  static float prevTargetPosEyeBall = currentPosEyeBall;
  static float currentPosEyeLid = lidClosed;

  if (abs(data.eyeball - currentPosEyeBall) >= 1)
  {
    currentPosEyeBall = (data.eyeball * moveRate) + (currentPosEyeBall * (1.0 - moveRate));
    currentPosEyeBall = constrain(currentPosEyeBall, ballLeft, ballRight);
    eyeBall.write( currentPosEyeBall);

    if (abs(currentPosEyeBall - prevTargetPosEyeBall) < abs(data.eyeball - prevTargetPosEyeBall) / 2)
    {
      currentPosEyeLid = (lidClosed * moveRate) + (currentPosEyeLid * (1.0 - moveRate));
      currentPosEyeLid = constrain(currentPosEyeLid, lidOpen, lidClosed);
      eyeLid.write( currentPosEyeLid);
    }
    else
    {
      currentPosEyeLid = (lidOpen * moveRate) + (currentPosEyeLid * (1.0 - moveRate));
      currentPosEyeLid = constrain(currentPosEyeLid, lidOpen, lidClosed);
      eyeLid.write(currentPosEyeLid);
    }
  }
  prevTargetPosEyeBall = data.eyeball;
}

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

void flashThreeColors(CRGB color0, CRGB color1, CRGB color2, uint16_t frequency)
{
  static uint32_t prevMillis = 0;
  static uint8_t colorIndex = 0;
  static bool isColor = false;
  unsigned long currentMillis = millis();
  
  if (currentMillis - prevMillis >= frequency)
  {
    prevMillis = currentMillis;

    if(isColor)
    {
      fill_solid(Strip1, NUM_LED1, CRGB::Black);
    }
    else
    {
      switch(colorIndex)
      {
        case 0:
          fill_solid(Strip1, NUM_LED1, color0);
          fill_solid(Strip2, NUM_LED2, color0);
          break;
        case 1:
          fill_solid(Strip1, NUM_LED1, color1);
          fill_solid(Strip2, NUM_LED2, color1);
          break;
        case 2:
          fill_solid(Strip1, NUM_LED1, color2);
          fill_solid(Strip2, NUM_LED2, color2);
          break;
      }
    }
    FastLED.show();
    isColor = !isColor;
    if(isColor == false)
    {
      colorIndex = (colorIndex + 1) % 3;
    }

  }
}

void rainbow(uint16_t rainbowCycleTime)
{
  static uint32_t prevMillis = 0;
  static uint8_t colorIndex = 0;
  unsigned long currentMillis = millis();
  if (currentMillis - prevMillis >= rainbowCycleTime / 256)
  {
    prevMillis = currentMillis;

    for (int i = 0; i < NUM_LED1; i++)
    {
      Strip1[i] = CHSV(colorIndex + (i * 256/NUM_LED1), 255, 255);
    }
    FastLED.show();
    colorIndex++;
    if(colorIndex > 255)
    {
      colorIndex = 0;
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

