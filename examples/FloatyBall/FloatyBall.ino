/*
  Floatyball.ino
  Written by Chris Martinez, 3/5/2014
*/

#include <SPI.h>
#include <EEPROM.h>
#include "Arduboy.h"
#include "bitmaps.h"

Arduboy arduboy;

int gameFPS = 1000/30;

// Things that make the game work the way it do
const int pipeArraySize = 4;  // For the for loops, at current settings only 3 sets of pipes can be onscreen at once
int pipes[2][pipeArraySize];  // Row 0 for x values, row 1 for gap location
int pipeWidth = 12;
int pipeGapHeight = 30;
int pipeCapWidth = 2;
int pipeCapHeight = 3;        // Caps push back into the pipe, it's not added length
int pipeMinHeight = 8;        // Higher values center the gaps more
int pipeGenTimer = 30;        // How many frames until a new pipe is generated

int ballY = 32;               // Floaty's starting height
int ballX = 64;               // Floaty's X Axis
int ballRadius = 4;
int ballFlapper = ballRadius; // Floaty's wing length
int jumpHeight = -6;          // Jumping is negative because 0 is up



// Storage Vars
int gameState = 0;
int gameScore = 0;
int gameScoreX = 0;
int gameScoreY = 0;
int gameScoreVY = 0;
int gameScoreRiser = 0;
int gameHighScore = 0;
int pipeGenCount = 0;  // Frame counter to generate new pipes
int ballVY = 0;
unsigned long lTime;


// Sounds
const byte PROGMEM bing [] = {
0x90,0x30, 0,107, 0x80, 0x90,0x60, 1,244, 0x80, 0xf0};
const byte PROGMEM intro [] = {
0x90,72, 1,244, 0x80, 0x90,60, 1,244, 0x80, 0x90,64, 1,244, 0x80, 0x90,69, 1,244, 0x80, 0x90,67, 
1,244, 0x80, 0x90,60, 1,244, 0x80, 0x90,72, 1,244, 0x80, 0x90,60, 1,244, 0x80, 0x90,64, 1,244, 
0x80, 0x90,69, 1,244, 0x80, 0x90,67, 1,244, 0x80, 0x90,60, 1,244, 0x80, 0x90,69, 1,244, 0x80, 
0x90,57, 1,244, 0x80, 0x90,60, 1,244, 0x80, 0x90,65, 1,244, 0x80, 0x90,62, 1,244, 0x80, 0x90,57, 
1,244, 0x80, 0x90,69, 1,244, 0x80, 0x90,57, 1,244, 0x80, 0x90,60, 1,244, 0x80, 0x90,67, 1,244, 
0x80, 0x90,62, 1,244, 0x80, 0x90,65, 1,244, 0x80, 0x90,72, 7,208, 0x80, 0xf0};
const byte PROGMEM point [] = {
0x90,83, 0,75, 0x80, 0x90,88, 0,225, 0x80, 0xf0};
const byte PROGMEM flap [] = {
0x90,24, 0,125, 0x80, 0xf0};
const byte PROGMEM horns [] = {
0x90,60, 1,44, 0x80, 0x90,50, 0,150, 0x80, 0,150, 0x90,48, 0,150, 0x80, 0x90,55, 2,238, 
0x80, 0x90,62, 3,132, 0x80, 0x90,60, 0,37, 0x80, 0x90,59, 0,37, 0x80, 0x90,57, 0,37, 0x80, 
0x90,55, 0,37, 0x80, 0x90,53, 0,18, 0x80, 0x90,52, 0,18, 0x80, 0x90,50, 0,18, 0x80, 0x90,48, 
0,18, 0x80, 0x90,47, 0,18, 0x80, 0x90,45, 0,18, 0x80, 0x90,43, 0,18, 0x80, 0x90,41, 0,18, 
0x80, 0xf0};
const byte PROGMEM hit [] = { 
0x90,60, 0,31, 0x80, 0x90,61, 0,31, 0x80, 0x90,62, 0,31, 0x80, 0xf0};

void drawFloor() {
  arduboy.drawLine(0,HEIGHT-1,WIDTH-1,HEIGHT-1,WHITE);
}
void drawFloaty() {
  ballFlapper--;
  if (ballFlapper < 0) { ballFlapper = ballRadius; }  // Flapper starts at the top of the ball
  arduboy.drawCircle(ballX, ballY, ballRadius, BLACK);  // Black out behind the ball
  arduboy.drawCircle(ballX, ballY, ballRadius, WHITE);  // Draw outline
  arduboy.drawLine(ballX, ballY, ballX-(ballRadius+1), ballY - ballFlapper, WHITE);  // Draw wing
  arduboy.drawPixel(ballX-(ballRadius+1), ballY - ballFlapper + 1, WHITE);  // Dot the wing
  arduboy.drawPixel(ballX+1, ballY-2, WHITE);  // Eye	
}
void drawPipes() {
  for (int x = 0; x < pipeArraySize; x++){
    if (pipes[0][x] != 255) {  // value set to 255 if array element is inactive,
                               // otherwise it is the xvalue of the pipe's left edge
      // Pipes
      arduboy.drawRect(pipes[0][x], -1, pipeWidth, pipes[1][x], WHITE);
      arduboy.drawRect(pipes[0][x], pipes[1][x] + pipeGapHeight, pipeWidth, HEIGHT - pipes[1][x] - pipeGapHeight, WHITE);
      // Caps
      arduboy.drawRect(pipes[0][x] - pipeCapWidth, pipes[1][x] - pipeCapHeight, pipeWidth + (pipeCapWidth*2), pipeCapHeight, WHITE);
      arduboy.drawRect(pipes[0][x] - pipeCapWidth, pipes[1][x] + pipeGapHeight, pipeWidth + (pipeCapWidth*2), pipeCapHeight, WHITE);
      // Detail lines
      arduboy.drawLine(pipes[0][x]+2, 0, pipes[0][x]+2, pipes[1][x]-5, WHITE);
      arduboy.drawLine(pipes[0][x]+2, pipes[1][x] + pipeGapHeight + 5, pipes[0][x]+2, HEIGHT - 3,WHITE);
    }
  }
}
void generatePipe() {
  for (int x = 0; x < pipeArraySize; x++) {
    if (pipes[0][x] == 255) {  // If the element is inactive (255)
      pipes[0][x] = WIDTH;  // Then create it starting right of the screen
      pipes[1][x] = random(pipeMinHeight,HEIGHT-(pipeMinHeight*2)-pipeGapHeight);
      return;
    }
  }
}
bool checkPipe(int x) {  // Collision detection, x is pipe to check
  int AxA = ballX - (ballRadius-1);  // Hit box for floaty is a square
  int AxB = ballX + (ballRadius-1);  // If the ball radius increases too much, corners
  int AyA = ballY - (ballRadius-1);  // of the hitbox will go outside of floaty's
  int AyB = ballY + (ballRadius-1);  // drawing
  int BxA, BxB, ByA, ByB;
	
  // check top cylinder
  BxA = pipes[0][x];
  BxB = pipes[0][x] + pipeWidth;
  ByA = 0;
  ByB = pipes[1][x];
  if (AxA < BxB && AxB > BxA && AyA < ByB && AyB > ByA) { return true; } // Collided with top pipe
	
  // check top cap
  BxA = pipes[0][x] - pipeCapWidth;
  BxB = BxA + pipeWidth + (pipeCapWidth*2);
  ByA = pipes[1][x] - pipeCapHeight;
  if (AxA < BxB && AxB > BxA && AyA < ByB && AyB > ByA) { return true; } // Collided with top cap
	
  // check bottom cyllinder
  BxA = pipes[0][x];
  BxB = pipes[0][x] + pipeWidth;
  ByA = pipes[1][x] + pipeGapHeight;
  ByB = HEIGHT-1;
  if (AxA < BxB && AxB > BxA && AyA < ByB && AyB > ByA) { return true; } // Collided with bottom pipe
	
  // check bottom cap
  BxA = pipes[0][x] - pipeCapWidth;
  BxB = BxA + pipeWidth + (pipeCapWidth*2);
  ByB = ByA + pipeCapHeight;
  if (AxA < BxB && AxB > BxA && AyA < ByB && AyB > ByA) { return true; } // Collided with bottom pipe

  return false; // Nothing hits
}
int getOffset(int s) {
  if (s > 9999) { return 20; }
  if (s > 999) { return 15; }
  if (s > 99) { return 10; }
  if (s > 9) { return 5; }
  return 0;
}
void setup() {
  arduboy.start();
  for(int i=-8; i<28; i=i+2) {
    arduboy.clearDisplay();
    arduboy.drawSlowXYBitmap(46,i, arduino, 32,8,1);
    arduboy.display();
  }
  arduboy.tunes.playScore (bing);
  delay(2000);
  arduboy.clearDisplay();
  arduboy.drawSlowXYBitmap(0,0,floatyball,128,64,1);
  arduboy.display();
  arduboy.tunes.playScore (intro);
  delay(500);
  arduboy.setCursor(18,55);
  arduboy.print("Press Any Button");
  arduboy.display();

  while (!arduboy.getInput());

  delay(500);
  for (int x = 0; x < pipeArraySize; x++) { pipes[0][x] = 255; }  // set all pipes offscreen
  lTime = millis();
}
void loop() {
  if (millis() > lTime + gameFPS) {
    arduboy.clearDisplay();
    lTime = millis();
    if (gameState == 0) {       // If the game is paused
      drawFloor();
      drawFloaty();
      if (arduboy.getInput()) { // Wait for a button press
        gameState = 1;          // Then start the game
        ballVY = jumpHeight;    // And make Floaty jump
        if (arduboy.tunes.playing()) { arduboy.tunes.stopScore(); }
        arduboy.tunes.playScore (flap);
      }
    }
    if (gameState == 1) {       // If the game is playing
      pipeGenCount++;           // inc pipe generator counter 1 frame
      if (ballVY > 0) {         // If the ball isn't already rising, check for jump
        if (arduboy.pressed(B_BUTTON) || arduboy.pressed(A_BUTTON)) {
          ballVY = jumpHeight;  // jump
          if (arduboy.tunes.playing()) { arduboy.tunes.stopScore(); }
          arduboy.tunes.playScore (flap);
        }
      }
      if (pipeGenCount > pipeGenTimer) {  // Every pipeGenTimer worth of frames
        generatePipe();                   // Generate a pipe
        pipeGenCount = 0;                 // Reset the generator counter
      }
      ballY = ballY + ballVY;             // Move the ball according to ballVY
      if (ballY < ballRadius) { ballY = ballRadius; } // No clipping the top
      ballVY++;                           // Decrement VY
      for (int x = 0; x < pipeArraySize; x++) {  // For each pipe array element
        if (pipes[0][x] != 255) {         // If the x value isn't 255
          pipes[0][x] = pipes[0][x] - 2;  // Then move it left 2px
          if (pipes[0][x] + pipeWidth < 0) {  // If the pipe's right edge is off screen
          pipes[0][x] = 255;              // Then set its value to 255
        }
          if (pipes[0][x] + pipeWidth == (ballX-ballRadius)) {  // If the pipe passed Floaty
            gameScore++;                  // And increment the score
            gameScoreX = ballX;           // Load up the floating text with
            gameScoreY = ballY - ballRadius; // Current ball x/y values
            gameScoreRiser = 15;          // And set it for 15 frames
            if (arduboy.tunes.playing()) { arduboy.tunes.stopScore(); }
            arduboy.tunes.playScore (point);
          }
        }
      }

      if (gameScoreRiser > 0) {  // If we have floating text
        arduboy.setCursor(gameScoreX - 2,gameScoreY + gameScoreRiser - 24);
        arduboy.print(gameScore);								
        gameScoreX = gameScoreX - 2;
        gameScoreRiser--;
      }

      if (ballY + ballRadius > (HEIGHT-1)) {  // If the ball has fallen below the screen
        ballY = (HEIGHT-1) - ballRadius;      // Don't let the ball go under :O
        gameState = 2;                        // Game over. State is 2.
      }
      // Collision checking
      for (int x = 0; x < 6; x++) {             // For each pipe array element
        if (pipes[0][x] != 255) {               // If the pipe is active (not 255)
          if (checkPipe(x)) { gameState = 2; }  // If the check is true, game over
        }
      }

      drawPipes();
      drawFloor();
      drawFloaty();

    }
    if (gameState == 2) {  // If the gameState is 2 then we draw a Game Over screen w/ score
      if (gameScore > gameHighScore) { gameHighScore = gameScore; }
      if (arduboy.tunes.playing()) { arduboy.tunes.stopScore(); }
      arduboy.display();              // Make sure final frame is drawn
      arduboy.tunes.playScore (hit);  // Hit sound
      delay(100);                     // Pause for the sound
      while (ballY + ballRadius < (HEIGHT-1)) {  // While floaty is still airborne
      if (ballVY < 0) { ballVY = 0; } // Stop any upward momentum
        ballY = ballY + ballVY;       // Fall
        ballVY++;                     // Increase falling speed
        if (ballY + ballRadius > (HEIGHT-1)) { ballY = HEIGHT - ballRadius; } // Don't fall through the floor plx
        arduboy.clearDisplay();
        drawPipes();
        drawFloor();
        drawFloaty();
        arduboy.display();
      }
      arduboy.tunes.playScore (horns);     // SOUND THE LOSER'S HORN	
      arduboy.drawRect(16,8,96,48, WHITE); // Box border
      arduboy.fillRect(17,9,94,46, BLACK); // Black out the inside
      arduboy.drawSlowXYBitmap(30,12,gameover,72,14,1);
      arduboy.setCursor(56 - getOffset(gameScore),30);
      arduboy.print(gameScore);
      arduboy.setCursor(69,30);
      arduboy.print("Score");

      arduboy.setCursor(56 - getOffset(gameHighScore),42);
      arduboy.print(gameHighScore);
      arduboy.setCursor(69,42);
      arduboy.print("High");

      arduboy.display();

      while (!arduboy.getInput());

      gameState = 0;       // Then start the game paused
      gameScore = 0;       // Reset score to 0
      gameScoreRiser = 0;  // Clear the floating score
      for (int x = 0; x < pipeArraySize; x++) { pipes[0][x] = 255; }	// set all pipes inactive
      ballY = 32;          // Reset ball to center
      ballVY = 0;          // With zero lift
      delay(250);          // Slight delay so input doesn't break pause

    }
    arduboy.display();  // Finally draw this thang
  }
}
