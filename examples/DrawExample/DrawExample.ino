/**
 * @file DrawExample.ino
 * \brief Implements a test of the draw routine from the ArduboyBase class.
 */

#include <Arduboy.h>

// make an instance of arduboy used for many functions
Arduboy arduboy;

void setup() {
  arduboy.begin();
  arduboy.setFrameRate(15);
}

void loop() {
  // call the draw routine from the arduboy reference
  arduboy.draw(draw);
}

// function to pass to the Arduboy draw routine
void draw() {
  arduboy.setCursor(4, 9);
  arduboy.print(F("Hello, world!"));
}
