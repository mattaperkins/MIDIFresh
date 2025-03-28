#include <Arduino.h>
// Simple program to read a pot so lower and uppter limit can be tested. 

void setup() {
  Serial.begin(115200);
 
  static int cross_fader = 0; 
  static int old_cross_fader = 0; 
}

void loop() {  
      int cross_fader = analogRead(A1);
      int old_cross_fader; 
    if ( cross_fader != old_cross_fader) {
      int old_cross_fader = cross_fader; 
      Serial.printf("pot value = %d, \n", cross_fader);
      delay(100);
    }
  
}
