

/*
  Snakeling for Arduboy
 Copyright (C) 2015 David Martinez
 All rights reserved.
 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.
 */



#include <SPI.h>
#include <Wire.h>
#include "ArduboyDev.h"
#include "Collision.h"
#include <EEPROM.h>
Arduboy display;
Collision collision;
const byte width = 128;   //Width of screen
const byte height = 64;   //Hight of screen
#define RIGHT 5
#define LEFT 9
#define UP 8
#define DOWN 10
#define A_BTN A0
#define B_BTN A1
#define BLACK 0
#define WHITE 1

boolean gameover = false;
boolean showTitle = true;

int x = 0;
int y = 0;
//high score variables
char text[16];
char initials[3];
///
int score = 0;
byte activeNum = 0;
byte     moveAmount = 3;
const int unitSize = 3;
byte  moveDelayReset = 5;
int slowDelay = 9;
byte medDelay = 5;
byte fastDelay = 3;
int moveDelay = moveDelayReset;

byte direct = RIGHT;
int pressATimer = 0;
const int pressATimerReset = 30;
const byte maxUnits = 200  ; //the maximum body segmants you can have///
const byte startX = 4 * unitSize;
const byte startY = 8 * unitSize;





class Unit {
  public:
    byte x, y;


};
Unit body[maxUnits]; //makes maxUnits number of body segments.
Unit apple;

void setup() {
  // put your setup code here, to run once:
  SPI.begin();
  display.start();

  intro();

  display.setTextSize(1);
  display.setCursor(x, y);

  pressATimer = pressATimerReset;

}



void intro()
{
  display.setTextSize(1);
  for (int i = -8; i < 28; i = i + 2)
  {
    display.clearDisplay();
    display.setCursor(46, i);

    display.print("ARDUBOY");

    display.display();
    delay(2);
  }

  tone(A2, 987, 160);
  delay(160);

  tone(A2, 1318, 400);
  delay(2000);
}

//Function by nootropic design to display highscores
void displayHighScores(byte file)
{

  display.setTextSize(1);

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
      //display.display();
    }
  }
  display.display();
  boolean exitLoop = false;
  while (!exitLoop) {
    if (!digitalRead(B_BTN))
    {
      delay(300);
      exitLoop = true;

    }
  }

  //delay(300);

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

    if (!digitalRead(5))
    {
      index--;
      if (index < 0)
      {
        index = 0;
      } else
      {

        tone(A2, 1046, 250);
      }
    }

    if (!digitalRead(9))
    {
      index++;
      if (index > 2)
      {
        index = 2;
      }  else {

        tone(A2, 1046, 250);
      }
    }

    if (!digitalRead(8))
    {
      initials[index]++;

      tone(A2, 523, 250);
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

    if (!digitalRead(10))
    {
      initials[index]--;

      tone(A2, 523, 250);
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

    if (!digitalRead(A0))
    {
      if (index < 2)
      {
        index++;

        tone(A2, 1046, 250);
      } else {

        tone(A2, 1046, 250);
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
  display.setTextSize(1);
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



void drawUnits() {
  if (activeNum > 0) {
    for (byte i = 0; i < activeNum - 1; i++) {

      display.fillRect(body[i].x, body[i].y, unitSize, unitSize, WHITE);

    }
  }
}
void updateUnits() {
  int oldPosX, oldPosY, newPosX, newPosY;
  newPosX = x;
  newPosY = y;
  if (activeNum > 0) {
    for (byte i = 0; i < maxUnits; i++) {

      oldPosX = body[i].x;
      oldPosY = body[i].y;

      body[i].x = newPosX;
      body[i].y = newPosY;
      newPosX = oldPosX;
      newPosY = oldPosY;
    }


  }
}
void checkCollision() {
  if (x < 0 | x > width - unitSize | y > height - unitSize | y < 0) {

    delay(500);
    gameover = true;

  }

  if (activeNum > 0) {
    for (byte i = 0; i < activeNum - 1; i++) {

      if (collision.collidePointRect(x, y, body[i].x, body[i].y, unitSize, unitSize)) {
        delay(500);
        gameover = true;

      }

    }
  }

  if (collision.collideRectRect(apple.x, apple.y, unitSize, unitSize, x, y, unitSize, unitSize)) {
    tone(A2, 800, 200);
    if (moveDelayReset == slowDelay)
      score += 5;
    if (moveDelayReset == medDelay)
      score += 10;
    if (moveDelayReset == fastDelay)
      score += 20;


    resetApple();
    addUnit();
  }
}
void addUnit() {
  activeNum ++;
  if (activeNum > maxUnits)
    activeNum = maxUnits;

}

void resetApple() {
  apple.x = random(4, 40); apple.x *= unitSize;
  apple.y = random(4, 20); apple.y *= unitSize;

}

void updateHead() {

  if (!digitalRead(RIGHT) & direct != LEFT) {
    direct = RIGHT;
  }
  if (!digitalRead(LEFT) & direct != RIGHT) {
    direct = LEFT;
  }
  if (!digitalRead(DOWN)& direct != UP) {
    direct = DOWN;
  }
  if (!digitalRead(UP)& direct != DOWN) {
    direct = UP;
  }

  moveDelay --;
  if (moveDelay < 0) {


    moveDelay = moveDelayReset;
    updateUnits();
    switch (direct) {
      case RIGHT:
        x += moveAmount;
        break;

      case LEFT:
        x -= moveAmount;
        break;
      case UP:
        y -= moveAmount;
        break;
      case DOWN:
        y += moveAmount;
        break;
    }
  }
}
void resetSnake() {
  activeNum = 0;


  x = startX;
  y = startY;
  direct = RIGHT;
}

void titleScreen()
{

  //Clears the screen
  display.clearDisplay();
  display.setCursor(8, 16);
  display.setTextSize(2);
  display.print("SNAKELING");
  display.setTextSize(1);
  //display.display();

  if (!digitalRead(A_BTN))
  {

    menuSelect();

    showTitle = false;
    randomSeed(millis());
    resetApple();
    x = startX;
    y = startY;

  }

  if (!digitalRead(B_BTN)) {
    delay(300);
    displayHighScores(4);
  }
  if (showTitle) {
    pressATimer --;
    if (pressATimer > 0) {

      //Draws "Press FIRE"
      //display.bitmap(31, 53, fire);  display.display();
      display.setCursor(16, 40);
      display.print("PRESS A To Start");
      display.setCursor(9, 50);
      display.print("PRESS B For Scores");
      display.display();

    } else if (pressATimer < 0) {
      //Removes "Press FIRE"
      display.clearDisplay();
      display.setCursor(8, 16);
      display.setTextSize(2);
      display.print("SNAKELING");
      display.setTextSize(1);
      display.display();

      display.display();
    }
    if (pressATimer < -pressATimerReset) {
      pressATimer = pressATimerReset;
    }


  }
}

void menuSelect() {
  boolean exitLoop = false;
  byte pointerX = 10;
  byte pointerY = 26;
  int currentSelected = 0;

  display.clearDisplay();
  display.drawRect(0, 0, width, height, WHITE);
  display.setCursor(14, 30);
  display.print("Slow   Med   Fast");
  display.drawRect(pointerX, pointerY, 30, 18, WHITE);
  display.setCursor(28, 5);
  display.setTextSize(1);
  display.print("Speed Select");
  display.display();
  delay(800);
  while (!exitLoop) {
    display.clearDisplay();
    display.drawRect(0, 0, width, height, WHITE);
    display.setCursor(14, 30);
    display.print("Slow   Med   Fast");
    display.drawRect(pointerX, pointerY, 30, 18, WHITE);
    display.setCursor(28, 5);
    display.setTextSize(1);
    display.print("Speed Select");
    display.display();

    if (!digitalRead(RIGHT)) {
      currentSelected ++;
      if (currentSelected > 2) {
        currentSelected = 0;

      }
      tone(A2, 400, 200);
      delay(300);
    }

    if (!digitalRead(LEFT)) {
      currentSelected --;
      if (currentSelected < 0) {
        currentSelected = 2;
      }
      tone(A2, 400, 200);
      delay(300);
    }
    if (!digitalRead(A_BTN)) {
      if (currentSelected == 0)
        moveDelayReset = slowDelay;
      if (currentSelected == 1)
        moveDelayReset = medDelay;
      if (currentSelected == 2)
        moveDelayReset = fastDelay;

      display.fillRect(pointerX, pointerY, 30, 18, WHITE);
      display.display();
      delay(400);
      display.clearDisplay();
      display.display();

      exitLoop = true;
    }
    if (currentSelected == 0) {
      pointerX = 10;
    }
    if (currentSelected == 1) {
      pointerX = 50;
    }
    if (currentSelected == 2) {
      pointerX = 88;
    }

  }
}

void gameoverScreen() {
  display.clearDisplay();
  display.setCursor(12, 24);
  display.setTextSize(2);
  display.print("Game Over");
  display.display();
  delay(4000);
  enterHighScore(4);
  resetPlayArea();

}
void resetPlayArea() {
  score = 0;
  resetApple();
  x = startX;
  y = startY;
  direct = RIGHT;
  resetSnake();
  showTitle = true;
  gameover = false;
}

void loop() {
  // put your main code here, to run repeatedly:
  if (!showTitle & !gameover) {



    display.drawRect(0, 0, width, height, WHITE);
    display.fillRect(x, y, unitSize, unitSize, WHITE);
    display.setCursor(width / 2, 3);
    display.print(score);
    drawUnits();
    ////draw apple
    display.drawRect(apple.x, apple.y, unitSize, unitSize, WHITE);
    display.drawPixel(apple.x + 1, apple.y - 1, WHITE);
    display.drawPixel(apple.x, apple.y - 2, WHITE);
    /////////

    //display.drawBitmap(apple.x,apple.y,appleImg,8,8,WHITE);
    display.display();

    display.clearDisplay();
    updateHead();
    checkCollision();
    display.setCursor(x, y);
    delay (moveDelay);


  } else if (showTitle) {
    titleScreen();
  } else if (gameover) {
    delay(1000);
    gameoverScreen();

  }

}
