/**
 * @file Arduboy.h
 * \brief The Arduboy class definition.
 *
 */

#ifndef ARDUBOY_H
#define ARDUBOY_H

#include "ArduboyBase.h"
#include "ArduboyTunes.h"

class Arduboy : public Print, public ArduboyBase
{
public:
  /// Arduboy constructor.
  Arduboy();

  ArduboyTunes tunes;

  /**
   * Writes a single ASCII character to the screen.
   * \return returns size_t.
   */
  virtual size_t write(uint8_t);

  /**
   * Sets the location of the text cursor.
   * \param x
   * \param y
   */
  void setCursor(int16_t x, int16_t y);

  /**
   * Get the text cursor X position.
   */
  uint16_t getCursorX();

  /**
   * Get the text cursor Y position.
   */
  uint16_t getCursorY();

  /**
   * Sets the text foreground color.
   * \param color Pass an unsigned byte to set as color.
   */
  void setTextColor(uint8_t color);

  /**
   * Sets the text background color.
   * \param color Pass an unsigned byte to set as background color.
   */
  void setTextBackground(uint8_t bg);

  /**
   * Set the text size.
   * \param s Size of text passed as uint8_t.
   * \details
   * Individual ASCII characters are 6x8 pixels
   * (5x7 with spacing on two edges). The size is a pixel multiplier,
   * so a size of 2 means each character will be 12x16, etc.
   */
  void setTextSize(uint8_t s);

  /**
   * Sets whether text will wrap at screen edges.
   * \param w uint8_t
   */
  void setTextWrap(bool w);

  /**
   * Clears the display and sets the cursor to 0, 0
   */
  void clear();

protected:
  /// Cursor x position.
  int16_t cursor_x;
  /// Cursor y position.
  int16_t cursor_y;
  /// Color of text to print.
  uint8_t textColor;
  /// Color of text background.
  uint8_t textBackground;
  /// Line height of text to print.
  uint8_t textSize;
  /// Flag to set line wraps.
  bool textWrap;
};

#endif
