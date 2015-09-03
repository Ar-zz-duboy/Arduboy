#include <Arduino.h>
#include "globals.h"

void displayTitle()
{
	display.clearDisplay();	
	display.setTextSize(2);
	display.setCursor(32, 12);
	display.print(F("GLOVE"));
	display.setTextSize(1);
	//display.drawBitmap(0, 0, titleScreen, 128, 64, 1);
	
	int roomsCleared = getRoomClearPercentage();
	if(roomsCleared > 0) {
		display.setCursor(6, 8*6 + 4);
		display.print(F("ROOMS CLEARED: "));
		display.print(roomsCleared);
		display.print("%");
	} else {
		display.setCursor(0, scrh-8);
		display.print(F("fuopy 2015"));
	}
	
	display.display();
	delay(1000);
	
	
	while(true) {
		updateNewInput();
		display.drawPixel(random(0,128), random(0,64), 0);
		display.display();
		if(A_PRESSED) break;
		updateOldInput();
	}	
	updateOldInput();
}