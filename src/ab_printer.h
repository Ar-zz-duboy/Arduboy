#ifndef AbPrinter_h
#define AbPrinter_h

#include <Arduino.h>
#include "Arduboy.h"
#include <Print.h>

class Arduboy;

/// Separate class to break out Print subclassing
/**
 * This is necessary because C++ keeps a virtual function lookup table around
 * for the virtual function "write" that the Print subclass requires. That
 * single reference to the "write" function in the vtable means "write" will 
 * always be compiled and including in the final ouput (even if it's never 
 * actually used in your sketch) - which means it's dependencies are also 
 # included:
 * 
 *   - drawChar
 *   - font (256 bytes!)
 *   - drawRect (for large font 'pixels')
 *   - cursor_x, cursor_y, wrap_mode, etc.
 *
 * This quickly adds up to 1 to 2kb of wasted space that many graphics 
 * based games never take advantage of.  By breaking printing out into a 
 * separate class we prevent this.  If you need it use it, if not then it
 * will get dropped by the compiler during building.
 *
 */
class AbPrinter : public Print
{
public:
  AbPrinter(Arduboy &ab);

  /// Writes a single ASCII character to the screen.
  virtual size_t write(uint8_t);

  /// Sets the location of the screen cursor.
  void setCursor(int16_t x, int8_t y);

  /// Set text size
  /**
   * As mentioned in drawChar(), individual ASCII characters are 6x8 pixels
   * (5x7 with spacing on two edges). The size is a pixel multiplier,
   * so a size of 2 means each character will be 12x16, etc.
   */
  void setSize(uint8_t s);

  /// Sets whether text will wrap at screen edges.
  void setWrap(boolean w);
	
private:
	Arduboy *arduboy;

protected:
	int16_t cursor_x;
	int8_t cursor_y;
	uint8_t size;
	boolean wrap; // If set, 'wrap' text at right edge of display

};

#endif 
