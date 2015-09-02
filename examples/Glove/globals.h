#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include <Arduino.h>

#include <SPI.h>
#include <EEPROM.h>
#include "Arduboy.h"
#include "ArduboyGlove.h"

#define GAME_SAVE_FILE 6 // Save file for use with the game

#define GSTR(X) strcpy(progmemString, PSTR(X))

#define FILE_VALID 0
#define FILE_COMPLETION 5

#define FILE_SCORE 0
#define FILE_TIME 2
#define FILE_LEVEL 4
#define FILE_CURRENT_LEVEL 5
#define FILE_HEALTH 6
#define FILE_NAME 6
#define FILE_CONTINUE 9

extern ArduboyGlove display;


// Input
#define A_DOWN new_a
#define A_PRESSED (new_a && !old_a)
#define A_RELEASED (!new_a && old_a)

#define B_DOWN new_b
#define B_PRESSED (new_b && !old_b)
#define B_RELEASED (!new_b && old_b)

#define UP_DOWN new_up
#define UP_PRESSED (new_up && !old_up)
#define UP_RELEASED (!new_up && old_up)

#define DOWN_DOWN new_down
#define DOWN_PRESSED (new_down && !old_down)
#define DOWN_RELEASED (!new_down && old_down)

#define LEFT_DOWN new_left
#define LEFT_PRESSED (new_left && !old_left)
#define LEFT_RELEASED (!new_left && old_left)

#define RIGHT_DOWN new_right
#define RIGHT_PRESSED (new_right && !old_right)
#define RIGHT_RELEASED (!new_right && old_right)

extern bool new_a, new_b, new_up, new_left, new_down, new_right;
extern bool old_a, old_b, old_up, old_down, old_left, old_right;

const unsigned char scrw = WIDTH;
const unsigned char scrh = HEIGHT;

void updateNewInput();
void updateOldInput();
bool checkFileValidity(byte file);
void initializeSave(byte file);
void breakSave(byte file);
void tautInput();
int getRoomClearPercentage();


extern char progmemString[52];
// Read PROGMEM string
char* rps(const char* const* str);
char* dps(const char* str);

void printTime(unsigned short time);

#endif
