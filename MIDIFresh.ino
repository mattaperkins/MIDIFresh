// MIDIFresh
// Matt Perkins - Copyright (C) 2025
// A Tiny MIDI compatable DJ deck for use with Traktor (should work with others)
// Has a cross fader and 3 knobs - Uses a Adafruit Feather ESP32-S3 reverse


/*

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

*/




#include <Arduino.h>
#include <BLEMidi.h>          // BLE Midi Library
#include <Adafruit_GFX.h>     // Core graphics library
#include <Adafruit_ST7789.h>  // Hardware-specific library for ST7789
#include <SPI.h>
#include <Wire.h>              // One Wire lib 
#include "Adafruit_MAX1704X.h" // Battery meter lib. 


// Init Screen
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

// Init Battery Monitor 
Adafruit_MAX17048 maxlipo;
bool addr0x36 = true;


// Misc Fonts
#include "Fonts/FreeSans9pt7b.h"
#include "Fonts/FreeSans12pt7b.h"
#include "Fonts/FreeSans18pt7b.h"
#include "Fonts/FreeMono12pt7b.h"

char *mainver = "0.03";  // version

// Midi Channel
const uint8_t midi_channel = 0;

// Crossfader
int crossfader_pin = A3;    // crossfader anaglog pin
int crossfader_min = 3;    // Minimum value for crossfader
int crossfader_max = 4095;  // maximum value for crossfader
int crossfader_midi = 10;   // MIDI Control # for crossfader

// Knob1
int knob1_pin = A0;    // knob1 anaglog pin
int knob1_min = 1;    // Minimum value for knob1
int knob1_max = 4095;  // maximum value for knob1
int knob1_midi = 11;   // MIDI Control # for knob1

// Knob2
int knob2_pin = A1;    // knob2 anaglog pin
int knob2_min = 3;    // Minimum value for knob2
int knob2_max = 4095;  // maximum value for knob2
int knob2_midi = 12;   // MIDI Control # for knob2

// Knob3
int knob3_pin = A2;    // knob3 anaglog pin
int knob3_min = 3;    // Minimum value for knob3
int knob3_max = 4095;  // maximum value for knob3
int knob3_midi = 13;   // MIDI Control # for knob3

int sleep_timeout = 360; // Bluetooth timeout before unit goes to sleep to save battery unit X 500ms  = 3 Minutes 

// globals
int old_cross_fader = 0;
int old_knob1 = 0;
int old_knob2 = 0;
int old_knob3 = 0; 
int tft_update = 0;
int batt =0; 

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Initializing bluetooth");
  BLEMidiServer.begin("MIDIFresh");
  Serial.println("Ready for connections...");

  // Setup Battery monitor
  maxlipo.begin();
  addr0x36 = true;

  // turn on backlite
  pinMode(TFT_BACKLITE, OUTPUT);
  digitalWrite(TFT_BACKLITE, HIGH);

  // Setup display
  pinMode(TFT_I2C_POWER, OUTPUT);
  digitalWrite(TFT_I2C_POWER, HIGH);
  delay(10);

  // initialize TFT
  tft.init(135, 240);  // Init ST7789 240x135
  tft.setRotation(3);


  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(0, 20);
  tft.setTextColor(ST77XX_CYAN);
  tft.setTextWrap(true);
  tft.setFont(&FreeSans18pt7b);
  tft.printf("MIDI");
  tft.setTextColor(ST77XX_WHITE);
  tft.print("Fresh");
  tft.setFont(&FreeSans12pt7b);
  tft.setTextColor(ST77XX_YELLOW);
  tft.printf("\nVersion %s\nBy Matt Perkins\nCopyright 2025\n", mainver);
  delay(2000);
}

void loop() {


  if (BLEMidiServer.isConnected()) {
    if(tft_update == 1){ 
      tft.fillScreen(ST77XX_BLACK);
      tft.setCursor(0, 25);
      tft.setTextColor(ST77XX_CYAN);
      tft.setTextWrap(true);
      tft.setFont(&FreeSans18pt7b);
      tft.printf("MIDI");
      tft.setTextColor(ST77XX_WHITE);
      tft.print("Fresh");
      tft.setFont(&FreeSans12pt7b);
      tft.setTextColor(ST77XX_YELLOW);
      tft.printf("\nConnected.\n");
      tft_update = 0; 
    }

    // Crossfader
    int cross_fader = map(analogRead(crossfader_pin), crossfader_min, crossfader_max, 0, 127);
    if (cross_fader != old_cross_fader) {
      old_cross_fader = cross_fader;
      Serial.printf("cross_fader = %d, \n", cross_fader);
      BLEMidiServer.controlChange(midi_channel, crossfader_midi, cross_fader);
    }

    // Knob 1
    int knob1 = map(analogRead(knob1_pin), knob1_min, knob1_max, 0, 127);
    if (knob1 != old_knob1) {
      old_knob1 = knob1;
      Serial.printf("knob1 = %d, \n", knob1);
      BLEMidiServer.controlChange(midi_channel, knob1_midi, knob1);
    }

    // Knob 2
    int knob2 = map(analogRead(knob2_pin), knob2_min, knob2_max, 0, 127);
    if (knob2 != old_knob2) {
      old_knob2 = knob2;
      Serial.printf("knob2 = %d, \n", knob2);
      BLEMidiServer.controlChange(midi_channel, knob2_midi, knob2);
    } 
    
    // Knob 3
    int knob3 = map(analogRead(knob3_pin), knob3_min, knob3_max, 0, 127);
    if (knob3 != old_knob3) {
      old_knob3 = knob3;
      Serial.printf("knob3 = %d, \n", knob3);
      BLEMidiServer.controlChange(midi_channel, knob3_midi, knob3);
    }

// Update the display with some usefull info 
  tft.setCursor(00, 80);
  tft.setTextColor(ST77XX_WHITE);
  batt = maxlipo.cellPercent();
  tft.printf("BAT %d %",batt);

  } else {
    // Check if we need to go to powersave 
       sleep_timeout = sleep_timeout - 1; 
      if(sleep_timeout ==0){
          esp_deep_sleep_start(); // Put ESP32 into deep sleep mode to save battery 
      }

    if (tft_update == 0) {
      // Display waiting to connect bluetooth.
      tft.fillScreen(ST77XX_BLACK);
      tft.setCursor(0, 25);
      tft.setTextColor(ST77XX_CYAN);
      tft.setTextWrap(true);
      tft.setFont(&FreeSans18pt7b);
      tft.printf("MIDI");
      tft.setTextColor(ST77XX_WHITE);
      tft.print("Fresh");
      tft.setFont(&FreeSans12pt7b);
      tft.setTextColor(ST77XX_YELLOW);
      tft.printf("\nWaiting for Bluetooth.\n");
      tft.setTextColor(ST77XX_RED);
      tft.printf("Will sleep if not found.");
      tft_update = 1;
   
    }
    delay(500); 
  }
}
