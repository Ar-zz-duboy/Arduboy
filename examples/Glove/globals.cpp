#include <Arduino.h>
#include "globals.h"

bool old_a = false;
bool old_b = false;
bool old_up = false;
bool old_down = false;
bool old_left = false;
bool old_right = false;

bool new_a = false;
bool new_b = false;
bool new_up = false;
bool new_down = false;
bool new_left = false;
bool new_right = false;

char progmemString[] = "";

void tautInput()
{
	new_a = true;
	new_b = true;
	new_up = true;
	new_down = true;
	new_left = true;
	new_right = true;
	updateOldInput();
}

void updateNewInput()
{
	new_a = !digitalRead(A1);
	new_b = !digitalRead(A0);
	new_up = !digitalRead(8);
	new_down = !digitalRead(10);
	new_left = !digitalRead(9);
	new_right = !digitalRead(5);
}

void updateOldInput()
{
	old_a = new_a;
	old_b = new_b;
	old_up = new_up;
	old_down = new_down;
	old_left = new_left;
	old_right = new_right;
}

char* rps(const char* const* str)
{
	strcpy_P(progmemString, (char*)pgm_read_word(str));
	return progmemString;
}

char* dps(const char* str)
{
	strcpy_P(progmemString, (char*)pgm_read_word(str));
	return progmemString;
}

void breakSave(byte file)
{
	int address = file * 10 * 5;
	
	// Write game identity
	EEPROM.write(address+FILE_VALID, 'L');
	EEPROM.write(address+FILE_VALID+1, 'o');
	EEPROM.write(address+FILE_VALID+2, 'M');
	EEPROM.write(address+FILE_VALID+3, 'e');
	EEPROM.write(address+FILE_VALID+4, 'q');
}

void initializeSave(byte file)
{
	int address = file * 10 * 5;
	
	// Write game identity
	EEPROM.write(address+FILE_VALID, 'L');
	EEPROM.write(address+FILE_VALID+1, 'o');
	EEPROM.write(address+FILE_VALID+2, 'M');
	EEPROM.write(address+FILE_VALID+3, 'e');
	EEPROM.write(address+FILE_VALID+4, 'M');
	
	// Blank out Room Visited Completion
	EEPROM.write(address+FILE_COMPLETION, 0);
	EEPROM.write(address+FILE_COMPLETION+1, 0);
	EEPROM.write(address+FILE_COMPLETION+2, 0);
	EEPROM.write(address+FILE_COMPLETION+3, 0);
	EEPROM.write(address+FILE_COMPLETION+4, 0);
	
	// Blank out Continue File
	EEPROM.write(10+address+FILE_SCORE, 0);
	EEPROM.write(10+address+FILE_SCORE+1, 0);
	EEPROM.write(10+address+FILE_TIME, 0);
	EEPROM.write(10+address+FILE_TIME+1, 0);
	EEPROM.write(10+address+FILE_LEVEL, 0);
	EEPROM.write(10+address+FILE_CURRENT_LEVEL, 0);
	EEPROM.write(10+address+FILE_HEALTH, 0);
	EEPROM.write(10+address+FILE_HEALTH+1, 0);
	EEPROM.write(10+address+FILE_CONTINUE, 0);
	
	// Blank out High Score files
	for(char player = 0; player < 30; player += 10) {
		EEPROM.write(20+address+player+FILE_SCORE, 0);
		EEPROM.write(20+address+player+FILE_SCORE+1, 0);
		EEPROM.write(20+address+player+FILE_TIME, 0);
		EEPROM.write(20+address+player+FILE_TIME+1, 0);
		EEPROM.write(20+address+player+FILE_LEVEL, 0);
		EEPROM.write(20+address+player+FILE_NAME, 0);
		EEPROM.write(20+address+player+FILE_NAME+1, 0);
		EEPROM.write(20+address+player+FILE_NAME+2, 0);
		EEPROM.write(20+address+player+FILE_NAME+3, 0);
	}
}

bool checkFileValidity(byte file)
{
	int address = file * 10 * 5;
	if((EEPROM.read(address+FILE_VALID) == 'L')
		&& (EEPROM.read(address+FILE_VALID+1) == 'o')
		&& (EEPROM.read(address+FILE_VALID+2) == 'M')
		&& (EEPROM.read(address+FILE_VALID+3) == 'e')
		&& (EEPROM.read(address+FILE_VALID+4) == 'M'))
	{
		return true;
	}
	return false;
}

int getRoomClearPercentage()
{
	char i;
	int address = GAME_SAVE_FILE * 10 * 5;
	char block;
	char completed = 0;
	
	for(char blockNum = 0; blockNum < 5; ++blockNum) {
		block = EEPROM.read(address+FILE_COMPLETION+blockNum);
		for(i=0; i<8; ++i) {
			if((block>>i) & 1) completed++;
		}
	}
	
	return (completed*100)/30;
}


void printTime(unsigned short time)
{
	short mins = time/60;
	short secs = time%60;
	
	if(mins < 10) display.print('0');
	display.print(mins);
	display.print(':');
	if(secs < 10) display.print('0');
	display.print(secs);
		
}
