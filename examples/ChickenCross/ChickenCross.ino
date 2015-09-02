/*
  Chicken Cross
 Copyright (C) 2015 David Martinez
 All rights reserved.

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.
 */

#include <SPI.h>
#include <EEPROM.h>
#include "Arduboy.h"

#include "Collision.h"
#include "titlebmp.h"

Arduboy display;
Collision collision;

byte pad, pad2, pad3;   //Button press buffer used to stop pause repeating
byte oldpad, oldpad2, oldpad3;

#include "pins_arduino.h" // Arduino pre-1.0 needs this

int score = 0;
int lives = 3;
//high score variables
char text[16];
char initials[3];
///
boolean showTitle = true;
boolean enableSound = true;
boolean gameOver = false;
int timer;
int timerReset = 1300;
int titleScreenTimer = 0;
int titleScreenTimerReset = 2;

class Chicken {
  public:

    int x, y;
    const byte w = 8, h = 5;
    const int startX = WIDTH / 2 - 3, startY = HEIGHT - h;
    const byte hopDistance = 7;
};

class Egg{
  public:
  byte x,y;
  const byte minX = 2,maxX= 18, minY = 2,maxY=7,w = 6,h=7;
  boolean isActive = false;
  int eggTimerReset = random(1000,2000); 
  int eggTimer = eggTimerReset;
  
  void Update(){
    eggTimer --;
    if(eggTimer == 0){
      if(isActive == false){      
     isActive  = true; 
       display.tunes.tone(1200, 300);
     x = random(minX,maxX);
     x *= 7;
     y = random(minY,maxY) ;
     y *=7;
      }
    }if(eggTimer < -800){
     eggTimer =  eggTimerReset;
     isActive = false;
     //move offscreen
     x = 140;
     y = 140;
    }
  }

};

class Car {
  public:
    int x, y, xSpeed, delayReset = 4;
    byte w = 8, h = 7;
    int moveDelay = 0;
    boolean isActive = true;

    void Update() {
      if (isActive) {
        for (byte i = 0; i < 3; i++) {
          moveDelay--;
          if (moveDelay < 0 ) {
            moveDelay = delayReset;
            x += xSpeed;

            if (xSpeed < 0 & x < 0 - w)
              x = WIDTH + 10;
            if (xSpeed > 0 & x > WIDTH)
              x = 0 - w - 10;
          }
        }
      }
    }
};

extern const byte PROGMEM  chickenTitle[];

// speaker icon , had to split it in 4 in order to get it to work, a 32 x 32 image would not render properly
const byte speaker0[] PROGMEM = {B00000111, B00001111, B00001111, B00001111, B00001111, B00011111, B00111111, B01111111,};
const byte speaker1[] PROGMEM = {B11000000, B11100000, B11100000, B11100000, B11100000, B11110000, B11111000, B11111100,};
const byte speaker2[] PROGMEM = {B11111110, B00000000, B11100000, B00010000, B01100000, B10001100, B00110000, B11000000,};
const byte speaker3[] PROGMEM = {B11111111, B00000000, B00001111, B00010000, B00001100, B01100011, B00011000, B00000111,};

const byte chickenImg[] PROGMEM = {B00000010, B00000110, B00001110, B00001110, B00011100, B00001100, B00001111, B00000010,};

const unsigned char eggImg [] PROGMEM = {
0x3C, 0x66, 0x7B, 0x7F, 0x7E, 0x3C, 0x00, 0x00, 
};


const byte carImg[] PROGMEM = {
  B00111000, B10111010, B11111110, B10101010,
  B00101000, B10101010, B11111110, B10111010,
};

const byte truckImg[] PROGMEM = {
  B00001110, B00001110, B00011111, B00011111, B00001110,
  B00001110, B00011111, B00011111, B00011111, B00011111,
  B00011111, B00011111, B00011111, B00011111, B00011111,
  B00011111, B00011111, B00011111, B00011111, B00011111,
  B00011111, B00011111, B00011111, B00011111,
};

const byte tractorImg[] PROGMEM = {
  B01011101, B01110111, B01110111, B01111111, B01111111,
  B01111111, B01011101, B00010100, B00010100, B01111111,
};
const byte speakerImg[] PROGMEM = {
  B00000111, B11000000,
  B00001111, B11100000,
  B00001111, B11100000,
  B00001111, B11100000,
  B00001111, B11100000,
  B00011111, B11110000,
  B00111111, B11111000,
  B01111111, B11111100,
  B11111111, B11111110,
  B00000000, B00000000,
  B00001111, B11100000,
  B00010000, B00010000,
  B00001100, B01100000,
  B01100011, B10001100,
  B00011000, B00110000,
  B00000111, B11000000,
};

//Which came first? the chicken or the egg?
Chicken chicken;
Egg egg;

Car car[3];
Car truck[3];
Car truck2[3];
Car tractor[3];
Car car2[3];
Car tractor2[3];



void checkScore() {
  if (chicken.y < 4) {
    score += 100;

    if (enableSound)
      display.tunes.tone(1200, 300);
    delay(200);
    if (enableSound)
      display.tunes.tone(200, 200);
    delay(100);
    if (enableSound)
      display.tunes.tone(1500, 400);
    delay(200);
    chicken.x = chicken.startX;
    chicken.y = chicken.startY;
    timer = timerReset;
  }
}

void intro()
{
  for (int i = -8; i < 28; i = i + 2)
  {
    display.clearDisplay();
    display.setCursor(46, i);
    display.print("ARDUBOY");

    display.display();

  }
  if (enableSound)
    display.tunes.tone(987, 160);
  delay(160);
  if (enableSound)
    display.tunes.tone(1318, 400);
  delay(2000);
}

void setup()
{
  SPI.begin();
  display.start();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Hello World!");
  display.display();
  intro();

  chicken.x = chicken.startX;
  chicken.y = chicken.startY;
  timer = timerReset;
  titleScreenTimer = titleScreenTimerReset;

  for (byte i = 0; i < 3; i++) {
    car[i].x = 40 * i;
    car[i].y = HEIGHT - 14;
    car[i].xSpeed = -1 ;

    car2[i].x = 60 * i;
    car2[i].y = 29;
    car2[i].xSpeed = -1;
    car2[i].delayReset = 4;

    truck[i].x = 64 * i;
    truck[i].y = 37;
    truck[i].xSpeed = -1;
    truck[i].delayReset = 14;
    truck[i].w = 24;
    truck[i].h = 5;

    truck2[i].x = 40 * i;
    truck2[i].y = 14;
    truck2[i].xSpeed = -1;
    truck2[i].delayReset = 5;
    truck2[i].w = 24;
    truck2[i].h = 5;


    tractor[i].x = 50 * i;
    tractor[i].y = 43;
    tractor[i].xSpeed = 1;
    tractor[i].delayReset = 8;
    tractor[i].w = 10;
    tractor[i].h = 7;

    tractor2[i].x = 40 * i;
    tractor2[i].y = 22;
    tractor2[i].xSpeed = 1;
    tractor2[i].delayReset = 3;
    tractor2[i].w = 10;
    tractor2[i].h = 7;



  }

  //move one of the trucks offscreen
  truck[2].y = - 20;
  truck[2].isActive = false;


}

//Function by nootropic design to display highscores
boolean displayHighScores(byte file)
{
  display.setTextSize(1);
  if (titleScreenTimer > - 1) {
    byte y = 10;
    byte x = 24;
    // Each block of EEPROM has 10 high scores, and each high score entry
    // is 5 bytes long:  3 bytes for initials and two bytes for score.
    int address = file * 10 * 5;
    byte hi, lo;
    display.clearDisplay();
    display.setCursor(32, 0);
    display.print("HIGH SCORES");
    display.display();

    for (int i = 0; i < 10; i++)
    {
      sprintf(text, "%2d", i + 1);
      display.setCursor(x, y + (i * 8));
      display.print( text);
      display.display();
      hi = EEPROM.read(address + (5 * i));
      lo = EEPROM.read(address + (5 * i) + 1);

      if ((hi == 0xFF) && (lo == 0xFF))
      {
        score = 0;
      }
      else
      {
        score = (hi << 8) | lo;
      }

      initials[0] = (char)EEPROM.read(address + (5 * i) + 2);
      initials[1] = (char)EEPROM.read(address + (5 * i) + 3);
      initials[2] = (char)EEPROM.read(address + (5 * i) + 4);

      if (score > 0)
      {
        sprintf(text, "%c%c%c %u", initials[0], initials[1], initials[2], score);
        display.setCursor(x + 24, y + (i * 8));
        display.print(text);
        display.display();
      }
    }
    if (pollFireButton(300))
    {
      return true;
    }
    return false;
    display.display();
  } else {
    titleScreenTimer = titleScreenTimerReset;
  }
}

//Function by nootropic design to add high scores
void enterInitials()
{
  char index = 0;

  display.clearDisplay();

  initials[0] = ' ';
  initials[1] = ' ';
  initials[2] = ' ';

  while (true)
  {
    display.display();
    display.clearDisplay();

    display.setCursor(16, 0);
    display.print("HIGH SCORE");
    sprintf(text, "%u", score);
    display.setCursor(88, 0);
    display.print(text);
    display.setCursor(56, 20);
    display.print(initials[0]);
    display.setCursor(64, 20);
    display.print(initials[1]);
    display.setCursor(72, 20);
    display.print(initials[2]);
    for (byte i = 0; i < 3; i++)
    {
      display.drawLine(56 + (i * 8), 27, 56 + (i * 8) + 6, 27, 1);
    }
    display.drawLine(56, 28, 88, 28, 0);
    display.drawLine(56 + (index * 8), 28, 56 + (index * 8) + 6, 28, 1);
    delay(150);

    if (!digitalRead(PIN_RIGHT_BUTTON))
    {
      index--;
      if (index < 0)
      {
        index = 0;
      } else
      {
        if (enableSound)
          display.tunes.tone(1046, 250);
      }
    }

    if (!digitalRead(PIN_LEFT_BUTTON))
    {
      index++;
      if (index > 2)
      {
        index = 2;
      }  else {
        if (enableSound)
          display.tunes.tone(1046, 250);
      }
    }

    if (!digitalRead(PIN_UP_BUTTON))
    {
      initials[index]++;
      if (enableSound)
        display.tunes.tone(523, 250);
      // A-Z 0-9 :-? !-/ ' '
      if (initials[index] == '0')
      {
        initials[index] = ' ';
      }
      if (initials[index] == '!')
      {
        initials[index] = 'A';
      }
      if (initials[index] == '[')
      {
        initials[index] = '0';
      }
      if (initials[index] == '@')
      {
        initials[index] = '!';
      }
    }

    if (!digitalRead(PIN_DOWN_BUTTON))
    {
      initials[index]--;
      if (enableSound)
        display.tunes.tone(523, 250);
      if (initials[index] == ' ') {
        initials[index] = '?';
      }
      if (initials[index] == '/') {
        initials[index] = 'Z';
      }
      if (initials[index] == 31) {
        initials[index] = '/';
      }
      if (initials[index] == '@') {
        initials[index] = ' ';
      }
    }

    if (!digitalRead(PIN_A_BUTTON))
    {
      if (index < 2)
      {
        index++;
        if (enableSound)
          display.tunes.tone(1046, 250);
      } else {
        if (enableSound)
          display.tunes.tone(1046, 250);
        return;
      }
    }
  }

}

void enterHighScore(byte file)
{
  // Each block of EEPROM has 10 high scores, and each high score entry
  // is 5 bytes long:  3 bytes for initials and two bytes for score.
  int address = file * 10 * 5;
  byte hi, lo;
  char tmpInitials[3];
  unsigned int tmpScore = 0;

  // High score processing
  for (byte i = 0; i < 10; i++)
  {
    hi = EEPROM.read(address + (5 * i));
    lo = EEPROM.read(address + (5 * i) + 1);
    if ((hi == 0xFF) && (lo == 0xFF))
    {
      // The values are uninitialized, so treat this entry
      // as a score of 0.
      tmpScore = 0;
    } else
    {
      tmpScore = (hi << 8) | lo;
    }
    if (score > tmpScore)
    {
      enterInitials();
      for (byte j = i; j < 10; j++)
      {
        hi = EEPROM.read(address + (5 * j));
        lo = EEPROM.read(address + (5 * j) + 1);

        if ((hi == 0xFF) && (lo == 0xFF))
        {
          tmpScore = 0;
        }
        else
        {
          tmpScore = (hi << 8) | lo;
        }

        tmpInitials[0] = (char)EEPROM.read(address + (5 * j) + 2);
        tmpInitials[1] = (char)EEPROM.read(address + (5 * j) + 3);
        tmpInitials[2] = (char)EEPROM.read(address + (5 * j) + 4);

        // write score and initials to current slot
        EEPROM.write(address + (5 * j), ((score >> 8) & 0xFF));
        EEPROM.write(address + (5 * j) + 1, (score & 0xFF));
        EEPROM.write(address + (5 * j) + 2, initials[0]);
        EEPROM.write(address + (5 * j) + 3, initials[1]);
        EEPROM.write(address + (5 * j) + 4, initials[2]);

        // tmpScore and tmpInitials now hold what we want to
        //write in the next slot.
        score = tmpScore;
        initials[0] = tmpInitials[0];
        initials[1] = tmpInitials[1];
        initials[2] = tmpInitials[2];
      }

      score = 0;
      initials[0] = ' ';
      initials[1] = ' ';
      initials[2] = ' ';

      return;
    }
  }
}






void movePlayer()
{
  //Move right

  if ( !digitalRead(PIN_RIGHT_BUTTON) & chicken.x < WIDTH - chicken.w - 4 )
  {
    pad = PIN_RIGHT_BUTTON;
    if (pad != oldpad) {
      if (enableSound)
        display.tunes.tone(987, 60);
      chicken.x += chicken.hopDistance;
      oldpad = pad;
    }
  }

  //Move left

  else if ( !digitalRead(PIN_LEFT_BUTTON) & chicken.x > 0 + 5 )
  {
    pad = PIN_LEFT_BUTTON;
    if (pad != oldpad) {
      if (enableSound)
        display.tunes.tone(987, 60);
      chicken.x -= chicken.hopDistance;
      oldpad = pad;
    }
  }
  else if ( !digitalRead(PIN_UP_BUTTON))
  {
    pad = PIN_UP_BUTTON;
    if (pad != oldpad) {
      if (enableSound)
        display.tunes.tone(987, 60);
      chicken.y -= chicken.hopDistance;
      oldpad = pad;
    }
  }

  else if ( !digitalRead(PIN_DOWN_BUTTON) & chicken.y < HEIGHT - chicken.h)
  {
    pad = PIN_DOWN_BUTTON;
    if (pad != oldpad) {
      if (enableSound)
        display.tunes.tone(987, 60);
      chicken.y += chicken.hopDistance;
      oldpad = pad;
    }
  }


  else {
    oldpad = 0;
  }

}






//Used to delay images while reading button input
boolean pollFireButton(int n)
{
  for (int i = 0; i < n; i++)
  {
    delay(15);
    pad = !digitalRead(PIN_A_BUTTON);
    if (pad == 1 && oldpad == 0)
    {
      oldpad3 = 1; //Forces pad loop 3 to run once
      return true;
    }
    oldpad = pad;
  }
  return false;
}





boolean titleScreen()
{
  score = 0;
  lives = 3;
  timer = timerReset;

  titleScreenTimer --;
  if (titleScreenTimer > 0) {
    //Clears the screen
    display.clearDisplay();


    display.setCursor(12, 5);
    display.setTextSize(2);
    //display.print("Chicken");
    display.setCursor(60, 25);
    //display.print("Cross");
    display.setTextSize(1);
    display.drawBitmap(0, 0, chickenTitle, 128, 64, 1);
    // draw speaker icon in sections // could not get a 32x32 image to render properly
    if (enableSound) {
      display.drawBitmap(106, 56, speaker0, 8, 8, 0);
      display.drawBitmap(106, 56 - 8, speaker1, 8, 8, 0);
      display.drawBitmap(106 + 8, 56 - 8, speaker2, 8, 8, 0);
      display.drawBitmap(106 + 8, 56, speaker3, 8, 8, 0);
      display.display();
    }
    if (pollFireButton(25))
    {
      showTitle = false;
      randomSeed(millis());

    }
    if ( !digitalRead(PIN_B_BUTTON) )
    {
      pad = 1;
      if (oldpad != pad) {
        if (enableSound) {
          enableSound = false;

        } else {
          enableSound = true;
        }
      }
    }  else {
      pad = 0;
    }
    oldpad = pad;

    //Flash "Press FIRE" 5 times
    for (byte i = 0; i < 5; i++)
    {
      //Draws "Press FIRE"


      display.setCursor(31, 53);
      display.print("PRESS FIRE!");
      display.display();

      if (pollFireButton(50))
      {
        showTitle = false;
        randomSeed(millis());
        break;
      }

      if ( !digitalRead(PIN_B_BUTTON) )
      {
        pad = 1;
        if (enableSound) {
          enableSound = false;
          ;
        } else {
          enableSound = true;
        }
      }  else {
        pad = 0;
      }
      //Removes "Press FIRE"
      oldpad = pad;
      display.clearDisplay();

      display.setCursor(12, 5);
      display.setTextSize(2);
      //  display.print("Chicken");
      display.setCursor(60, 25);
      //  display.print("Cross");
      display.drawBitmap(0, 0, chickenTitle, 128, 64, 1);
      // draw speaker icon in sections // could not get a 32x32 image to render properly
      if (enableSound) {
        display.drawBitmap(106, 56, speaker0, 8, 8, 0);
        display.drawBitmap(106, 56 - 8, speaker1, 8, 8, 0);
        display.drawBitmap(106 + 8, 56 - 8, speaker2, 8, 8, 0);
        display.drawBitmap(106 + 8, 56, speaker3, 8, 8, 0);
      }
      display.setTextSize(1);
      display.display();

      display.display();
      if (pollFireButton(25))
      {
        showTitle = false;
        randomSeed(millis());
        break;
      }
      if ( !digitalRead(PIN_B_BUTTON) )
      {
        pad = 1;
        if (oldpad != pad) {
          if (enableSound) {
            enableSound = false;

          } else {
            enableSound = true;
          }
        }
      } else {
        pad = 0;
      }
    }
    oldpad = pad;
  } else {
    displayHighScores(3);
  }

}

void checkCollision() {
  boolean collisionDetected = false;
  for (byte i = 0; i < 3; i++) {
    if ( collision.collideRectRect(chicken.x, chicken.y, chicken.w, chicken.h, car[i].x, car[i].y, car[i].w, car[i].h)) {
      collisionDetected = true;
    }
    if ( collision.collideRectRect(chicken.x, chicken.y, chicken.w, chicken.h, car2[i].x, car2[i].y, car2[i].w, car2[i].h)) {
      collisionDetected = true;
    }
    if ( collision.collideRectRect(chicken.x, chicken.y, chicken.w, chicken.h, truck[i].x, truck[i].y, truck[i].w, truck[i].h)) {
      collisionDetected = true;
    }
    if ( collision.collideRectRect(chicken.x, chicken.y, chicken.w, chicken.h, truck2[i].x, truck2[i].y, truck2[i].w, truck2[i].h)) {
      collisionDetected = true;
    }
    if ( collision.collideRectRect(chicken.x, chicken.y, chicken.w, chicken.h, tractor[i].x, tractor[i].y, tractor[i].w, tractor[i].h)) {
      collisionDetected = true;
    }
    if ( collision.collideRectRect(chicken.x, chicken.y, chicken.w, chicken.h, tractor2[i].x, tractor2[i].y, tractor2[i].w, tractor2[i].h)) {
      collisionDetected = true;
    }
  }
  if (collisionDetected) {
    if (enableSound)
      display.tunes.tone(200, 250);
    delay(150);
    if (enableSound)
      display.tunes.tone(180, 250);
    delay(400);
    lives --;
    timer = timerReset;
    chicken.x = chicken.startX;
    chicken.y = chicken.startY;
    if (lives < 1) {
      gameOver = true;
    }
  }
  
  //check egg
    if ( collision.collideRectRect(chicken.x, chicken.y, chicken.w, chicken.h, egg.x, egg.y, egg.w, egg.h)) {
      egg.isActive = false;
      egg.x = 160;
      egg.y = 160;
      score += 300;
      display.tunes.tone(900,250);
      egg.eggTimer = egg.eggTimerReset;
    }
}
void checkTimer() {
  timer--;
  if (timer < 0) {
    gameOver = true;
    if (enableSound)
      display.tunes.tone(200, 250);
    delay(150);
    if (enableSound)
      display.tunes.tone(180, 250);
    delay(400);
  }
}
void loop()
{
  if (!showTitle & !gameOver) {
    display.display();
    checkScore();
    checkCollision();
    
    checkTimer();
    display.clearDisplay();
    display.fillRect(0, 0, WIDTH, 8, 1);
    display.fillRect(0, HEIGHT - 6, WIDTH, 8, 1);
    display.setCursor(WIDTH / 2 - 10, 1);
    display.print(score);
    display.setCursor(WIDTH - 24, HEIGHT - 6);
    display.print(timer);



    movePlayer();

    for (byte i = 0; i < 3; i++) {
      car[i].Update();
      car2[i].Update();
      truck[i].Update();
      truck2[i].Update();
      tractor[i].Update();
      tractor2[i].Update();
      
    }
    egg.Update();
    ///////////
    byte color;
    if (chicken.y < 4 | chicken.y > HEIGHT - 9) {
      color = 0;
    } else {
      color = 1;
    }
    display.drawBitmap(chicken.x, chicken.y, chickenImg, 8, 8, color);
    //////////
    for (byte i = 0; i < 3; i++) {

      display.drawBitmap(car[i].x, car[i].y, carImg, 8, 8, 1);
      display.drawBitmap(car2[i].x, car2[i].y, carImg, 8, 8, 1);
      if (truck[i].isActive)
        display.drawBitmap(truck[i].x, truck[i].y, truckImg, 24, 8, 1);
      display.drawBitmap(truck2[i].x, truck2[i].y, truckImg, 24, 8, 1);
      display.drawBitmap(tractor[i].x, tractor[i].y, tractorImg, 10, 8, 1);
      display.drawBitmap(tractor2[i].x, tractor2[i].y, tractorImg, 10, 8, 1);

    }
    
    //draw egg only if active
    if(egg.isActive){
     display.drawBitmap(egg.x,egg.y,eggImg,8,8,1); 
    }


  } else if (showTitle)
  {
    titleScreen();
  }
  else if (gameOver) {
    display.clearDisplay();
    display.setCursor(14, 30);
    display.setTextSize(2);
    if (timer > 0) {
      display.print("Game Over");
    } else {
      display.print("Time Over");
    }
    display.display();
    delay(3000);
    display.setTextSize(1);
    enterHighScore(3);
    gameOver = false;
    showTitle = true;
    lives = 3;
    score = 0;
    timer = timerReset;
    titleScreenTimer = titleScreenTimerReset;
    chicken.x = chicken.startX;
    chicken.y - chicken.startY;
  }
}
