#include <Arduino.h>
#include "globals.h"
#include "prompt.h"

#define TONE_BACK 400


void displayRecords()
{
	int address = GAME_SAVE_FILE * 10 * 5;
	
	display.clearDisplay();
	display.setCursor(0, 0);
	display.print(F("     - Records -     "));
	display.setCursor(0, 8*2);
	display.print(F("Name Score Time  #Rms"));
	
	byte hi;
	byte lo;
	short val;
	// Loop through scores
	int scoreAddress;
	
	for(char i=0; i<3; ++i) {
		scoreAddress = 20 + (i*10) + address;
		
		hi = EEPROM.read(scoreAddress+FILE_NAME);
		display.setCursor(0, 8*(3+i));
		display.print(char(hi));
		hi = EEPROM.read(scoreAddress+FILE_NAME+1);
		display.setCursor(6*1, 8*(3+i));
		display.print(char(hi));
		hi = EEPROM.read(scoreAddress+FILE_NAME+2);
		display.setCursor(6*2, 8*(3+i));
		display.print(char(hi));
		hi = EEPROM.read(scoreAddress+FILE_NAME+3);
		display.setCursor(6*3, 8*(3+i));
		display.print(char(hi));
		
		hi = EEPROM.read(scoreAddress+FILE_SCORE);
		lo = EEPROM.read(scoreAddress+FILE_SCORE+1);
		val = (hi << 8) | lo;
		display.setCursor(6*5, 8*(3+i));
		display.print(val);
		if(val>0) display.print("00");
		
		hi = EEPROM.read(scoreAddress+FILE_TIME);
		lo = EEPROM.read(scoreAddress+FILE_TIME+1);
		val = (hi << 8) | lo;
		display.setCursor(6*11, 8*(3+i));
		//display.print(val);
		printTime(val);
		
		hi = EEPROM.read(scoreAddress+FILE_LEVEL);
		display.setCursor(6*17, 8*(3+i));
		display.print(hi);
	}
	display.setCursor(0, 8*7);
	
	display.print(F("ROOMS CLEARED: "));
	display.print(getRoomClearPercentage());
	display.print("%");
	
	display.display();
	
	tautInput();
	while(true) {
		delay(10);
		updateNewInput();
		if(B_PRESSED || A_PRESSED) {
			if(A_PRESSED && LEFT_DOWN && RIGHT_DOWN) 
				initializeSave(GAME_SAVE_FILE);
			tone(A2, TONE_BACK, 20);
			break;
		}
		updateOldInput();
	}
	updateOldInput();
}
