/** 
 * @file Arduboy.cpp
 * \brief Implementation of the Arduboy class.
 *
 */

#include "Arduboy.h"

/*
 * Arduboy Class
 */

Arduboy::Arduboy()
{
  cursor_x = 0;
  cursor_y = 0;
  textColor = 1;
  textBackground = 0;
  textSize = 1;
  textWrap = 0;
}

size_t Arduboy::write(uint8_t c)
{
  if (c == '\n')
  {
    cursor_y += textSize * 8;
    cursor_x = 0;
  }
  else if (c == '\r') ; // skip carriage returns
  else
  {
    drawChar(cursor_x, cursor_y, c, textColor, textBackground, textSize);
    cursor_x += textSize * 6;
    if (textWrap && (cursor_x > (WIDTH - textSize * 6)))
    {
      // calling ourselves recursively for 'newline' is 
      // 12 bytes smaller than doing the same math here
      write('\n');
    }
  }
  return 1;
}

void Arduboy::setCursor(int16_t x, int16_t y)
{
  cursor_x = x;
  cursor_y = y;
}

uint16_t Arduboy::getCursorX()
{
  return cursor_x;
}

uint16_t Arduboy::getCursorY()
{
  return cursor_y;
}

void Arduboy::setTextColor(uint8_t color)
{
  textColor = color;
}

void Arduboy::setTextBackground(uint8_t bg)
{
  textBackground = bg;
}

void Arduboy::setTextSize(uint8_t s)
{
  // size must always be 1 or higher
  textSize = max(1, s);
}

void Arduboy::setTextWrap(bool w)
{
  textWrap = w;
}

void Arduboy::clear()
{
  ArduboyBase::clear();
  cursor_x = cursor_y = 0;
}
