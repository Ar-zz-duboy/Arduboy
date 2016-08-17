/**
 * @file DrawExample.ino
 * \brief Implements a test of the draw routine from the ArduboyBase class.
 */

#include "Arduboy.h"

// make an instance of arduboy used for many functions
Arduboy arduboy;

// function to pass to the Arduboy perform routine in loop()
void action() {
  arduboy.setCursor(4, 9);
  arduboy.print(F("Hello, world!"));
}

void setup() {
  arduboy.begin();
  arduboy.setFrameRate(15);
}

void loop() {
  // perform the action() function specified below.
  arduboy.perform(action);
}
