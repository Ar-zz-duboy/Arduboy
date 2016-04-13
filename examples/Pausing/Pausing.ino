#include "Arduboy.h"

Arduboy arduboy;

bool isPaused = false;

void setup() {
  // put your setup code here, to run once:
  arduboy.begin();
  arduboy.setFrameRate(60);
  //arduboy.setupPause(1);
}

void loop() {
  if (!(arduboy.nextFrame())) return;
  arduboy.clear();
  // put your main code here, to run repeatedly:
  if(isPaused){
    arduboy.drawCircle(WIDTH/2, HEIGHT/2, 9, WHITE);
  }
  isPaused = arduboy.checkPause(isPaused);
  arduboy.display();
}
