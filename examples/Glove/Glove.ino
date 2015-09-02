#include <SPI.h>
#include <EEPROM.h>

#include "Arduboy.h"
#include "ArduboyGlove.h"
#include "globals.h"
#include "prompt.h"
#include "mainmenu.h"
#include "game.h"
#include "title.h"

#include "pins_arduino.h" // Arduino pre-1.0 needs this

ArduboyGlove display;

PROGMEM const unsigned char arduino [] =
{
	0x3F, 0xFF, 0xFF, 0xFC, 0x40, 0x00, 0x00, 0x02, 0x89, 0x99,0x54,
	0x91, 0x95, 0x55, 0x56, 0xA9, 0x9D, 0x95, 0x55, 0xA9, 0x95, 0x59,
	0xD4, 0x91, 0x40, 0x00, 0x00, 0x02, 0x3F, 0xFF, 0xFF, 0xFC
};

bool validSave;

void intro()
{
	short y = -8;
	bool bounced = false;
	short vy = 0;
	while(y < 28) {
		y += vy;
		vy ++;
		if(y >= 28) {
			if(!bounced) {
				bounced = true;
				vy = -4;
				y = 27;
			} else {
				y = 28;
			}
		}
		display.clearDisplay();
		display.setCursor(46, y);
		display.print("ARDUBOY");
		display.display();
		delay(20);
	}
	delay(100);
	display.tunes.tone(987, 80);
	delay(80);
	display.tunes.tone(1318, 400);
	delay(1000);
}

void setup()
{
	SPI.begin();
	display.start();
	display.setTextSize(1);
	intro();
	
	validSave = checkFileValidity(GAME_SAVE_FILE);
	if(!validSave) 
		initializeSave(GAME_SAVE_FILE);
}

void loop() 
{
	displayTitle();
	
	updateNewInput();
	
	updateOldInput();
	
	if(validSave) {
		displayMainMenu();
	} else {
		displayGame();
		validSave = true;
	}
}
