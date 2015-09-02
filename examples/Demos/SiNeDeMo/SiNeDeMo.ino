// This is the SiNe-DeMo created for Arduboy
// it started on Thursday 11 june 2015
// contributers are:
// JO3RI http://www.JO3RI.be
//

// All of this could have been done with precaculated values
// even with bitmaps, but what is the funin that. I wanted to see
// how fast the demo can run with pure calculation.




#include <SPI.h>
#include "Arduboy.h"

#include <EEPROM.h>
#include "bitmaps.h"

Arduboy arduboy;

int i = 0;
float d = 0.0;
int c = 129;

void setup() {
  // Cleanup the display
  //______________________________
  arduboy.start();
  arduboy.clearDisplay();
  arduboy.display();
  
  // first we'll show some bitmaps
  //______________________________
  
  // JO3RI Logo
  arduboy.drawBitmap(20, 4, JO3RI, 88, 56, 1);
  arduboy.display();
  delay(3000);
  arduboy.clearDisplay();
  
  //double the text height
  arduboy.setTextSize(2);
  
  //member
  arduboy.setCursor(10, 25);
  arduboy.print("Member of");
  arduboy.display();
  delay(1500);
  arduboy.clearDisplay();
  
  // TEAM a.r.g. Logo
  arduboy.drawBitmap(14, 16, TEAMarg, 96, 32, 1);
  arduboy.display();
  delay(3000);
  arduboy.clearDisplay();
  
  //presents
  arduboy.setCursor(10, 10);
  arduboy.print("Presents:");
  
  //SiNe-DeMo
  arduboy.setCursor(10, 35);
  arduboy.print("SiNe-DeMo");
  arduboy.display();  
  delay(3000);
  arduboy.clearDisplay();
}

void loop() {
  
  float p = 2 * sin(d);
  float q = 2 * sin(-d);
  arduboy.clearDisplay();
  
  for (byte k = 2; k < 15; k++) {
    // some calculations are used multiple times
    // so better caclutate them once and reuse
    // the results.
    float r = sin(d + (k * 0.4));
    float t = sin(d + 3 + (k * 0.4));

    // First lets draw the top 12 bals
    arduboy.fillCircle(k * 8 + p, 12 + (r * 10), 2, 1);
    // and now the bottom 12 balls
    arduboy.fillCircle(k * 8 + q, 12 + (t * 10), 2, 1);

    // now we draw a line from the top ball to the bottom ball
    arduboy.drawLine(k * 8 + p, 10 + (r * 10), k * 8 + q, (10 + t * 10), 1);

    // and finally we need a line from ball to ball on the top part
    arduboy.drawLine((k * 8 + p), 12 + (r * 10), 6 + (k * 8 + p), (12 + sin(d + ((k + 1) * 0.4)) * 10), 1);

    // and the bottom part
    arduboy.drawLine((k * 8 + q), 12 + (t * 10), 6 + (k * 8 + q), (12 + sin(d + 3 + ((k + 1) * 0.4)) * 10), 1);
  }

  // begin wobbling text
  //____________________

  // We could use font, but it's to small, so I made
  // little bitmaps for each letter. Next to that, if
  // we would use font, we need each letter to separated
  // in an array.
  // Now let's put each letter on the sine wave
  // int c makes the letters move right to left
  // and sine calculation makes them go up an down
  arduboy.drawBitmap(c, 32 - sin(d + (0 * 0.4)) * 5, LetterH, 8, 8, 1);
  arduboy.drawBitmap(c + 10, 32 - sin(d + (1 * 0.4)) * 5, LetterE, 8, 8, 1);
  arduboy.drawBitmap(c + 20, 32 - sin(d + (2 * 0.4)) * 5, LetterL, 8, 8, 1);
  arduboy.drawBitmap(c + 30, 32 - sin(d + (3 * 0.4)) * 5, LetterL, 8, 8, 1);
  arduboy.drawBitmap(c + 40, 32 - sin(d + (4 * 0.4)) * 5, LetterO, 8, 8, 1);
  arduboy.drawBitmap(c + 50, 32 - sin(d + (5 * 0.4)) * 5, Letter, 8, 8, 1);
  arduboy.drawBitmap(c + 60, 32 - sin(d + (6 * 0.4)) * 5, LetterA, 8, 8, 1);
  arduboy.drawBitmap(c + 70, 32 - sin(d + (7 * 0.4)) * 5, LetterR, 8, 8, 1);
  arduboy.drawBitmap(c + 80, 32 - sin(d + (8 * 0.4)) * 5, LetterD, 8, 8, 1);
  arduboy.drawBitmap(c + 90, 32 - sin(d + (9 * 0.4)) * 5, LetterU, 8, 8, 1);
  arduboy.drawBitmap(c + 100, 32 - sin(d + (10 * 0.4)) * 5, LetterB, 8, 8, 1);
  arduboy.drawBitmap(c + 110, 32 - sin(d + (11 * 0.4)) * 5, LetterO, 8, 8, 1);
  arduboy.drawBitmap(c + 120, 32 - sin(d + (12 * 0.4)) * 5, LetterY, 8, 8, 1);
  // end wobbling text


  // rotating lines
  //_______________

  // each line is devided in 16 small parts (byte k)
  // the small parts are actually rectangles growing bigger
  // and smaller so they seem to come in to the front and then
  // go to the background.

  // growing and shrinking of the rectangles is a sine calculation
  // and so is the moving of the lines too.
  for (byte k = 2; k < 20; k++) {
    // the first line
    arduboy.drawRect(k * 6 + (p / 2), 54 + (sin(d + (k * 0.1)) * 6), 4, 1 + (p / 2), 1);
    // the second line is actually the same line,
    // but evolved 1/3 further in the sine calculation
    arduboy.drawRect(k * 6 + (sin(d + 2)), 54 + (sin(d + 2 + (k * 0.1)) * 6), 4, 1 + sin(d + 2), 1);
    // the third line is actually the same line,
    // but evolved 2/3 further in the sine calculation
    arduboy.drawRect(k * 6 + (sin(d + 4)), 54 + (sin(d + 4 + (k * 0.1)) * 6), 4, 1 + sin(d + 4), 1);
  }
  // int c is supstracted to move those letters
  // right to left. If all letters are offscreen
  // let's start over
  c--;
  if (c == -130) c = 129;

  // float d goes 1 step further on the sine evolution
  d = d + 0.10;

  // update the screen so all those parts are drown
  arduboy.display();
}

