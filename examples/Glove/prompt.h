#include "globals.h"

void prompt_draw(const char* const* prompts, unsigned char promptCount, char choice);
char prompt_start(const char* const* prompts, unsigned char promptCount);
void error(const char* message);
char prompt_keyboard(char& cursorPos);
void prompt_flash(const char* msg);