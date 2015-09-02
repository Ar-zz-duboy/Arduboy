/*
 Blob Attack: http://www.team-arg.org/BLBA-manual.html

 Arduboy version 1.0:  http://www.team-arg.org/BLBA-downloads.html

 MADE by TEAM a.r.g. : http://www.team-arg.org/About.html

 2015 - JO3RI
 */

//determine the game
#define GAME_ID 28

#include <SPI.h>
#include <EEPROM.h>
#include "Arglib.h"
#include "menu_bitmap.h"
#include "blobs_bitmap.h"
#include "numberbitmaps.h"

//define menu states (on main menu)
#define STATE_MENU_MAIN          0
#define STATE_MENU_HELP          1
#define STATE_MENU_INFO          3
#define STATE_MENU_SOUNDFX       4

//define game states (on main menu)
#define STATE_GAME_INIT          2
#define STATE_GAME_PLAYING       8
#define STATE_GAME_PAUSE         9
#define STATE_GAME_OVER          10

#define PLAYFIELD_WIDTH          8         // Playfield width in blobs
#define PLAYFIELD_HEIGHT         10        // Playfield height in blobs
#define PLAYFIELD_ZERO_X         2         // zero x-position of the playfield in pixel position on the screen
#define PLAYFIELD_ZERO_Y         3         // zero x-position of the playfield in pixel position on the screen
#define BLOB_FREE                0         // an empty place in the playfield

#define BLOB_PIXELS              6
#define TILES_IN_BLOBS           3      // number of vertical or horizontal tiles in blobsgrid

#define BLOB_CURRENT             0
#define BLOB_NEXT                2
#define BLOB_WAITING             4

#define NO_FLAG_ON_FIELD         0
#define FLAG_ON_FIELD            1

Arduboy arduboy;
SimpleButtons buttons (arduboy);

unsigned char game_state = STATE_MENU_MAIN;
boolean soundyesno;
boolean giveExtraScore;
boolean canMoveBlobsDown;

int menucount = 1;

unsigned long scorePlayer;
unsigned long extraScoreForChain;
int level;

int Current_Blobs[] =
{
  // array for current blob
  0, 0, 0,
  0, 0, 0,
  0, 0, 0,
};

int Field[PLAYFIELD_WIDTH][PLAYFIELD_HEIGHT];
int FieldFlags[PLAYFIELD_WIDTH][PLAYFIELD_HEIGHT];

int RandomBlobPit[6];
int BlobNumbers;

int BlobsXY[2];          // X and Y coordinates for the small 3x3 blob grid


void setup()
{
  arduboy.start();
  arduboy.setFrameRate(60);
  arduboy.drawBitmap(15, 16, TEAMarg, 96, 32, 1);
  arduboy.display();
  delay(3000);
  game_state = STATE_MENU_MAIN;
  if (EEPROM.read(EEPROM_AUDIO_ON_OFF)) soundyesno = true;
  arduboy.initRandomSeed();
}



void loop()
{
  if (!(arduboy.nextFrame())) return;
  buttons.poll();
  if (soundyesno == true) arduboy.audio.on();
  else arduboy.audio.off();
  switch (game_state)
  {
    case STATE_MENU_MAIN:
      // show the splash art
      arduboy.clearDisplay();
      arduboy.drawBitmap(0, 0, splashscreen_bitmap, 128, 64, 1);
      arduboy.drawBitmap(2, 56, mainmenu_bitmap, 90, 8, 0);
      arduboy.drawBitmap(2, 54, menumask_bitmap, 90, 2, 1);
      arduboy.drawBitmap(5 + (23 * (menucount - 1)), 54, selector_bitmap, 16, 1, 0);
      if (buttons.justPressed(RIGHT_BUTTON) && (menucount < 4))menucount++;
      if (buttons.justPressed(LEFT_BUTTON) && (menucount > 1))menucount--;
      if (buttons.justPressed(A_BUTTON | B_BUTTON)) game_state = menucount;
      //arduboy.display();
      break;
    case STATE_MENU_HELP: // QR code
      arduboy.clearDisplay();
      arduboy.drawBitmap(32, 0, qrcode_bitmap, 64, 64, 1);
      if (buttons.justPressed(A_BUTTON | B_BUTTON)) game_state = STATE_MENU_MAIN;
      //arduboy.display();
      break;
    case STATE_MENU_INFO: // infoscreen
      arduboy.clearDisplay();
      arduboy.drawBitmap(32, 20, info_bitmap, 64, 24, 1);
      if (buttons.justPressed(A_BUTTON | B_BUTTON)) game_state = STATE_MENU_MAIN;
      //arduboy.display();
      break;
    case STATE_MENU_SOUNDFX: // soundconfig screen
      arduboy.clearDisplay();
      arduboy.drawBitmap(0, 0, splashscreen_bitmap, 128, 64, 1);
      arduboy.drawBitmap(2, 56, soundmenu_bitmap, 84, 8, 0);
      arduboy.drawBitmap(16 + (44 * soundyesno), 54, selector_bitmap, 16, 1, 0);
      if (buttons.justPressed(RIGHT_BUTTON)) soundyesno = true;
      if (buttons.justPressed(LEFT_BUTTON)) soundyesno = false;
      if (buttons.justPressed(A_BUTTON | B_BUTTON))
      {
        arduboy.audio.save_on_off();
        game_state = STATE_MENU_MAIN;
      }
      if (soundyesno == true) arduboy.audio.on();
      else arduboy.audio.off();
      //arduboy.display();
      break;
    case STATE_GAME_INIT:
      level = 0;
      scorePlayer = 0;
      BlobNumbers = 0;
      InitPlayfield();                        // let's clean up the playfield and start with fresh ones
      removeFlag();
      FillBlobPit();                          // fill the pit with random blobs.
      CreateCurrentBlobs();
      giveExtraScore = false;
      canMoveBlobsDown = true;
      arduboy.clearDisplay();
      arduboy.drawBitmap(0, 0, playfield_bitmap, 128, 64, 1);
      game_state = STATE_GAME_PLAYING;
      //arduboy.display();
      break;
    case STATE_GAME_PLAYING:
      if (arduboy.everyXFrames(90))DropBlobs();
      UpdateStage();
      if (buttons.justPressed(RIGHT_BUTTON)) MoveBlobsRight();
      if (buttons.justPressed(LEFT_BUTTON)) MoveBlobsLeft();
      if (buttons.justPressed(DOWN_BUTTON)) DropBlobs();
      if (buttons.justPressed(B_BUTTON)) RotateBlobsRight();
      if (buttons.justPressed(A_BUTTON)) RotateBlobsLeft();
      if (buttons.justPressed(UP_BUTTON))
      {
        pause();
        game_state = STATE_GAME_PAUSE;
      }
      //arduboy.display();
      break;
    case STATE_GAME_OVER:
      arduboy.clearDisplay();
      arduboy.drawBitmap(0, 0, gameOver_bitmap, 128, 64, 1);
      score_draw(83, 33);
      if (buttons.justPressed(A_BUTTON | B_BUTTON))
      {
        arduboy.clearDisplay();
        //arduboy.display();
        game_state = STATE_MENU_MAIN;
      }
      //arduboy.display();
      break;
    case STATE_GAME_PAUSE:
      if (buttons.justPressed(UP_BUTTON))
      {
        afterPause();
        game_state = STATE_GAME_PLAYING;
      }
      break;
  }
  arduboy.display();
}


// playfield
//-----------

void InitPlayfield()
{
  for (int x = 0; x < PLAYFIELD_WIDTH; x++)
  {
    for (int y = 0; y < PLAYFIELD_HEIGHT; y++)
    {
      Field[x][y] = BLOB_FREE;
    }
  }
}

void removeFlag()
{
  for (int x = 0; x < PLAYFIELD_WIDTH; x++)
  {
    for (int y = 0; y < PLAYFIELD_HEIGHT; y++)
    {
      FieldFlags[x][y] = NO_FLAG_ON_FIELD;
    }
  }
}

void StoreBlob(int array_x, int array_y)
{
  int draw_pointer = 0;
  for (int y = array_y; y < array_y + TILES_IN_BLOBS; y++)
  {
    for (int x = array_x; x < array_x + TILES_IN_BLOBS; x++)
    {
      if (Current_Blobs[draw_pointer] != 0) Field[x][y] = Current_Blobs[draw_pointer];
      draw_pointer++;
    }
  }
}

void StoreOneBlob(int array_x, int array_y)
{
  // if the blob is not on the floor
  if ((array_y) < PLAYFIELD_HEIGHT - 2)
  {

    for (int x = 0; x < TILES_IN_BLOBS; x++)
    {
      if ((!IsTileFree(array_x + x, array_y + 2)) && (!IsOnlyOneBlob()) && (Current_Blobs[3 + x] != 0))
      {
        Field [array_x + x][array_y + 1] = Current_Blobs[3 + x];
        Current_Blobs[3 + x] = 0;
      }
    }
  }
}

void DrawField()
{
  for (int y = 0; y < PLAYFIELD_HEIGHT; y++)
  {
    for (int x = 0 ; x < PLAYFIELD_WIDTH; x++)
    {
      // draw every tile in the playfield
      arduboy.drawBitmap((x * BLOB_PIXELS) + PLAYFIELD_ZERO_X, (y * BLOB_PIXELS) + PLAYFIELD_ZERO_Y, blobsinverted_bitmap[Field[x][y]], 6, 6, 0);
    }
  }
}

boolean IsTileFree(int array_x, int array_y)
{
  if (Field [array_x][array_y] == BLOB_FREE) return true;
  else return false;
}

boolean aboveIsSameBlob(int array_x, int array_y)
{
  if ((array_y - 1 > 0) && (Field [array_x][array_y] == Field [array_x][array_y - 1])) return true;
  else return false;
}

boolean underIsSameBlob(int array_x, int array_y)
{
  if ((array_y + 1 < PLAYFIELD_HEIGHT) && (Field [array_x][array_y] == Field [array_x][array_y + 1])) return true;
  else return false;
}

boolean rightIsSameBlob(int array_x, int array_y)
{
  if ((array_x + 1 < PLAYFIELD_WIDTH ) && (Field [array_x][array_y] == Field [array_x + 1][array_y])) return true;
  else return false;
}

boolean leftIsSameBlob(int array_x, int array_y)
{
  if ((array_x - 1 > 0 ) && (Field [array_x][array_y] == Field [array_x - 1][array_y])) return true;
  else return false;
}



boolean IsMovePossible (int array_x, int array_y)
{
  // checks collision with blocks already stored in the playfield
  // check if the 3x3 tiles of a blob with the correct area in the playfield provide by draw_x and draw_y
  int draw_pointer = 0;
  for (int y = array_y; y < array_y + TILES_IN_BLOBS; y++)
  {
    for (int x = array_x; x < array_x + TILES_IN_BLOBS; x++)
    {
      // check if the block is outside the limits of the playfield
      if (x < 0 || x > PLAYFIELD_WIDTH - 1 || y > PLAYFIELD_HEIGHT - 1)
      {
        byte temp = Current_Blobs[draw_pointer];
        if ( temp != 0) return false;
      }

      // check if the block has collided with tile already stored in the playfield array
      byte temp = Current_Blobs[draw_pointer];
      if ((temp != 0) && (IsTileFree(x, y) == false)) return false;
      draw_pointer++;
    }
  }
  return true;
}
boolean IsOneBlobDropPossible(int array_x, int array_y)
{
  // checks 1 blob collision with blobs already stored in the playfield
  if ((Current_Blobs[1] == 0) && (Current_Blobs[7] == 0))
  {
    for (byte temp = 3; temp < 6; temp++)
    {
      if ((Current_Blobs[temp] != 0) && (IsTileFree(array_x, array_y) == false)) return true;
    }
  }
  else return false;
}

boolean IsOnlyOneBlob()
{
  byte temp = 0;
  for (int i = 0; i < 9; i++)
  {
    if (Current_Blobs[i] != 0)temp++;
  }
  if (temp < 2) return true;
  else return false;
}

void DeletePossibleBlobs()
{
  extraScoreForChain = 0;
  while (canMoveBlobsDown)
  {
    fourInPack();
    fourInColumn();
    fourInRow();
    removeGroups();
    moveBlobsDown();
    delay(200);
  }
  canMoveBlobsDown = true;
}

void moveBlobsDown()
{
  canMoveBlobsDown = false;
  for (byte column = 0; column < PLAYFIELD_WIDTH; column++)
  {
    for (byte row = PLAYFIELD_HEIGHT - 1; row > 0; row--)
    {
      if (IsTileFree(column, row))
      {
        if (!IsTileFree(column, row - 1))
        {
          Field [column][row] = Field [column][row - 1];
          Field [column][row - 1] = BLOB_FREE;
          DrawField();
          arduboy.drawBitmap((column * BLOB_PIXELS) + PLAYFIELD_ZERO_X, (row * BLOB_PIXELS) + PLAYFIELD_ZERO_Y, blob00inverted_bitmap, 6, 6, 0);
          arduboy.drawBitmap((column * BLOB_PIXELS) + PLAYFIELD_ZERO_X, (row * BLOB_PIXELS) + PLAYFIELD_ZERO_Y, blobs_bitmap[Field [column][row]], 6, 6, 1);
          arduboy.display();
          canMoveBlobsDown = true;
        }
      }
    }
  }
}

void removeGroups()
{
  for (int x = 0; x < PLAYFIELD_WIDTH; x++)
  {
    for (int y = 0; y < PLAYFIELD_HEIGHT; y++)
    {
      if (FieldFlags[x][y] == FLAG_ON_FIELD)
      {
        giveExtraScore = true;
        Field[x][y] = BLOB_FREE;
        scorePlayer += 50;
      }
    }
  }
  if (giveExtraScore == true)
  {
    scorePlayer += extraScoreForChain;
    extraScoreForChain += 500;
    arduboy.tunes.tone(440, 100);
    delay(100);
    arduboy.tunes.tone(1047, 200);
  }
  giveExtraScore = false;
  removeFlag();
}


void fourInPack()
{
  for (byte column = 0; column < PLAYFIELD_WIDTH; column++)
  {
    for (byte row = PLAYFIELD_HEIGHT - 1; row > 0; row--)
    {
      if (!IsTileFree(column, row))
      {
        if (aboveIsSameBlob(column, row) && rightIsSameBlob(column, row) && aboveIsSameBlob(column + 1, row))
        {
          FieldFlags[column][row] = FLAG_ON_FIELD;
          FieldFlags[column][row - 1] = FLAG_ON_FIELD;
          FieldFlags[column + 1][row] = FLAG_ON_FIELD;
          FieldFlags[column + 1][row - 1] = FLAG_ON_FIELD;
        }
        if (rightIsSameBlob(column, row) && aboveIsSameBlob(column + 1, row) && rightIsSameBlob(column + 1, row - 1))
        {
          FieldFlags[column][row] = FLAG_ON_FIELD;
          FieldFlags[column + 1][row] = FLAG_ON_FIELD;
          FieldFlags[column + 1][row - 1] = FLAG_ON_FIELD;
          FieldFlags[column + 2][row - 1] = FLAG_ON_FIELD;
        }
        if (rightIsSameBlob(column, row) && underIsSameBlob(column + 1, row) && rightIsSameBlob(column + 1, row + 1))
        {
          FieldFlags[column][row] = FLAG_ON_FIELD;
          FieldFlags[column + 1][row] = FLAG_ON_FIELD;
          FieldFlags[column + 1][row + 1] = FLAG_ON_FIELD;
          FieldFlags[column + 2][row + 1] = FLAG_ON_FIELD;
        }
        if (aboveIsSameBlob(column, row) && rightIsSameBlob(column, row - 1) && aboveIsSameBlob(column + 1, row - 1))
        {
          FieldFlags[column][row] = FLAG_ON_FIELD;
          FieldFlags[column][row - 1] = FLAG_ON_FIELD;
          FieldFlags[column + 1][row - 1] = FLAG_ON_FIELD;
          FieldFlags[column + 1][row - 2] = FLAG_ON_FIELD;
        }
        if (aboveIsSameBlob(column, row) && leftIsSameBlob(column, row - 1) && aboveIsSameBlob(column - 1, row - 1))
        {
          FieldFlags[column][row] = FLAG_ON_FIELD;
          FieldFlags[column][row - 1] = FLAG_ON_FIELD;
          FieldFlags[column - 1][row - 1] = FLAG_ON_FIELD;
          FieldFlags[column - 1][row - 2] = FLAG_ON_FIELD;
        }
      }
    }
  }
}


void fourInColumn()
{
  for (byte column = 0; column < PLAYFIELD_WIDTH; column++)
  {
    for (byte row = PLAYFIELD_HEIGHT - 1; row > 0; row--)
    {
      if (!IsTileFree(column, row))
      {
        if (aboveIsSameBlob(column, row) && aboveIsSameBlob(column, row - 1))
        {
          if (aboveIsSameBlob(column, row - 2))
          {
            FieldFlags[column][row] = FLAG_ON_FIELD;
            FieldFlags[column][row - 1] = FLAG_ON_FIELD;
            FieldFlags[column][row - 2] = FLAG_ON_FIELD;
            FieldFlags[column][row - 3] = FLAG_ON_FIELD;
          }
          for (byte temp = 0; temp < 3; temp++)
          {
            if (rightIsSameBlob(column, row - temp))
            {
              FieldFlags[column][row] = FLAG_ON_FIELD;
              FieldFlags[column][row - 1] = FLAG_ON_FIELD;
              FieldFlags[column][row - 2] = FLAG_ON_FIELD;
              FieldFlags[column + 1][row - temp] = FLAG_ON_FIELD;
            }
          }
          for (byte temp = 0; temp < 3; temp++)
          {
            if (leftIsSameBlob(column, row - temp))
            {
              FieldFlags[column][row] = FLAG_ON_FIELD;
              FieldFlags[column][row - 1] = FLAG_ON_FIELD;
              FieldFlags[column][row - 2] = FLAG_ON_FIELD;
              FieldFlags[column - 1][row - temp] = FLAG_ON_FIELD;
            }
          }
        }
      }
    }
  }
}


void fourInRow()
{
  //check if 4 or more blobs are equal in the same row
  for (byte column = 0; column < PLAYFIELD_WIDTH; column++)
  {
    for (byte row = PLAYFIELD_HEIGHT - 1; row > 0; row--)
    {
      if (!IsTileFree(column, row))
      {
        if (rightIsSameBlob(column, row) && rightIsSameBlob(column + 1, row))
        {
          if (rightIsSameBlob(column + 2, row))
          {
            FieldFlags[column][row] = FLAG_ON_FIELD;
            FieldFlags[column + 1][row] = FLAG_ON_FIELD;
            FieldFlags[column + 2][row] = FLAG_ON_FIELD;
            FieldFlags[column + 3][row] = FLAG_ON_FIELD;
          }
          for (byte temp = 0; temp < 3; temp++)
          {
            if (aboveIsSameBlob(column + temp, row))
            {
              FieldFlags[column][row] = FLAG_ON_FIELD;
              FieldFlags[column + 1][row] = FLAG_ON_FIELD;
              FieldFlags[column + 2][row] = FLAG_ON_FIELD;
              FieldFlags[column + temp][row - 1] = FLAG_ON_FIELD;
            }
          }
          for (byte temp = 0; temp < 3; temp++)
          {
            if (underIsSameBlob(column + temp, row))
            {
              FieldFlags[column][row] = FLAG_ON_FIELD;
              FieldFlags[column + 1][row] = FLAG_ON_FIELD;
              FieldFlags[column + 2][row] = FLAG_ON_FIELD;
              FieldFlags[column + temp][row + 1] = FLAG_ON_FIELD;
            }
          }
        }
      }
    }
  }
}




// blobs
//-------

void clearBlob(int array_x, int array_y)
{
  Field [array_x][array_y] = BLOB_FREE;
}

void MoveBlobsRight()
{
  if (!IsOnlyOneBlob() && IsMovePossible(BlobsXY[0] + 1, BlobsXY[1])) BlobsXY[0]++;
}


void MoveBlobsLeft()
{
  if (!IsOnlyOneBlob() && IsMovePossible(BlobsXY[0] - 1, BlobsXY[1])) BlobsXY[0]--;
}


void DropBlobs()
{
  if (IsOneBlobDropPossible(BlobsXY[0], BlobsXY[1] + 1)) StoreOneBlob(BlobsXY[0], BlobsXY[1]);
  //move down is no longer possible because the field is full, the game is over
  if ((BlobsXY[1] == 0) && !IsTileFree(BlobsXY[0] + 1, 0)) game_state = STATE_GAME_OVER;
  if (IsMovePossible(BlobsXY[0], BlobsXY[1] + 1))
  {
    BlobsXY[1]++;
    arduboy.tunes.tone(104, 10);
  }
  else if (game_state != STATE_GAME_OVER)
  {
    StoreBlob(BlobsXY[0], BlobsXY[1]);
    scorePlayer += 10;
    DeletePossibleBlobs();
    CreateCurrentBlobs();
  }
}

void RotateBlobsRight()
{
  if (!IsOnlyOneBlob())
  {
    byte temp = Current_Blobs[1];
    Current_Blobs[1] = Current_Blobs[3];
    Current_Blobs[3] = Current_Blobs[7];
    Current_Blobs[7] = Current_Blobs[5];
    Current_Blobs[5] = temp;
    arduboy.tunes.tone(330, 25);
  }
  if (!IsMovePossible(BlobsXY[0], BlobsXY[1]))
  {
    byte temp = Current_Blobs[1];
    Current_Blobs[1] = Current_Blobs[5];
    Current_Blobs[5] = Current_Blobs[7];
    Current_Blobs[7] = Current_Blobs[3];
    Current_Blobs[3] = temp;
  }
}

void RotateBlobsLeft()
{
  if (!IsOnlyOneBlob())
  {
    byte temp = Current_Blobs[1];
    Current_Blobs[1] = Current_Blobs[5];
    Current_Blobs[5] = Current_Blobs[7];
    Current_Blobs[7] = Current_Blobs[3];
    Current_Blobs[3] = temp;
    arduboy.tunes.tone(330, 25);
  }
  if (!IsMovePossible(BlobsXY[0], BlobsXY[1]))
  {
    byte temp = Current_Blobs[1];
    Current_Blobs[1] = Current_Blobs[3];
    Current_Blobs[3] = Current_Blobs[7];
    Current_Blobs[7] = Current_Blobs[5];
    Current_Blobs[5] = temp;
  }
}


void FillBlobPit()
{
  for (byte x = 0; x < 6; x++)
  {
    RandomBlobPit[x] = random(1, 4);
  }
}

void DrawBlobs (int draw_x, int draw_y, int which_blobs)
{
  switch (which_blobs)
  {
    case BLOB_CURRENT:
      {
        int draw_pointer = 0;
        for (int y = draw_y; y < draw_y + 18; y = y + BLOB_PIXELS)
        {
          for (int x = draw_x; x < draw_x + 18; x = x + BLOB_PIXELS)
          {
            int temp = Current_Blobs[draw_pointer];
            if (temp > 0)
            {
              arduboy.drawBitmap(x, y, blobs_bitmap[temp], 6, 6, 1);
            }
            draw_pointer++;
          }
        }

        break;
      }
    case BLOB_NEXT:
      arduboy.drawBitmap(draw_x, draw_y, bigblobs_bitmap[RandomBlobPit[0]], 10, 10, 1);
      arduboy.drawBitmap(draw_x, draw_y + (2 * BLOB_PIXELS), bigblobs_bitmap[RandomBlobPit[1]], 10, 10, 1);
      break;
    case BLOB_WAITING:
      arduboy.drawBitmap(draw_x, draw_y, blobsinverted_bitmap[RandomBlobPit[2]], 6, 6, 1);
      arduboy.drawBitmap(draw_x, draw_y + BLOB_PIXELS, blobsinverted_bitmap[RandomBlobPit[3]], 6, 6, 1);
      break;

  }
}

void CreateCurrentBlobs ()
{
  BlobsXY[0] = 2;     //player X
  BlobsXY[1] = 0;     //player Y
  for (byte i = 0; i < 9; i++)
  {
    Current_Blobs[i] = 0;
  }
  Current_Blobs[1] = RandomBlobPit[0];
  Current_Blobs[4] = RandomBlobPit[1];

  for (byte i = 0; i < 4; i++)
  {
    RandomBlobPit[i] = RandomBlobPit[i + 2];
  }

  RandomBlobPit[4] = random(1, 6);
  RandomBlobPit[5] = random(1, 6);
}

// stage
//-------
void pause()
{
  arduboy.drawBitmap(0, 0, playfield_bitmap, 128, 64, 1);
  arduboy.drawBitmap(88, 27, face_mask_bitmap, 24, 16, 0);
  arduboy.drawBitmap(88, 27, face_bitmap, 24, 16, 1);
  arduboy.drawBitmap((PLAYFIELD_WIDTH * BLOB_PIXELS) + 7, 3, bigblobsbackground_bitmap, 12, 24, 1);
  arduboy.drawBitmap((PLAYFIELD_WIDTH * BLOB_PIXELS) + 21, 9, blobsbackground_bitmap, 7, 13, 1);
  arduboy.drawBitmap(56, 13, pause_bitmap, 19, 5, 0);
  int draw_x = (BlobsXY[0] * BLOB_PIXELS) + PLAYFIELD_ZERO_X;
  int draw_y = (BlobsXY[1] * BLOB_PIXELS) + PLAYFIELD_ZERO_Y;
  int draw_pointer = 0;
  for (int y = draw_y; y < draw_y + 18; y = y + BLOB_PIXELS)
  {
    for (int x = draw_x; x < draw_x + 18; x = x + BLOB_PIXELS)
    {
      int temp = Current_Blobs[draw_pointer];
      if (temp > 0)
      {
        arduboy.drawBitmap(x, y, blob00inverted_bitmap, 6, 6, 0);
      }
      draw_pointer++;
    }
  }
}

void afterPause()
{
  arduboy.drawBitmap(88, 27, face_mask_bitmap, 24, 16, 0);
  arduboy.drawBitmap(0, 0, playfield_bitmap, 128, 64, 1);
  arduboy.drawBitmap((PLAYFIELD_WIDTH * BLOB_PIXELS) + 7, 3, bigblobsbackground_bitmap, 12, 24, 1);
  arduboy.drawBitmap((PLAYFIELD_WIDTH * BLOB_PIXELS) + 21, 9, blobsbackground_bitmap, 7, 13, 1);
}

void UpdateStage()
{
  DrawField();
  DrawCurrentBlobs();
  DrawNextAndWaitingBlobs();
  score_draw(57, 54);
}


void DrawNextAndWaitingBlobs()
{

  arduboy.drawBitmap((PLAYFIELD_WIDTH * BLOB_PIXELS) + 7, 3, bigblobsbackground_bitmap, 12, 24, 0);
  DrawBlobs((PLAYFIELD_WIDTH * BLOB_PIXELS) + 8, 4, BLOB_NEXT);
  arduboy.drawBitmap((PLAYFIELD_WIDTH * BLOB_PIXELS) + 21, 9, blobsbackground_bitmap, 7, 13, 0);
  DrawBlobs((PLAYFIELD_WIDTH * BLOB_PIXELS) + 22, 10, BLOB_WAITING);
}

void DrawCurrentBlobs()
{
  DrawBlobs((BlobsXY[0]*BLOB_PIXELS) + PLAYFIELD_ZERO_X, (BlobsXY[1]*BLOB_PIXELS) + PLAYFIELD_ZERO_Y, BLOB_CURRENT);
}

void score_draw(int scoreX, int scoreY)
{
  arduboy.drawBitmap(scoreX, scoreY, numberbackground_bitmap, 40, 5, 0);
  char buf[10];
  //scorePlayer = arduboy.cpuLoad();
  itoa(scorePlayer, buf, 10);
  char charLen = strlen(buf);
  char pad = 10 - charLen;

  //draw 0 padding
  for (byte i = 0; i < pad; i++)
  {
    arduboy.drawBitmap(scoreX + (4 * i), scoreY, number_bitmaps[0], 3, 5, 1);
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
    arduboy.drawBitmap(scoreX + (pad * 4) + (4 * i), scoreY, number_bitmaps[digit], 3, 5, 1);
  }
}

