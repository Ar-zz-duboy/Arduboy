/*
 SHADOW RUNNER: http://www.team-arg.org/SHRUN-manual.html

 Arduboy version 1.1 : http://www.team-arg.org/SHRUN-downloads.html

 MADE by TEAM a.r.g. : http://www.team-arg.org/About.html

 2015 - JO3RI - Trodoss

 Special thanks to Dreamer3 for helping with the conversion

 SimpleButtons: Dreamer3 https://github.com/yyyc514/ArduboyExtra
 ArduboyLib: Dreamer3 https://github.com/yyyc514/ArduboyLib

 */

//determine the game
#define GAME_ID 15

#include <SPI.h>
#include <EEPROM.h>
#include "Arglib.h"
#include "otherbitmaps.h"
#include "candlebitmaps.h"
#include "numberbitmaps.h"
#include "runnerbitmaps.h"
#include "backgroundbitmaps.h"
#include "fencebitmap.h"
#include "forgroundbitmaps.h"
#include "itembitmaps.h"

//define menu states (on main menu)
#define STATE_MENU_MAIN    0
#define STATE_MENU_HELP    1
#define STATE_MENU_CREDITS 2
#define STATE_MENU_SOUND   3

#define GAME_INIT    4
#define GAME_PLAYING 5
#define GAME_PAUSED  6
#define GAME_OVER    7

#define RUNNER_RUNNING 0
#define RUNNER_JUMPING 8
#define RUNNER_DUCKING 9

Arduboy arduboy;
SimpleButtons buttons (arduboy);

//determines the state of the game
unsigned char game_state = 0;

// These are all getting a value in STATE_GAME_INIT
int life;
unsigned long score;
unsigned long previousScore;
byte level;

//generic variable to store selection (on screens)
unsigned char selection = 0;
boolean soundyesno = false;

//these are used for the runner animation in the mainscreen
int runnerX = -127;
int runnerY = 0;
boolean show_runner = false;
byte runnerframe = 0;
bool jumping = false;
bool ducking = false;
byte leap[] = {19, 13, 8, 6, 8, 13, 19};

byte showitems =  B00000000;        // this byte holds all the items the player runs into during the game
//                   | ||||
//                   | |||└->  stone1
//                   | ||└-->  stone2
//                   | |└--->  bird1
//                   | └---->  bird2
//                   |
//                   └------>  extra life


byte birdframe = 0;
byte flameid = 0;
boolean heartframe = 0;
int background1step = 0;
int background2step = 128;
byte background1id = 0;
byte background2id = 1;
int fence1step = 0;
int fence2step = 128;
byte fence1id = 0;
byte fence2id = 1;
int forgroundstep = 128;
byte forgroundid = 0;


int itemX[6] = { -64, 96, 48, 128, 128};

void setup () {
  arduboy.start();
  arduboy.setFrameRate(60);
  arduboy.drawBitmap(15, 16, TEAMarg, 96, 32, 1);
  arduboy.display();
  delay(3000);
  arduboy.clearDisplay();
  game_state = STATE_MENU_MAIN;
  if (EEPROM.read(EEPROM_AUDIO_ON_OFF)) soundyesno = true;
}

void loop() {
  if (!(arduboy.nextFrame())) return;
  buttons.poll();
  if (soundyesno == true) arduboy.audio.on();
  else arduboy.audio.off();
  switch (game_state)
  {
    case STATE_MENU_MAIN:
      if (arduboy.everyXFrames(4))
      {
        // show the splash art
        arduboy.fillRect(0, 0, 128, 30, 0);
        arduboy.drawBitmap(16, 0, shadowRunner_bitmap, 94, 24, 1);
        arduboy.drawBitmap(49, 26, menu_bitmap, 32, 32, 1);
        arduboy.fillRect(89, 42, 16, 8, 0);
        switch (soundyesno) {
          case true:
            arduboy.drawBitmap(89, 42, yes_bitmap, 16, 8, 1);
            break;
          case false:
            arduboy.drawBitmap(89, 42, no_bitmap, 14, 8, 1);
            break;
        }
        arduboy.fillRect(8, 18, 38, 46, 0);
        candle_draw(29, selection * 8);
        for (byte i = 0; i < 4; i++)
        {
          if (selection != i) {
            arduboy.drawBitmap(49, (i * 8) + 26, shade_bitmap, 32, 8, 0);
            arduboy.drawBitmap(82, (i * 8) + 26, shade_bitmap, 32, 8, 0);
          }
        }

        if (show_runner)
        {
          arduboy.fillCircle(runnerX + 15, 14, 14, 1);
          runner_draw();
        }
        runnerY = 0;
        runnerX += 4;
        if (runnerX > 127)
        {
          runnerX = -127;
          show_runner = !show_runner;
        }
        arduboy.display();
      }
      if (buttons.justPressed(UP_BUTTON))
      {
        if (selection > 0) selection--;
      }
      if (buttons.justPressed(DOWN_BUTTON))
      {
        if (selection < 3) selection++;
      }
      if (buttons.justPressed(A_BUTTON | B_BUTTON | LEFT_BUTTON | RIGHT_BUTTON))
      {
        switch (selection)
        {
          case 0:
            arduboy.clearDisplay();
            arduboy.drawBitmap(31, 0, QR_bitmap, 64, 64, 1);
            arduboy.display();
            game_state = STATE_MENU_HELP;
            break;
          case 1:
            arduboy.clearDisplay();
            arduboy.fillRect(0, 0, 128, 64, 0);
            arduboy.drawBitmap(16, 0, shadowRunner_bitmap, 94, 24, 1);
            arduboy.drawBitmap(15, 23, info_bitmap, 96, 40, 1);
            arduboy.display();
            game_state = STATE_MENU_CREDITS;
            break;
          case 2:
            soundyesno = !soundyesno;
            if (soundyesno == true) arduboy.audio.on();
            else arduboy.audio.off();
            arduboy.audio.save_on_off();
            break;
          case 3:
            arduboy.clearDisplay();
            arduboy.display();
            game_state = GAME_INIT;
            break;
        }
      }
      break;
    case STATE_MENU_HELP:
    case STATE_MENU_CREDITS:
      {
        if (buttons.justPressed(A_BUTTON | B_BUTTON | LEFT_BUTTON | RIGHT_BUTTON))
        {
          arduboy.clearDisplay();
          arduboy.display();
          game_state = STATE_MENU_MAIN;
        }
      }
      break;

    //******* Game-specific events **************
    case GAME_OVER:
      arduboy.fillRect(0, 0, 128, 64, 0);
      arduboy.drawBitmap(31, 16, gameover_bitmap, 64, 24, 1);
      score_draw(28, 40);

      if (buttons.justPressed(A_BUTTON | B_BUTTON | LEFT_BUTTON | RIGHT_BUTTON | UP_BUTTON | DOWN_BUTTON))
      {
        arduboy.clearDisplay();
        game_state = STATE_MENU_MAIN;
      }
      arduboy.display();
      break;
    case GAME_INIT:
      runnerX = 0;
      runnerY = 28;
      score = 0;
      previousScore = 1000;
      life = 128;
      level = 0;
      showitems = B00000000;
      game_state = GAME_PLAYING;
      break;
    case GAME_PLAYING:
      //draw the background
      //------------------
      if (arduboy.everyXFrames(3))
      {
        background1step -= 1;
        background2step -= 1;
        arduboy.clearDisplay();
      }
      if (background1step < -127) 
      {
        background1step = 128;
        background1id = random(0, 2);
      }
      if (background2step < -127)
      {
        background2step = 128;
        background2id = random(1, 5);
      }
      arduboy.drawBitmap(background1step, 0, background_bitmaps[background1id], 128, 52, 1);
      arduboy.drawBitmap(background2step, 0, background_bitmaps[background2id], 128, 52, 1);
      
      // draw the fence
      //---------------
      if (arduboy.everyXFrames(1))
      {
        fence1step -= 1;
        fence2step -= 1;
      }
      arduboy.drawBitmap(fence1step, 36, fence_bitmaps[fence1id], 128, 16, 0);
      arduboy.drawBitmap(fence2step, 36, fence_bitmaps[fence2id], 128, 16, 0);
      if (fence1step < -127) 
      {
        fence1step = 128;
        //fence1id = random(0, 3);
      }
      if (fence2step < -127)
      {
        fence2step = 128;
        //fence2id = random(2, 5);
      }
      

      // draw the items
      //---------------

      if (arduboy.everyXFrames(1)) 
      {
        itemX[0] -= 2;
        if (itemX[0] < -127)
        {
          switch (random(0, 2))  //stone 1
          {
            case 0:
              showitems ^= B00000001;
              break;
            case 1:
              showitems |= B00000001;
              break;
          }
          itemX[0] = 128;
        }

        itemX[1] -= 2;
        if (itemX[1] < -127)
        {
          switch (random(0, 2))  //stone 2
          {
            case 0:
              showitems ^= B00000010;
              break;
            case 1:
              showitems |= B00000010;
              break;
          }
          itemX[1] = 128;
        }

        itemX[2] -= 2;
        if (itemX[2] < -127)
        {
          switch (random(0, 2))  //bird1
          {
            case 0:
              showitems ^= B00000100;
              break;
            case 1:
              showitems |= B00000100;
              break;
          }
          itemX[2] = 128;
        }

        itemX[3] -= 2;
        if (itemX[3] < -127)
        {
          switch (random(0, 2))  //bird2
          {
            case 0:
              showitems ^= B00001000;
              break;
            case 1:
              showitems |= B00001000;
              break;
          }
          if ((showitems & B00000010) == B00000010)
          {
            if ((showitems & B00001000) == B00001000) showitems ^= B00001000; // if stone2 no Bird2
          }
          itemX[3] = 128;
        }
      }


      if ((showitems & B00000001) == B00000001)
      {
        arduboy.drawBitmap(itemX[0], 36, stone_bitmap, 16, 16, 0);
        if(collide(runnerY, runnerY + 24, itemX[0] + 3, 38, itemX[0] + 13, 52) == true)
        {
          life -= 4;
          arduboy.tunes.tone(175, 100);
        }
        
      }

      if ((showitems & B00000010) == B00000010)
      {
        arduboy.drawBitmap(itemX[1], 36, stone_bitmap, 16, 16, 0);
        if(collide(runnerY, runnerY + 24, itemX[1] + 3, 38, itemX[1] + 13, 52) == true)
        {
          life -= 4;
          arduboy.tunes.tone(175, 100);
        }
      }

      if ((showitems & B00000100) == B00000100)
      {
        switch (birdframe)
        {
          case 0:
            arduboy.drawBitmap(itemX[2], 16, bird1_bitmap, 16, 16, 0);
            break;
          case 1:
            arduboy.drawBitmap(itemX[2], 21, bird2_bitmap, 16, 8, 0);
            break;
          case 2:
            arduboy.drawBitmap(itemX[2], 23, bird3_bitmap, 16, 8, 0);
            break;
          case 3:
            arduboy.drawBitmap(itemX[2], 23, bird4_bitmap, 16, 16, 0);
            break;
          case 4:
            arduboy.drawBitmap(itemX[2], 22, bird5_bitmap, 16, 16, 0);
            break;
          case 5:
            arduboy.drawBitmap(itemX[2], 23, bird6_bitmap, 16, 16, 0);
            break;
          case 6:
            arduboy.drawBitmap(itemX[2], 23, bird7_bitmap, 16, 8, 0);
            break;
          case 7:
            arduboy.drawBitmap(itemX[2], 18, bird8_bitmap, 16, 8, 0);
            break;
        }
        if (collide(runnerY, runnerY + 24, itemX[2] + 2, 16, itemX[2] + 16, 32) == true) 
        {
          life -= 2;
          arduboy.tunes.tone(523, 50);
        }
      }

      if ((showitems & B00001000) == B00001000)
      {
        switch (birdframe)
        {
          case 0:
            arduboy.drawBitmap(itemX[3], 16, bird1_bitmap, 16, 16, 0);
            break;
          case 1:
            arduboy.drawBitmap(itemX[3], 21, bird2_bitmap, 16, 8, 0);
            break;
          case 2:
            arduboy.drawBitmap(itemX[3], 23, bird3_bitmap, 16, 8, 0);
            break;
          case 3:
            arduboy.drawBitmap(itemX[3], 23, bird4_bitmap, 16, 16, 0);
            break;
          case 4:
            arduboy.drawBitmap(itemX[3], 22, bird5_bitmap, 16, 16, 0);
            break;
          case 5:
            arduboy.drawBitmap(itemX[3], 23, bird6_bitmap, 16, 16, 0);
            break;
          case 6:
            arduboy.drawBitmap(itemX[3], 23, bird7_bitmap, 16, 8, 0);
            break;
          case 7:
            arduboy.drawBitmap(itemX[3], 18, bird8_bitmap, 16, 8, 0);
            break;
        }
        if(collide(runnerY, runnerY + 24, itemX[3] + 2, 4, itemX[3] + 16, 20) == true)
        {
          life -= 2;
          arduboy.tunes.tone(523, 50);
        }
      }

      if ((showitems & B00100000) == B00100000)
      {
        if (arduboy.everyXFrames(1))
        {
          itemX[4] -= 2;
          if (itemX[4] < -24)
          {
            if ((showitems & B00100000) == B00100000) showitems ^= B00100000;
            itemX[4] = 128;
          }
        }
        switch (heartframe)
        {
          case 0:
            arduboy.drawBitmap(itemX[4], 4, heart1_bitmap, 16, 16, 0);
            break;
          case 1:
            arduboy.drawBitmap(itemX[4], 4, heart2_bitmap, 16, 16, 0);
            break;
        }
        if (collide(runnerY, runnerY + 24, itemX[4] + 2, 4, itemX[4] + 12, 20) == true)
        {
          showitems ^= B00100000;
          itemX[4] = 128;
          life = 128;
          score += 500;
          arduboy.tunes.tone(750, 200);
        }
      }


      if (arduboy.everyXFrames(6))
      {
        birdframe++;
        heartframe = !heartframe;
      }
      if (birdframe > 7) birdframe = 0;

      //draw the runner
      //---------------
      runner_draw();

      //draw the forground trees
      //------------------------
      if (forgroundstep == 128) forgroundid = random(0, 3);
      arduboy.drawBitmap(forgroundstep, -4, forground_bitmaps[forgroundid], 56, 56, 0);

      if (arduboy.everyXFrames(2))
      {
        forgroundstep -= 4;
      }
      if (forgroundstep < -255) forgroundstep = 128;



      //draw score and lives on the screen
      if (arduboy.everyXFrames(16 - 2 * level))
      {
        life--;
      }
      arduboy.drawBitmap(2, 53, life_bitmap, 24, 6, 1);
      arduboy.fillRect(0, 60, 128, 66, 1);
      arduboy.fillRect(0, 61, life, 2, 0);
      if (life < 64)
      {
        showitems = showitems | B00100000;
      }
      score_draw(57, 52);

      //level_update();
      if (previousScore < score)
      {
        level += 1;
        previousScore += 1000;
        if (level > 7) level = 7;
      }
      score++;

      if (buttons.justPressed(UP_BUTTON | DOWN_BUTTON | RIGHT_BUTTON)) game_state = GAME_PAUSED;
      if (buttons.justPressed(B_BUTTON))
      {
        if (!jumping)
        {
          jumping = true;
          runnerframe = RUNNER_JUMPING;
        }
      }
      if (buttons.justPressed(A_BUTTON | LEFT_BUTTON))
      {
        if (!ducking && !jumping)
        {
          ducking = true;
          runnerframe = RUNNER_DUCKING;
        }
      }

      //check the runner's state
      if (life < 0)
      {
        game_state = GAME_OVER;
        delay(1000L);
      }
      arduboy.display();
      break;

    case GAME_PAUSED:
      arduboy.fillRect(0, 0, 128, 64, 0);
      arduboy.drawBitmap(35, 4, pause_bitmap, 56, 16, 1);
      candle_draw(56, 8);
      arduboy.display();
      if (buttons.justPressed(A_BUTTON | B_BUTTON | UP_BUTTON | DOWN_BUTTON | LEFT_BUTTON | RIGHT_BUTTON))
      {
        game_state = GAME_PLAYING;
        arduboy.fillRect(0, 0, 128, 64, 0);
      }
      break;
  }
}


//******************draw-specific  functions
void candle_draw (char x, char y)
{
  if (arduboy.everyXFrames(2))
  {
    flameid = random(0, 24);
  }
  arduboy.drawBitmap(x, y + 34, candletip_bitmap, 16, 32, 1);
  arduboy.drawBitmap(x + 4, y + 18, flames[flameid], 8, 16, 1);
}

void score_draw(int scoreX, int scoreY)
{
  arduboy.drawBitmap(scoreX, scoreY, score_bitmap, 32, 8, 1);
  char buf[8];
  itoa(score, buf, 8);
  char charLen = strlen(buf);
  char pad = 8 - charLen;

  //draw 0 padding
  for (byte i = 0; i < pad; i++)
  {
    arduboy.drawBitmap(scoreX + 30 + (5 * i), scoreY, number_bitmaps[0], 8, 5, 1);
  }

  for (byte i = 0; i < charLen; i++)
  {
    char digit = buf[i];
    byte j;
    if (digit <= 48)
    {
      digit = 0;
    }
    else {
      digit -= 48;
      if (digit > 9) digit = 0;
    }

    for (byte z = 0; z < 10; z++)
    {
      if (digit == z) j = z;
    }
    arduboy.drawBitmap(scoreX + 30 + (pad * 5) + (5 * i), scoreY, number_bitmaps[digit], 8, 5, 1);
  }
}

//******************  game elements

void runner_draw()
{
  if (arduboy.everyXFrames(4))
  {
    runnerframe++;
  }
  if (jumping)
  {
    ducking = false;
    runnerY = leap[runnerframe - RUNNER_JUMPING];
    if (runnerframe > 14)
    {
      runnerY = 28;
      jumping = false;
      runnerframe = RUNNER_RUNNING;
    }
    arduboy.drawBitmap(runnerX + 14, runnerY + 6, eyesmask_bitmap, 8, 2, 1);
    arduboy.drawBitmap(runnerX, runnerY, runner_bitmap09, 32, 24, 0);
  }
  else if (ducking)
  {
    jumping = false;
    runnerY = 36;
    if (runnerframe > 14)
    {
      runnerY = 28;
      ducking = false;
      runnerframe = RUNNER_RUNNING;
    }
    arduboy.drawBitmap(runnerX + 12, runnerY + 7, eyesmask_bitmap, 8, 2, 1);
    arduboy.drawBitmap(runnerX, runnerY, runner_bitmap10, 32, 24, 0);
  }
  else {
    if (runnerframe > 7)runnerframe = RUNNER_RUNNING;
    switch (runnerframe)
    {
      case 0:
        arduboy.drawBitmap(runnerX + 16, runnerY + 7, eyesmask_bitmap, 8, 2, 1);
        arduboy.drawBitmap(runnerX, runnerY, runner_bitmap01, 32, 24, 0);
        break;
      case 1:
        arduboy.drawBitmap(runnerX + 16, runnerY + 8, eyesmask_bitmap, 8, 2, 1);
        arduboy.drawBitmap(runnerX, runnerY, runner_bitmap02, 32, 24, 0);
        break;
      case 2:
        arduboy.drawBitmap(runnerX + 16, runnerY + 9, eyesmask_bitmap, 8, 2, 1);
        arduboy.drawBitmap(runnerX, runnerY, runner_bitmap03, 32, 24, 0);
        break;
      case 3:
        arduboy.drawBitmap(runnerX + 16, runnerY + 8, eyesmask_bitmap, 8, 2, 1);
        arduboy.drawBitmap(runnerX, runnerY, runner_bitmap04, 32, 24, 0);
        break;
      case 4:
        arduboy.drawBitmap(runnerX + 17, runnerY + 7, eyesmask_bitmap, 8, 2, 1);
        arduboy.drawBitmap(runnerX, runnerY, runner_bitmap05, 32, 24, 0);
        break;
      case 5:
        arduboy.drawBitmap(runnerX + 17, runnerY + 8, eyesmask_bitmap, 8, 2, 1);
        arduboy.drawBitmap(runnerX, runnerY, runner_bitmap06, 32, 24, 0);
        break;
      case 6:
        arduboy.drawBitmap(runnerX + 22, runnerY + 9, eyesmask_bitmap, 8, 2, 1);
        arduboy.drawBitmap(runnerX, runnerY, runner_bitmap07, 32, 24, 0);
        break;
      case 7:
        arduboy.drawBitmap(runnerX + 20, runnerY + 8, eyesmask_bitmap, 8, 2, 1);
        arduboy.drawBitmap(runnerX, runnerY, runner_bitmap08, 32, 24, 0);
        break;
    }
  }
}

boolean collide(int y1, int y2, int xx1, int yy1, int xx2, int yy2)
{
  if (35 <= xx1) return false;
  else if (23 >= xx2) return false;
  else if (y2 <= yy1) return false;
  else if (y1 >= yy2) return false;
  else return true;
}
