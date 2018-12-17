/*
 * Code for most basic version of ToffeePhonic
 * needs updating to current version of ScaleManager
 */
 
#include <Wire.h>
#include <VL53L1X.h>
#include <U8g2lib.h>        //https://github.com/olikraus/u8g2/blob/master/cppsrc/U8g2lib.h - manages I2C display
#include <ScaleManager.h>   //https://github.com/ChrisBall/ScaleManager      
#include <MIDIUSB.h>

#define VS1053_BANK_DEFAULT 0x00
#define VS1053_BANK_DRUMS1 0x78
#define VS1053_BANK_DRUMS2 0x7F
#define VS1053_BANK_MELODY 0x79

#define MIDI_NOTE_ON  0x90
#define MIDI_NOTE_OFF 0x80
#define MIDI_CHAN_MSG 0xB0
#define MIDI_CHAN_BANK 0x00
#define MIDI_CHAN_VOLUME 0x07
#define MIDI_CHAN_PROGRAM 0xC0

#define MIDIMODE true  //whether to send out MIDI (true) or debug (false) over USB HID Midi device
#define HAIRLESS false  //http://projectgus.github.io/hairless-midiserial/ if MIDIMODE==true, use to send MIDI over USB serial port (at 115200 baud)

#define UP 0
#define LEFT 1
#define CENTRE 2
#define RIGHT 3
#define DOWN 4
#define NUM_BUTTONS 5

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

byte midiInstrument = 0;
byte midiVolume = 127;
byte midiBank = 0;
byte midiBanks[] = {VS1053_BANK_DEFAULT, VS1053_BANK_DRUMS1};

U8G2_SSD1306_128X64_NONAME_2_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);   //page buffer = 128bytes (_1_ = 128, _2_ = 256, _F_ = 1024)
ScaleManager sm(true);

VL53L1X TOFSensor;

const byte buttonPins[] = {6,5,9,11,10}; //UP, LEFT, CENTRE, RIGHT, DOWN
byte buttonStates[NUM_BUTTONS];

void setup() {
  setupSM(); //setup scale manager
  setupTOF();
  u8g2.begin();
  setupPins();
  setupMidi();
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
  Serial.print('\t');
  Serial.print(activeMenuInput);
  Serial.print('\t');
  Serial.print(range);
  Serial.print('\t');
  Serial.print(outputScaleNote);
  Serial.print('\t');
  Serial.print(minRange);
  Serial.print('\t');
  Serial.print(maxRange);

  Serial.println();
}
