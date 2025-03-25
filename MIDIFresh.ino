#include <Arduino.h>
#include <BLEMidi.h>

// Configuration
const uint8_t midi_channel = 0;

// Crossfader
int crossfader_pin = A2;    // crossfader anaglog pin
int crossfader_min = 0;     // Minimum value for crossfader
int crossfader_max = 4096;  // maximum value for crossfader
int crossfader_midi = 1;    // MIDI Control # for crossfader

// Knob1 
int knob1_pin = A1;    // knob1 anaglog pin
int knob1_min = 0;     // Minimum value for knob1
int knob1_max = 4096;  // maximum value for knob1
int knob1_midi = 2;    // MIDI Control # for knob1


void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Initializing bluetooth");
  BLEMidiServer.begin("MIDIFresh");
  Serial.println("Ready for connections...");
}

void loop() {


  if (BLEMidiServer.isConnected()) {

    // Crossfader 
    int cross_fader = map(analogRead(crossfader_pin), crossfader_min, crossfader_max, 0, 127);
    int old_cross_fader;
    if (cross_fader != old_cross_fader) {
      int old_cross_fader = cross_fader;
      Serial.printf("cross_fader = %d, \n", cross_fader);
      BLEMidiServer.controlChange(midi_channel, crossfader_midi, cross_fader);
    }
    
    // Knob 1 
    int knob1 = map(analogRead(knob1_pin), knob1_min, knob1_max, 0, 127);
    int old_knob1;
    if(knob1 != old_knob1) {
      int old_knob1 = knob1;
      Serial.printf("knob1 = %d, \n", knob1);
      BLEMidiServer.controlChange(midi_channel, knob1_midi, knob1);
    }
  }
}
