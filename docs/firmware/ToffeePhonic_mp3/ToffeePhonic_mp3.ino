/*
   Code for MP3 version of ToffeePhonic
   tested on feather M0 only
   UNFINISHED!
   -needs a full rewrite of midiUpdate() downwards.
   -needs to read MP3s from folders
*/

#include <Wire.h>
#include <VL53L1X.h>
#include <U8g2lib.h>        //https://github.com/olikraus/u8g2/blob/master/cppsrc/U8g2lib.h - manages I2C display
#include <ScaleManager.h>   //https://github.com/ChrisBall/ScaleManager      
#include <MIDIUSB.h>

// include SPI, MP3 and SD libraries
#include <SPI.h>
#include <SD.h>
#include <Adafruit_VS1053.h>

#define MIDIMODE true  //whether to send out MIDI (true) or debug (false) over USB HID Midi device
#define HAIRLESS false  //http://projectgus.github.io/hairless-midiserial/ if MIDIMODE==true, use to send MIDI over USB serial port (at 115200 baud)

#define UP 0
#define LEFT 1
#define CENTRE 2
#define RIGHT 3
#define DOWN 4
#define NUM_BUTTONS 5

// These are the pins used
#define VS1053_RESET   -1     // VS1053 reset pin (not used!)
#define VS1053_CS       6     // VS1053 chip select pin (output)
#define VS1053_DCS     10     // VS1053 Data/command select pin (output)
#define CARDCS          5     // Card chip select pin
// DREQ should be an Int pin *if possible* (not possible on 32u4)
#define VS1053_DREQ     9     // VS1053 Data request, ideally an Interrupt pin

long unsigned int range = 300;
long unsigned int startTime = 0;

int minRange = 50;
int maxRange = 750;

int minRangeLimit = 50; //minimum distance we can sense
int maxRangeLimit = 1000; //maximum distance we can sense

boolean updateDisplayFlag = true;

byte scaleRange = 8;
byte currentMidiNote = 128; //128 is 'inactive'
byte activeMidiNote = 128; //'playing' midi note (used when notes change to turn off the old note);
byte outputScaleNote = 0;  //range scaled to note
boolean notePlaying = false;

int activeMenuInput = 0;
byte menuItems = 6;
boolean bigMenu = false;

char tabChar='\t';

byte instrument = 0;
byte volume = 127;

char filename[16];  //16 char filename (inc. folder)

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);   //page buffer = 128bytes (_1_ = 128, _2_ = 256, _F_ = 1024)
ScaleManager sm(true);

VL53L1X TOFSensor;

Adafruit_VS1053_FilePlayer musicPlayer = Adafruit_VS1053_FilePlayer(VS1053_RESET, VS1053_CS, VS1053_DCS, VS1053_DREQ, CARDCS);

const byte buttonPins[] = {10, 11, 9, 5, 6}; //UP, LEFT, CENTRE, RIGHT, DOWN
byte buttonStates[NUM_BUTTONS];

void setup() {
  delay(2000);
  Serial.begin(115200);
  setupSM(); //setup scale manager
  setupTOF();
  u8g2.begin();
  setupPins();
  VS1053setup();
  drawMenu();
}


void loop() {
  buttonUpdate();  //updates buttons states
  buttonActions(); //carries out actions from button states
  rangeUpdate();   //checks for data from the range sensor and updates the value in range and outputScaleNote
  midiUpdate();    //sends midi messages as appropriate

  if (updateDisplayFlag == true) {
    drawMenu();            //update the menu
    updateDisplayFlag = false;
  }

  if (!MIDIMODE) {
    printDebug();    //prints debug messages
  }
}

void printDebug() {
  for (int i = 0; i < 5; i++) {
    Serial.print(buttonStates[i]);
  }
  Serial.print(tabChar);
  Serial.print(activeMenuInput);
  Serial.print(tabChar);
  Serial.print(range);
  Serial.print(tabChar);
  Serial.print(outputScaleNote);
  Serial.print(tabChar);
  Serial.print(minRange);
  Serial.print(tabChar);
  Serial.print(maxRange);

  Serial.println();
}
