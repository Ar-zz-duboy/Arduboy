#include <SPI.h>
#include "Arduboy.h"
#include <EEPROM.h>
#include "bitmaps.h"
Arduboy arduboy;

float a = 0;
float b = 0;
float c = 0;
float d = 0;
unsigned long lastSecond = 1000;
unsigned long secondInterval = 1000;
unsigned long lastMinute = 60000;
unsigned long minuteInterval = 60000;
unsigned long lastHour = 3600000;
unsigned long hourInterval = 3600000;

void setup() {
	arduboy.start();
	arduboy.clearDisplay();
	arduboy.display();
	arduboy.drawBitmap(14, 16, TEAMarg, 96, 32, 1);
	arduboy.display();
	delay(3000);
}

void loop() {
	arduboy.clearDisplay();
	  	for(byte i = 1; i <= 12; i++){
		arduboy.drawLine(63+(30*sin(d))*sin(-0+3.12+((6.28/12))*i),31+30*sin(0+4.72+((6.28/12))*i),63+(27*sin(d))*sin(-0+3.12+((6.28/12))*i),31+27*sin(0+4.72+((6.28/12))*i),1);
	}
	arduboy.drawLine(63,31,63+(20*sin(d))*sin(-a+3.1),31+20*sin(a+4.7),1);
	arduboy.drawLine(63,31,63+(15*sin(d))*sin(-b+1.1),31+15*sin(b+2.6),1);
	arduboy.drawLine(63,31,63+(10*sin(d))*sin(-c+5.3),31+10*sin(c+0.6),1);

	if(millis() >= lastSecond){
		a+=0.105;
		lastSecond += secondInterval;
	}
	if(millis() >= lastMinute){
		b+=0.105;
		lastMinute += minuteInterval;
		}
	if(millis() >= lastHour){
		c+=0.525;
		lastHour += hourInterval;
	}

	d+=0.01;
	arduboy.display();
}
