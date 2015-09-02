/*
 Block Stacker
Copyright (C) 2015 David Martinez
All rights reserved.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.
*/

#include <SPI.h>
#include "Arduboy.h"
#include "bitmaps.h"
#include <EEPROM.h>


Arduboy arduboy;
#include "pins_arduino.h" // Arduino pre-1.0 needs this

// Variables
int score;

#define LEFT 4
#define RIGHT 6

const byte gridxSize = 7;
const byte gridySize = 15;

byte grid[gridySize][gridxSize];
byte gridxPos = 50;
byte gridyPos = 0;

byte unitSize = 4;
byte direct = RIGHT;
byte currentLine;
const byte tickCountMaster = 5;
byte tickCountReset = tickCountMaster;
byte tickCount = 0;
boolean buttonHeld = false;
byte currentNumBlocks;



void intro()
{
  for (int i = -8; i < 28; i = i + 2)
  {
    arduboy.clearDisplay();
    arduboy.setCursor(46, i);
    arduboy.print("ARDUBOY");
    arduboy.display();
  }

  arduboy.tunes.tone(987, 160);
  delay(160);
  arduboy.tunes.tone(1318, 400);
  delay(2000);
}


void logoSplash(){
  arduboy.clearDisplay();
  arduboy.drawBitmap(0,0,logo,128,64,WHITE);
  arduboy.display();
  delay (3000);
}

void clearGrid() {
  currentLine = gridySize -1;
  direct = RIGHT;
  currentNumBlocks = 3;
  tickCountReset = tickCountMaster;
  tickCount = 0;
  for (byte y = 0; y < gridySize; y++) {
    for (byte x = 0; x < gridxSize; x++) {
      grid[y][x] =0;
    }

  }
  grid[currentLine][2] = 1;
  grid[currentLine][3] = 1;
  grid[currentLine][4] = 1;
  
  
  //drawbackground on buffer twice
  arduboy.clearDisplay();
  arduboy.fillRect(0,0,WIDTH,HEIGHT,WHITE);
  arduboy.drawBitmap(0,0,backImg,128,64,BLACK);
   arduboy.fillRect(gridxPos-1,gridyPos+3, unitSize * gridxSize + 2,unitSize * gridySize ,BLACK);
  
  arduboy.display();
  
}

void setup()
{
  arduboy.start();
  intro();
  logoSplash();
  titleScreen();
  clearGrid();
 
}


void titleScreen(){
  byte flashTextTimer = 60;
  byte flashTextStop = 128;
  byte currentFlashTime = 0;
  bool exitTitle = false;
  
  while(!exitTitle){
 arduboy.clearDisplay();
 arduboy.setCursor( 0,10);
 arduboy.setTextSize (2);
 arduboy.print("Box Stacker");

 currentFlashTime++;
  if(currentFlashTime < flashTextTimer){
   arduboy.setCursor( 40,45);
   arduboy.setTextSize (1);
   arduboy.print("Press A");
  }else if(currentFlashTime > flashTextTimer){
   //do nothing 
  }if(currentFlashTime ==flashTextStop){
   currentFlashTime =0; 
  }
  
  arduboy.display();


 if(arduboy.pressed(A_BUTTON)){
  exitTitle = true; 
  delay(400);
 }

}
 
  
}

void updateLine(){
  tickCount ++;
  if(tickCount == tickCountReset){
    tickCount = 0;
  if(direct == RIGHT){
   if(!grid[currentLine][6]) {
      grid[currentLine][6] =   grid[currentLine][5] ;
       grid[currentLine][5] =   grid[currentLine][4] ;
    grid[currentLine][4] =   grid[currentLine][3] ;
    grid[currentLine][3] =   grid[currentLine][2] ;
    grid[currentLine][2] =   grid[currentLine][1] ;
    grid[currentLine][1] =   grid[currentLine][0] ;
    grid[currentLine][0] = 0;
   }else{direct = LEFT;}
  }
  
  else if(direct == LEFT){
   if(grid[currentLine][0]==0) {
    grid[currentLine][0] =   grid[currentLine][1] ;
    grid[currentLine][1] =   grid[currentLine][2] ;
    grid[currentLine][2] =   grid[currentLine][3] ;
    grid[currentLine][3] =   grid[currentLine][4] ;
     grid[currentLine][4] =   grid[currentLine][5] ;
      grid[currentLine][5] =   grid[currentLine][6] ;
    grid[currentLine][6] = 0;
   }else{direct = RIGHT;}
  }
  }
}

void drawGrid() {
  int8_t numYMoves = 0;
  for (byte y = 0; y < gridySize; y++) {
    numYMoves ++;
    for (byte x = 0; x < gridxSize; x++) {
      if (grid[y][x]==1) {
        arduboy.drawBitmap(gridxPos + x * unitSize ,gridyPos + y * unitSize, blockImg,4,8,WHITE);
       // arduboy.fillRect(gridxPos + x * unitSize , gridyPos +( y * unitSize) , unitSize, unitSize, WHITE);
      }

    }
  }
}
void checkWin(){
  if (currentLine == 0){
        arduboy.setCursor(40,25);
        arduboy.display();
        arduboy.print("YOU WIN!");
        arduboy.display();
        delay(5000);
        clearGrid();  
    } 
}
bool  checkGameover(){
   if(!grid[currentLine][0] &  !grid[currentLine][1] & !grid[currentLine][2]&  !grid[currentLine][3]&  !grid[currentLine][4]&  !grid[currentLine][5]&  !grid[currentLine][6]){
       arduboy.setCursor(40,25);
        arduboy.setTextSize (1);
        arduboy.display();
        
     
        
        
        arduboy.print("GAME OVER");
        arduboy.display();
        
            delay(100);
  arduboy.tunes.tone(400, 200);
  delay(400);
  arduboy.tunes.tone(200, 300);
  delay(400);
        delay(1000);
         clearGrid();
         return true;
         
   
   }
   return false;
}

void checkLine(){
 arduboy.tunes.tone(987, 160);
  
  
    if(currentLine != gridySize -1){
     for(byte x = 0;x<gridxSize;x++){
     if(grid[currentLine +1][x]==0){
      grid[currentLine][x] = 0; 
     
     }
    
     }
    }
  
    //reset current blocks left
    currentNumBlocks = 0;
    
    for(byte x = 0;x<gridxSize;x++){
     if(grid[currentLine][x]==1){
      grid[currentLine-1][x] = 1; 
      currentNumBlocks ++;
     }
    }
   
   if(currentLine == 9 ){
    if(currentNumBlocks >= 2 ){
     currentNumBlocks = 2;
     tickCountReset =4;
     tickCount = 3;
    for(byte x = 0;x<gridxSize;x++){
      grid[currentLine-1][x] = 0; 
    
     }
     grid[currentLine-1][2] = 1;
     grid[currentLine-1][3] = 1;
    }
     
    
  

} else if(currentLine == 5 ){
    if(currentNumBlocks >= 1){
     currentNumBlocks = 1;
      tickCountReset =3;
     tickCount = 2;
    for(byte x = 0;x<gridxSize;x++){
      grid[currentLine-1][x] = 0; 
    
     }
      grid[currentLine-1][3] = 1;
    }
     
    
    } else if(currentLine == 2 ){
       tickCountReset =2;
      tickCount = 1;
    }
   
    
    
}



void checkInput()
{
 
  if (arduboy.pressed(A_BUTTON)){
    if(!buttonHeld){
    buttonHeld = true;
  
     checkLine();
       if(checkGameover()){
        return; 
       }
      currentLine --;
      
      tickCount = 0;
    
      checkWin();
     
    }
    
  }else{buttonHeld = false;}

}





void Score()
{
  score += 10;

  arduboy.setCursor(80, 90);
  arduboy.print(score);
}







void loop()
{

  
  arduboy.fillRect(gridxPos-1,gridyPos+3, unitSize * gridxSize + 2,unitSize * gridySize ,BLACK);
  
  drawGrid();
  arduboy.display();
  checkInput();
  updateLine();
 
 
 

}


