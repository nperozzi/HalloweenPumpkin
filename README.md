# Halloween Pumpkins
## Video Demo:  <URL HERE>
## Description:
My final CS50 project is a set of four animatronic Halloween pumpkins that feature sound and light effects and an animatronic eye to make it even more spooky. The electronics are embedded into real carved pumpkins to make them realistic. The whole device is controlled with a custom Android application that communicates via Bluetooth with the electronics hosted in the main pumpkin.

Every year, for Halloween, my neighborhood hosts a  jack o'lantern exhibition. The kids living in the area and their parents carve pumpkins and display them at the neighborhood square. I participate in this event by taking my pumpkins to another level and making them their own animatronics show. It brings me tremendous joy to see how many people enjoy seeing the pumpkins sing and light up.

The pumpkin firmware is developed in C using the Arduino framework. The development environment is  VS Code with the [PlartformIO](https://platformio.org/) extension. The Android app was developed using a Skratch-like programming language developed by MIT called [MIT App Inventor](https://appinventor.mit.edu/). The hardware is built around a [Teensy 3.5 development board](https://www.pjrc.com/store/teensy35.html) which features an ARM Cortex-M4 at 120 MHz. All the electronics are mounted on a carefully routed protoboard.

<p align="center">
<img  src="images\main_picture.png"  width="80%" height="80%"><br>
<sub>Halloween pumpkings ready for the show</sub>
</p>

<p align="center">
<img  src="images\Grl_Electronics.png"  width="80%" height="80%"><br>
<sub>The electronics in the pumpkins.</sub>
</p>

### Features:
The sections bellow present a list of this project´s features.

#### Audio Effects
* Audio files are stored in an SD card. The audio files consist of several Halloween-related songs and spooky sound effects. The files are 16-bit PCM (Pulse Code Modulation) at 44100 Hz .wav files. 

#### Light Effects
* Two LED strips: one for the main pumpkin and another one for the three smaller pumpkins
* When not playing an audio file, the LED strips display simulates candles lit inside the pumpkins.
* Light effect 1: When playing an audio file, the LED strips can display the LED brightness synced to the audio file that is playing. This effect is displayed in different color versions: white, red, green, and purple.
* Light effect 2: When playing an audio file, the LED strips flash on and off with three different colors at a rate of 60 ms. This effect is displayed in two versions: red-green-blue and purple-green-orange.
* Light effect 3: When playing an audio file, the LED strips display a rainbow color effect that cycles through the color wheel in 5 seconds.
* Light effect 4: This is a specified light effect designed for the audio file MountainKing.

#### Animatronic Eye
* The animatronic eye features two servo motors, one for the eyelid and one for the eyeball.
* The code for both motors allows for an organic movement where the servos reduce their speed as they approach the new position.

#### Communication
* The communication between the pumpkin and the Android app happens via Bluetooth. During standard operation, the app transmits 6 values in a string, the Bluetooth module on the pumpkin receives it and passes the string to the microcontroller via serial communication.
* When a Bluetooth connection is established, the pumpkin transmits the name of the audio files stored in the SD card to the app which stores it in a list. That way the audio files can be changed without modifying the code.
* When the volume is changed on the app, the pumpkin sends the volume value to confirm to the user the actual volume.

### Pumpkin firmware description
The firmware on the pumpkin starts with a setup function that does all the necessary initial setups:

* Serial Port
* LED Strips
* SD card SPI communication
* Saving and communicating the Songs
* Initial volume
* Eye servos

On the loop, the code does the following:

1. Check for serial data. If it was not received successfully, play the candles. 1. If it was received successfully continue with the loop.
1. If the serial data has data for the eyelid, implement the eyelid function
1. If the serial data has data for the eyeball and the eyelid is open, implement the eyeball function.
1. If the serial data indicates that the volume needs to go up, implement the volume function
1. If the serial data indicates that the volume needs to go down, implement the volume function.
1. If the serial data indicates that a new audio file needs to be played, check if there is something playing. If not, store the accompanying light value and start playing the audio effect.
1. If there is an audio effect playing, play the saved light value for the light function, otherwise, play the candle effect.

Below I describe some interesting functions in the pumpkin firmware:

#### The `void lights(int value)` function.
The lights function evaluates if an audio file is playing and selects the right light effect from a switch-case function. If nothing is playing, it plays the candle effect.

#### The `bool smothEyeLid(SerialData &data, float moveRate)` and void `smoothEyeBall(SerialData &data, float moveRate)` functions
I wanted the eye to loop as realistic as possible. Servo motors react too quickly which gives a “robotic” sensation. A more organic movement of a real muscle accelerates while initiating the movement and decelerates when reaching the target position. These two functions provide organic movement by combining a fraction of the old position with a fraction of the new position.

#### The `void soundSyncLight()` function
This is the function that allows for the light's brightness to sync up with the audio effects. Every 24 milliseconds the function reads the height of the audio signal. That value is mapped to the brightness of the LED srips providing the syncing effect.

### Android App description
Starts with a single button that prompts you to establish a Bluetooth connection to the pumpkin. Once the connection is established, it reveals several buttons:
* 12 favorite buttons: These buttons allow for quick access to very popular and handy sound and light effects combinations. For example, the sound effect Boo! and the flashing white light which is very handy to give a good scare. Another example is a snippet of the song “U can´t touch this!” by MC Hummer and a flashing red light which is very handy to indicate to kits that “U can´t touch” the pumpkin. 
* Buttons to raise and lower the volume. Useful since the pumpkin is for outdoors where sound dissipates very easily.
* Button to open and close the pumpkin eyelid
* Slider to move the eyeball from left to right
* Custom audio and light effect combinations: The buttons “Picker Song” and “Picker Lights” give you access to the whole list of audio files stored in the SD card and all the pre-set light effects. The play button gets the magic going.

The program starts by waiting for the user to press the “Connection” button and select a Bluetooth signal. Once the connection is established, the app sends a flag char `S` to the pumpkin, indicating that the connection has been established and then it waits for the pumpkin to send the list of audio files in the SD card. That list is stored in a list called `songsList`.

Then the app starts communicating a 6 values message to the pumpkin every 500ms. This is done with the `SerialData_Clock` Timer. This message structure is as follows:
> [song, light, eyeball, eyelid, volUp, volDown]
>
> where:
> 
> * song: is the position of a specific audio effect in the songsList. If no song, then 0.
> * light: is the position of a specific light effect in the lightsList. If no light, then 0.
> * eyeball: is the position of the servo for the eyeball.
> * eyelid: is the position of the servo for the eyelid.
> * volUp: 1 indicates that the volume should go one position up, 0 indicates no change.
> * volDown:  1 indicates that the volume should go one position down, 0 indicates no change.

The values in the message are controlled by the press of the buttons in the app.

<p align="center">
<img  src="images\app_connection.png"  width="50%" height="50%"><br>
<sub>App capture before connecting to the pumpkin.</sub>
</p>

<p align="center">
<img  src="images\app_control.png"  width="50%" height="50%"><br>
<sub>App capture after connecting to the pumpkin.</sub>
</p>


#### Audio file storage
In order to have many audio files to play, I wanted the app to get the file names stored in the SD card so they could be changed easily without having to modify the code. To do that, I decided to include a sequence of steps when both the pumpkin´s firmware and the app are initializing:

1. When the pumpkin is going through setup, it reads the file names in the SD card and stores them in an array. Then it waits to receive a signal from the app.
1. A Bluetooth connection is established and the app sends the signal to the pumpkin letting it know that it is ready to receive the file names.
1. The pumpkin sends the file names to the app and moves on to the loop where it starts listening for serial communication.
1. The app stores the audio file names on a list where they are available for use.

During standard operation, the audio file names are used for the custom song picker and the favorite buttons.

##### Serial communication through Bluetooth
Even though the communication over the air happens via Bluetooth, the Bluetooth module in the pumpkin communicates with the microcontroller through serial communication. One of the most challenging features of the project was handling serial communication.

To control the pumpkin, the app sends a message with 6 values to the pumpkin every 500ms. The message is structured in the following way: song number, light effect number, eyeball position, eyelid position, volume up, volume down.

Since the serial buffer of the microcontroller has 64 bytes, the size of the buffer and the message need to be predefined, otherwise, data is misplaced when the buffer reaches its limit. To do this, the microcontroller reads the chars from the serial port and stores them in an array 16-byte array, which is the exact size of the message. Once the full message is received, the data in the array is scanned using the sscanf() function and organized into a struct.

### Files Description
In this section I describe the content of each of the files uploaded for this project.

| Files | Descriptions |
|---:|:---|
|app/HalloweenPumpkinV4.aia| This is the code for the Android app. It can be opened in the MIT App Inventor [MIT App Inventor](https://appinventor.mit.edu/) web app.|
|build/|This folder is generated automatically when working with [PlartformIO](https://platformio.org/) and it stores the outputs of the compilation and build process.|
| lib/ | This folder stores the custom libraries that I have built or imported. |
| lib/helper/helper.cpp | This is the source file of my custom library. It contains all the formulas I have developed for this project. |
| lib/helper/helper.h | This is the header file of my helper library. |
| lib/makeColor/makeColor.cpp | This library provides the algorithm for the candles effect. |
| lib/makeColor/makeColor.h | This is the header file for the makeColor library |
| src/main.cpp | This is the main file which contains global variables, the setup function, and the loop function. |
| platformio.ini | This is the PlatformIO file that contains all the necessary configurations for my microcontroller. In this case, mounted on a [Teensy 3.5 development board](https://www.pjrc.com/store/teensy35.html). | 

## Bill of Materials

| Part Name and links | Qty |
|---:|:---:|
| [Teensy 3.5] (https://www.pjrc.com/store/teensy35.html) | 1 |
| XL6009E1 Bost Converter module | 1 |
| 3 Inch Speaker, 8 Ohm, 20W | 1 |
| TDA8932 35W Digital Amplifier | 1 |
| Bluetooth module HC-06 | 1 |
| Ws2811 addressable LED strip | 20 LEDs |
| Ws2811 addressable LED strip | 3 LEDs |
| Hobby servo motors | 2 |
| AMS1117 5.0V  | 1 |
| Battery holder | 3 |
| 18560 li-ion batteries | 6 |
| Prototyping board | 1 |
| Male and female DUPONT header pins | many |
| TJC3 connectors | several |
| 22 AWG cable | much |
| 220-ohm resistors | 2 |
| 100uF capacitor | 1 |
| Real pumpkins | 3 |

## Programs used
| Program | Purpose |
|---:|:---|
| VS Code | This was the code editor that I used to develop the Teensy code. |
| PlatformIO | This was the embedded development environment that I used as an add-on to VS Code. It facilitates developing and deploying embedded code for MCUs. |
| Git and GitHub | I used Git for version control and GitHub to store my project. |
| MIT App Inventor | This was the web application that I used to develop the Android app. It provides an environment to develop mobile applications using a Skratch-like block language. |
| Fritzing | Fritzing is an open-source hobby CAD software for electronics development. I used this program to develop the layout of my protoboard. |
| Audacity | Audacity is an open-source digital audio editor. I used it to edit the audio file with the sound effects. |
| Fusion 360 | 3D CAD modeling software. I used it to modify the eye model for 3D printing and develop other 3D printed support mechanical pieces. |

## Schematics
The image below illustrates the wiring block diagram that was used to solder the protoboard.

<p align="center">
<img  src="images\Block_Diagram.png"  width="100%" height="100%"><br>
<sub>Protoboard block diagram</sub>
</p>

<p align="center">
<img  src="images\Protoboard_top.png"  width="80%" height="80%"><br>
<sub>Protoboard picture from the top</sub>
</p>

<p align="center">
<img  src="images\Protoboard_bottom.png"  width="80%" height="80%"><br>
<sub>Protoboard picture from the bottom</sub>
</p>

## Calibration
### Eyeball and eyelid calibration

The eyeball and eyelid are controlled by servos and those servos are restricted by the movement range of the 3D printed parts for the eyeball and eyelid. Those values can be calibrated with the variables `ballLeft`, `ballRight`, `lidOpen`, and `lidClosed`.

### Volume calibration
Depending on where will you be using the pumpkin, the max and min volume and volume steps can be calibrated with the definitions: `#define VOL_CHANGE`, 
`#define VOL_MIN`, `#define VOL_MAX`.

## Potential improvements
* The LED strips do not have their ground connected to the microcontroller's ground. The LED strips do work because the ground connection is happening through the ground of the audio amplifier. This is not a good design but when trying to connect the LED strips ground to the MCU ground, a lot of noise was generated on the audio signal.

## Acknowledgments and Credits
| Acknowledgement | Content |
|:---|:---|
| Erasmus C | Erasmus helped me optimize the serial communication function. |
| Paul Stoffregen | For several of the libraries used in this project plus the development of the Teensy 3.5 |
| James Bruton | For the development of the animatronic eye design which I modified to fit my application. |
