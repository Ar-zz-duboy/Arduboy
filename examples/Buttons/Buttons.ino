/*
Buttons example
June 11, 2015
Copyright (C) 2015 David Martinez
All rights reserved.

This code is the most basic barebone code for showing how to use buttons in Arduboy.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.
*/

#include <SPI.h>
#include "Arduboy.h"
#include <EEPROM.h>

// make an instance of arduboy used for many functions
Arduboy arduboy;

// Variables for your game go here.
char text[] ="Press Buttons!";
byte x;
byte y;


// This function runs once in your game.
// use it for anything that needs to be set only once in your game.
void setup()
{
  //initiate arduboy instance
  arduboy.start();

  //here we set the framerate to 30, we do not need to run at default 60 and it saves us battery life.
  arduboy.setFrameRate(30);
  
  //set x and y to the middle of the screen
  x = 24;
  y = 32;

}

// our main game loop, this runs once every cycle/frame.
// this is where our game logic goes.
void loop()
{
  // pause render until it's time for the next frame
  if (!(arduboy.nextFrame()))
    return;



// the next couple of lines will deal with checking if the D-pad buttons are pressed
// and move our text accordingly.

// if the right button is pressed move 1 pixel to the right every frame
if(arduboy.pressed(RIGHT_BUTTON)){
  x = x + 1;
  
}

// if the left button is pressed move 1 pixel to the left every frame
if(arduboy.pressed(LEFT_BUTTON)){
  x = x - 1;
}

// if the up button is pressed move 1 pixel up every frame
if(arduboy.pressed(UP_BUTTON)){
  y = y - 1;
}

// if the down button is pressed move 1 pixel down every frame
if(arduboy.pressed(DOWN_BUTTON)){
  y = y + 1;
}
// if the A button is pressed move 1 pixel up every frame
if(arduboy.pressed(A_BUTTON)){
   y = y - 1;
}

// if the B button is pressed move 1 pixel down every frame
if(arduboy.pressed(B_BUTTON)){
   y = y + 1;
}


 



  //we clear our screen to black
  arduboy.clearDisplay();

  //we set our cursor x pixels to the right and y down from the top
  arduboy.setCursor(x, y);

  //then we print to screen what is stored in our text variable we declared earlyer
  arduboy.print(text);

  //then we finaly we tell the arduboy to display what we just wrote to the display.
  arduboy.display();
}


