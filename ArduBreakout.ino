 /*
  Breakout
 Copyright (C) 2011 Sebastian Goscik
 All rights reserved.
 
 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.
 */

#include <SPI.h>
#include <Wire.h>
#include "Arduboy.h"

#include <EEPROM.h>
#include "breakout_bitmaps.h"

#define OLED_DC 8
#define OLED_CS 10   // SPI slave-select
#define OLED_CLK 13  // hardware SPI clock
#define OLED_MOSI 11   // hardware SPI MOSI
#define OLED_RESET 7

Arduboy display;

const byte width = 128;   //Width of screen
const byte height = 64;   //Hight of screen
int dx = -1;        //Initial movement of ball
int dy = -1;        //Initial movement of ball
int xb;           //Balls starting possition
int yb;           //Balls starting possition
boolean released;     //If the ball has been released by the player
boolean paused = false;   //If the game has been paused
byte xPaddle;       //X position of paddle
boolean isHit[5][12];   //Array of if bricks are hit or not
boolean bounced=false;  //Used to fix double bounce glitch
byte lives = 3;       //Amount of lives
byte level = 1;       //Current level
unsigned int score=0;   //Score for the game
unsigned int brickCount;  //Amount of bricks hit
byte pad,pad2,pad3;     //Button press buffer used to stop pause repeating
byte oldpad,oldpad2,oldpad3;
char text[16];      //General string buffer
boolean start=false;    //If in menu or in game
boolean initialDraw=false;//If the inital draw has happened
char initials[3];     //Initials used in high score

//Ball Bounds used in collision detection
byte leftBall;
byte rightBall;
byte topBall;
byte bottomBall;

//Brick Bounds used in collision detection
byte leftBrick;
byte rightBrick;
byte topBrick;
byte bottomBrick;

int ballclock = 0;

#include "pins_arduino.h" // Arduino pre-1.0 needs this

PROGMEM const unsigned char arduino [] =
{
  0x3F, 0xFF, 0xFF, 0xFC, 0x40, 0x00, 0x00, 0x02, 0x89, 0x99,0x54,
  0x91, 0x95, 0x55, 0x56, 0xA9, 0x9D, 0x95, 0x55, 0xA9, 0x95, 0x59,
  0xD4, 0x91, 0x40, 0x00, 0x00, 0x02, 0x3F, 0xFF, 0xFF, 0xFC
};

void intro()
{
  for(int i = -8; i < 28; i = i + 2)
  {
    display.clearDisplay();
    display.setCursor(46, i);
    display.print("ARDUBOY");
    display.display();
  }

  tone(A2, 987, 160);
  delay(160);
  tone(A2, 1318, 400);
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
}

void movePaddle()
{
  //Move right
  if(xPaddle < width - 12)
  {
    if( !digitalRead(5) )
    {
      xPaddle++;
    }
  }

  //Move left
  if(xPaddle > 0)
  {
    if( !digitalRead(9))
    {  
      xPaddle--;
    }
  } 
}

void moveBall()
{
  if(released)
  {
    //Move ball
    xb=xb + dx;
    yb=yb + dy;

    //Set bounds
    leftBall = xb;
    rightBall = xb + 2;
    topBall = yb;
    bottomBall = yb + 2;

    //Bounce off top edge
    if (yb <= 0)
    {
      yb = 2;
      dy = -dy;
      tone(A2, 523, 250);
    }

    //Lose a life if bottom edge hit
    if (yb >= 64)
    {
      display.drawRect(xPaddle, 63, 11, 1, 0);
      xPaddle = 54;
      yb=60; 
      released = false;
      lives--;
      drawLives();
      tone(A2, 175, 250);
      if (random(0, 2) == 0)
      {
        dx = 1;
      } 
      else
      {
        dx = -1;
      }
    }

    //Bounce off left side
    if (xb <= 0)
    {
      xb = 2;
      dx = -dx;
      tone(A2, 523, 250);
    }

    //Bounce off right side
    if (xb >= width - 2)
    {
      xb = width - 4;
      dx = -dx;
      tone(A2, 523, 250);
    }

    //Bounce off paddle
    if (xb+1>=xPaddle && xb<=xPaddle+12 && yb+2>=63 && yb<=64)
    {
      dy = -dy;
      dx = ((xb-(xPaddle+6))/3); //Applies spin on the ball
      tone(A2, 200, 250);
    }

    //Bounce off Bricks
    for (byte row = 0; row < 4; row++)
    {
      for (byte column = 0; column < 14; column++)
      {
        if (!isHit[row][column])
        {
          //Sets Brick bounds
          leftBrick = 10 * column;
          rightBrick = 10 * column + 10;
          topBrick = 6 * row + 1;
          bottomBrick = 6 * row + 7;

          //If A collison has occured
          if (topBall <= bottomBrick && bottomBall >= topBrick &&
              leftBall <= rightBrick && rightBall >= leftBrick)
          {
            Score();
            brickCount++;
            isHit[row][column] = true;
            display.drawRect(10*column, 2+6*row, 8, 4, 0);

            //Vertical collision
            if (bottomBall > bottomBrick || topBall < topBrick)
            {
              //Only bounce once each ball move
              if(!bounced)
              {
                dy =- dy;
                yb += dy;
                bounced = true;
                tone(A2, 261, 250);
              } 
            }

            //Hoizontal collision
            if (leftBall < leftBrick || rightBall > rightBrick)
            {
              //Only bounce once brick each ball move
              if(!bounced)
              {
                dx =- dx;
                xb += dx;
                bounced = true;
                tone(A2, 261, 250);
              } 
            }
          }
        }
      }
    }
    //Reset Bounce
    bounced = false;
  }
  else
  {
    //Ball follows paddle
    xb=xPaddle + 5;

    //Release ball if FIRE pressed
    pad3 = !digitalRead(A0);
    if (pad3 == 1 && oldpad3 == 0)
    {  
      released=true;

      //Apply random direction to ball on release
      if (random(0, 2) == 0)
      {
        dx = 1;
      } 
      else
      {
        dx = -1;
      }
      //Makes sure the ball heads upwards 
      dy = -1;
    }
    oldpad3 = pad3; 
  }
}

void drawBall()
{
  display.drawPixel(xb,   yb,   0);
  display.drawPixel(xb+1, yb,   0);
  display.drawPixel(xb,   yb+1, 0);
  display.drawPixel(xb+1, yb+1, 0);

  if(ballclock>4)
  {
    moveBall();
    ballclock=0;
  }

  ballclock++;

  display.drawPixel(xb,   yb,   1);
  display.drawPixel(xb+1, yb,   1);
  display.drawPixel(xb,   yb+1, 1);
  display.drawPixel(xb+1, yb+1, 1);
}

void drawPaddle()
{
  display.drawRect(xPaddle, 63, 11, 1, 0);
  movePaddle();
  display.drawRect(xPaddle, 63, 11, 1, 1);
}

void drawLives()
{
  sprintf(text, "LIVES:%u", lives);
  display.setCursor(0, 90);
  display.print(text);
}

void drawGameOver()
{
  display.drawPixel(xb,   yb,   0);
  display.drawPixel(xb+1, yb,   0);
  display.drawPixel(xb,   yb+1, 0);
  display.drawPixel(xb+1, yb+1, 0);
  display.setCursor(52, 42);
  display.print( "Game");
  display.setCursor(52, 54);
  display.print("Over");
  display.display();
  delay(4000);
}

void pause()
{
  paused = true;
  //Draw pause to the screen
  display.setCursor(52, 45);
  display.print("PAUSE");
  display.display();
  while (paused)
  {
    delay(150);
    //Unpause if FIRE is pressed
    pad2 = !digitalRead(A0);
    if (pad2 > 1 && oldpad2 == 0 && released)
    {
        display.fillRect(52, 45, 30, 11, 0);

        paused=false;
    }
    oldpad2=pad2; 
  }
}

void Score()
{
  score += (level*10);
  sprintf(text, "SCORE:%u", score);
  display.setCursor(80, 90);
  display.print(text); 
}

void newLevel(){
  //Undraw paddle
  display.drawRect(xPaddle, 63, 11, 1, 0);

  //Undraw ball
  display.drawPixel(xb,   yb,   0);
  display.drawPixel(xb+1, yb,   0);
  display.drawPixel(xb,   yb+1, 0);
  display.drawPixel(xb+1, yb+1, 0);

  //Alter various variables to reset the game
  xPaddle = 54;
  yb = 60;
  brickCount = 0;
  released = false;

  //Draws new bricks and resets their values
  for (byte row = 0; row < 4; row++) {
    for (byte column = 0; column < 13; column++)
    {
      isHit[row][column] = false;
      display.drawRect(10*column, 2+6*row, 8, 4, 1);
    }
  }
  
  //Draws the initial lives
  drawLives();
  
  //Draws the initial score
  sprintf(text, "SCORE:%u", score);
  display.setCursor(80, 90);
  display.print(text); 
}

//Used to delay images while reading button input
boolean pollFireButton(int n)
{
  for(int i = 0; i < n; i++)
  {
    delay(15);
    pad = !digitalRead(A0);
    if(pad == 1 && oldpad == 0)
    {
      oldpad3 = 1; //Forces pad loop 3 to run once
      return true;
    }
    oldpad = pad;
  }
  return false;
}

//Function by nootropic design to display highscores
boolean displayHighScores(byte file)
{
  byte y = 10;
  byte x = 24;
  // Each block of EEPROM has 10 high scores, and each high score entry
  // is 5 bytes long:  3 bytes for initials and two bytes for score.
  int address = file*10*5;
  byte hi, lo;
  display.clearDisplay();
  display.setCursor(32, 0);
  display.print("HIGH SCORES");
  display.display();

  for(int i = 0; i < 10; i++)
  {
    sprintf(text, "%2d", i+1);
    display.setCursor(x,y+(i*8));
    display.print( text);
    display.display();
    hi = EEPROM.read(address + (5*i));
    lo = EEPROM.read(address + (5*i) + 1);

    if ((hi == 0xFF) && (lo == 0xFF))
    {
      score = 0;
    }
    else
    {
      score = (hi << 8) | lo;
    }

    initials[0] = (char)EEPROM.read(address + (5*i) + 2);
    initials[1] = (char)EEPROM.read(address + (5*i) + 3);
    initials[2] = (char)EEPROM.read(address + (5*i) + 4);

    if (score > 0)
    {
      sprintf(text, "%c%c%c %u", initials[0], initials[1], initials[2], score);
      display.setCursor(x + 24, y + (i*8));
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
}

boolean titleScreen()
{
  //Clears the screen
  display.clearDisplay();
  display.setCursor(16,22);
  display.setTextSize(2);
  display.print("ARAKNOID");
  display.setTextSize(1);
  display.display();
  if (pollFireButton(25))
  {
    return true;
  }

  //Flash "Press FIRE" 5 times
  for(byte i = 0; i < 5; i++)
  {
    //Draws "Press FIRE"
    //display.bitmap(31, 53, fire);  display.display();
    display.setCursor(31, 53);
    display.print("PRESS FIRE!");
    display.display();

    if (pollFireButton(50))
    {
      return true;
    }
    //Removes "Press FIRE"
    display.clearDisplay();
    display.setCursor(16,22);
    display.setTextSize(2);
    display.print("ARAKNOID");
    display.setTextSize(1);
    display.display();

    display.display();
    if (pollFireButton(25))
    {
      return true;
    }
  }

  return false;
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

    display.setCursor(16,0);
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
    for(byte i = 0; i < 3; i++)
    {
      display.drawLine(56 + (i*8), 27, 56 + (i*8) + 6, 27, 1);
    }
    display.drawLine(56, 28, 88, 28, 0);
    display.drawLine(56 + (index*8), 28, 56 + (index*8) + 6, 28, 1);
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

  // High score processing
  for(byte i = 0; i < 10; i++)
  {
    hi = EEPROM.read(address + (5*i));
    lo = EEPROM.read(address + (5*i) + 1);
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
      for(byte j=i;j<10;j++)
      {
        hi = EEPROM.read(address + (5*j));
        lo = EEPROM.read(address + (5*j) + 1);

        if ((hi == 0xFF) && (lo == 0xFF))
        {
        tmpScore = 0;
        } 
        else
        {
          tmpScore = (hi << 8) | lo;
        }

        tmpInitials[0] = (char)EEPROM.read(address + (5*j) + 2);
        tmpInitials[1] = (char)EEPROM.read(address + (5*j) + 3);
        tmpInitials[2] = (char)EEPROM.read(address + (5*j) + 4);

        // write score and initials to current slot
        EEPROM.write(address + (5*j), ((score >> 8) & 0xFF));
        EEPROM.write(address + (5*j) + 1, (score & 0xFF));
        EEPROM.write(address + (5*j) + 2, initials[0]);
        EEPROM.write(address + (5*j) + 3, initials[1]);
        EEPROM.write(address + (5*j) + 4, initials[2]);

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



void loop() 
{
  display.display();

  //Title screen loop switches from title screen
  //and high scores until FIRE is pressed
  while (!start)
  {
    start = titleScreen();
    if (!start)
    {
      start = displayHighScores(2);
    }
  }

  //Initial level draw
  if (!initialDraw)
  {
    //Clears the screen 
    display.display();
    display.clearDisplay();
    //Selects Font
    //Draws the new level
    newLevel();
    initialDraw=true;
  }

  if (lives>0)
  {
    drawPaddle();

    //Pause game if FIRE pressed
    pad = !digitalRead(A0);

    if(pad >1 && oldpad==0 && released)
    {
      oldpad2=0; //Forces pad loop 2 to run once
      pause();
    }

    oldpad=pad;
    drawBall();

    if(brickCount==60)
    {
      level++;
      newLevel(); 
    }
  }
  else
  {
    drawGameOver();
    if (score > 0) 
    {
      enterHighScore(2);
    }

    display.clearDisplay();
    initialDraw=false;
    start=false;
    lives=3;
    score=0;
    newLevel();
  }
}


