#include "prompt.h"

#define TONE_ACCEPT 1300
#define TONE_BACK 400
#define TONE_MOVE 800

char prompt_start(const char* const* prompts, unsigned char promptCount)
{
	char choice = 0;
	prompt_draw(prompts, promptCount, choice);
	
	tautInput();
	while(true) {
		delay(10);
		updateNewInput();
		if(A_PRESSED) {
			display.tunes.tone(TONE_ACCEPT, 20);
			updateOldInput();
			return choice;
		}
		if(B_PRESSED) {		
			display.tunes.tone(TONE_BACK, 20);
			updateOldInput();
			return 255;
		}
		if(DOWN_PRESSED) {
			if(choice < promptCount-1) {
				choice++;
				display.tunes.tone(TONE_MOVE, 20);
				prompt_draw(prompts, promptCount, choice);
			}
		}
		if(UP_PRESSED) {
			if(choice > 0) {
				choice--;
				display.tunes.tone(TONE_MOVE, 20);
				prompt_draw(prompts, promptCount, choice);
			}
		}
		updateOldInput();
	}
	
	return choice;
}
void prompt_draw(const char* const* prompts, unsigned char promptCount, char choice)
{
	display.clearDisplay();
	display.setTextSize(1);
	display.setCursor(0, 0);
	display.print(rps(&(prompts[0])));
	unsigned char startPos = (choice>=6) ? choice-6 : 0;
	unsigned char endPos = startPos + 6;
	for(unsigned char i = startPos; (i<promptCount) && (i<=endPos); ++i) {
		if(i == choice) {
			display.setCursor(0, 8*(i+1-startPos));
			display.print(">");
		}
		display.setCursor(8, 8*(i+1-startPos));
		display.print(rps(&(prompts[i+1])));
	}
	display.display(); 
}

const char okString[] PROGMEM = "Continue";
void error(const char* message)
{
	const char* messages[] = {message, okString};
	prompt_start(messages, 1);
}


char prompt_table(const char choiceString[], const char* const rowStrings[], char rows, char tabWidth, char tabCount, char& cursor) {
	char i;
	for(i=0; i<rows; ++i) {
		display.setCursor(0, i*8+24);
		display.print(rps(&(rowStrings[i])));
	}
	
	updateOldInput();
	while(true) {
		updateNewInput();
		if(UP_PRESSED || DOWN_PRESSED || LEFT_PRESSED || RIGHT_PRESSED) {
			display.setCursor((cursor%tabCount) * 6 * tabWidth, (cursor/tabCount) * 8 + 24);
			display.print(" ");
		}
		if(A_PRESSED) {
			display.tunes.tone(TONE_ACCEPT, 20);
			return pgm_read_byte_near(choiceString + cursor);
		} else if(B_PRESSED) {
			display.tunes.tone(TONE_BACK, 20);
			return 0;
		} else if(UP_PRESSED) {
			display.tunes.tone(TONE_MOVE, 20);
			if(cursor < tabCount)
				cursor += tabCount*rows;
			cursor -= tabCount;
		} else if(DOWN_PRESSED) {
			display.tunes.tone(TONE_MOVE, 20);
			cursor += tabCount;
			if(cursor >= tabCount*rows)
				cursor -= tabCount*rows;
		} else if(LEFT_PRESSED) {
			display.tunes.tone(TONE_MOVE, 20);
			if(cursor % tabCount == 0)
				cursor += tabCount;
			cursor --;
		} else if(RIGHT_PRESSED) {
			display.tunes.tone(TONE_MOVE, 20);
			cursor ++;
			if(cursor % tabCount == 0)
				cursor -= tabCount;
		}
		display.setCursor((cursor%tabCount) * 6 * tabWidth, (cursor/tabCount) * 8 + 24);
		display.print(">");
		display.display();
		updateOldInput();
	}
}

const char alphaRowStrings_0[] PROGMEM = " 1 2 3 4 5 6 7 8 9 0";
const char alphaRowStrings_1[] PROGMEM = " Q W E R T Y U I O P";
const char alphaRowStrings_2[] PROGMEM = " A S D F G H J K L  ";
const char alphaRowStrings_3[] PROGMEM = " Z X C V B N M      ";

const char* const alphaRowStrings[] PROGMEM = {
	alphaRowStrings_0,
	alphaRowStrings_1,
	alphaRowStrings_2,
	alphaRowStrings_3
};
const char numAlphaRows = 4;
const char keyboardReturnString[] PROGMEM = "1234567890QWERTYUIOPASDFGHJKL ZXCVBNM   ";

char prompt_keyboard(char &cursorPos)
{
	return prompt_table(keyboardReturnString, alphaRowStrings, numAlphaRows, 2, 10, cursorPos);
}

void prompt_flash(const char* msg)
{
		display.clearDisplay();
		display.setCursor(0, 0);
		display.print(msg);
		display.display();
		delay(600);
}
