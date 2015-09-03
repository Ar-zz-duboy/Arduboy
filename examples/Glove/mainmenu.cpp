#include <Arduino.h>
#include "globals.h"
#include "prompt.h"
#include "mainmenu.h"
#include "records.h"

const char mainMenuItems_0[] PROGMEM = "[     Main Menu     ]";
const char mainMenuItems_1[] PROGMEM = "Continue";
const char mainMenuItems_2[] PROGMEM = "New Game";
const char mainMenuItems_3[] PROGMEM = "Records";

const char* const mainMenuItems[] PROGMEM = {
	mainMenuItems_0,
	mainMenuItems_1,
	mainMenuItems_2,
	mainMenuItems_3
};
const char mainMenuItemsCount = 3;

const char noContinueMenuItems_0[] PROGMEM = "[     Main Menu     ]";
const char noContinueMenuItems_1[] PROGMEM = "New Game";
const char noContinueMenuItems_2[] PROGMEM = "Records";

const char* const noContinueMenuItems[] PROGMEM = {
	noContinueMenuItems_0,
	noContinueMenuItems_1,
	noContinueMenuItems_2
};
const char noContinueMenuItemsCount = 2;

void displayMainMenu()
{
	int address = GAME_SAVE_FILE * 10 * 5;
	unsigned char choice;
	if(EEPROM.read(10+address+FILE_CONTINUE) == 1) {
		while(choice != 255) {
			 choice = prompt_start(mainMenuItems, mainMenuItemsCount);
			if(choice == 0) {
				displayLoadGame(GAME_SAVE_FILE);
				break;
			}
			else if(choice == 1) {
				displayGame();
				break;
			}
			else if(choice == 2)
				displayRecords();
		}
	} else {
		while(choice != 255) {
			 choice = prompt_start(noContinueMenuItems, noContinueMenuItemsCount);
			if(choice == 0) {
				displayGame();
				break;
			}
			else if(choice == 1)
				displayRecords();
		}
	}
}