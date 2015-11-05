#ifndef Arduboy_h
#define Arduboy_h

#include "core.h"
#include <SPI.h>
#include <Print.h>
#include <limits.h>


// EEPROM settings

#define EEPROM_VERSION 0
#define EEPROM_BRIGHTNESS 1
#define EEPROM_AUDIO_ON_OFF 2
// we reserve the first 16 byte of EEPROM for system use
#define EEPROM_STORAGE_SPACE_START 16 // and onward

// eeprom settings above are neded for audio
#include "audio.h"

#define PIXEL_SAFE_MODE


// compare Vcc to 1.1 bandgap
#define ADC_VOLTAGE _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1)
// compare temperature to 2.5 internal reference
// also _BV(MUX5)
#define ADC_TEMP _BV(REFS0) | _BV(REFS1) | _BV(MUX2) | _BV(MUX1) | _BV(MUX0)

#define PIN_SPEAKER_1 A2
#define PIN_SPEAKER_2 A3

class Arduboy : public Print, public ArduboyCore
{
public:
  Arduboy();

  /// Returns true if the button mask passed in is pressed.
  /**
   * if (pressed(LEFT_BUTTON + A_BUTTON))
   */
  boolean pressed(uint8_t buttons);
  
  /// Returns true if the button mask passed in not pressed.
  /**
   * if (not_pressed(LEFT_BUTTON))
   */
  boolean not_pressed(uint8_t buttons);
  
  /// This function sets up everything for the Arduboy's display and needs to be called at the start of your program.
  void start();
  
  /// A screen clear to black which uses the screen buffer. As with most of these functions, won't be seen until you call display().
  void clearDisplay();
  
  /// This function copies the contents of the screen buffer to the screen, finally displaying the image you've built on the buffer using all the other functions.
  /**
   * X and Y positions on the display are from the top left corner, thus a Y of 64
   * is the bottom of the screen and an X of 128 is the right side of the screen.
   * "Color" or "value" means choosing whether a pixel is lit or not - if color is
   * 0, the pixel is off (black), if color is 1, the pixel is on (white).
   * 
   * What is a screen buffer? Basically it is an internal, "virtual" screen that
   * you draw things on before sending it to the actual screen. This is so users
   * don't see you drawing directly on the screen including overlaying the
   * graphics, building the picture from its component parts. Sort of like drawing
   * on scrap paper privately before showing the drawing to everyone. Most of
   * these functions draw to the screen buffer, and then when you're ready you
   * call display() which sends the frame to the screen.
   */
  void display();
  
  /// Sets a single pixel on the screen buffer to white or black. This function is used by many of the line/shape drawing functions.
  void drawPixel(int x, int y, uint8_t color);
  
  uint8_t getPixel(uint8_t x, uint8_t y);
  
  /// Draws a circle of any radius in white or black. X and Y are the center point of the circle.
  void drawCircle(int16_t x0, int16_t y0, int16_t r, uint8_t color);
  
  /// Draws one or more "corners" of a circle (1/4th of the circle) depending on which bits are sit in cornername. This function is called by the rounded rectangle functions below.
  void drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint8_t color);
  
  /// Draws a filled-in circle of any radius in white or black. X and Y are the center point of the circle.
  void fillCircle(int16_t x0, int16_t y0, int16_t r, uint8_t color);
  
  /// Draws one or both vertical halves of a filled-in circle depending on which bits are sit in cornername. This function is called by the rounded rectangle functions below.
  void fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint8_t color);
  
  /// Draws a white or black line from one point to another using Bresenham's popular algorithm.
  void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t color);
  
  /// Draws a white or black rectangle of a certain width and height, starting at the top left corner.
  void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint8_t color);
  
  /// Draws a white or black vertical line of a certain height starting from the top.
  void drawFastVLine(int16_t x, int16_t y, int16_t h, uint8_t color);
  
  /// Draws a white or black horizontal line of a certain width starting from the left.
  void drawFastHLine(int16_t x, int16_t y, int16_t w, uint8_t color);
  
  /// Draws a filled-in rectangle in white or black and of a certain width and height, starting at the top left corner.
  void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint8_t color);
  
  /// Fills the entire screen buffer with white or black (uses a screen-sized fillRect to do this).
  void fillScreen(uint8_t color);
  
  /// Draws a rectangle with rounded edges in white or black and of a certain width and height, starting at the top left corner. You can set the radius of the rounded corners as well.
  void drawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint8_t color);
  
  /// Draws a filled-in rectangle with rounded edges in white or black and of a certain width and height, starting at the top left corner. You can set the radius of the rounded corners as well.
  void fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint8_t color);
  
  /// Draws a triangle between three points in white or black.
  void drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t color);
  
  /// Draws a filled-in triangle between three points in white or black.
  void fillTriangle (int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t color);
  
  /// Draws a bitmap from program memory to a specific X/Y location in white or black. This will be most commonly used for drawing sprites and other graphics on the screen.
  void drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint8_t color);
  
  /// Draws images that are bit-oriented horizontally.
  /**
   * This requires a lot of additional CPU power and will draw images much
   * more slowly than drawBitmap where the images are stored in a format that
   * allows them to be directly written to the screen hardware fast. It is
   * recommended you use drawBitmap when possible.
   */
  void drawSlowXYBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint8_t color);
  
  /// Draws an ASCII character to a specific X/Y location in white or black. Individual characters are 6x8 pixels (5x7 with spacing on two edges). "Size" is a pixel multiplier, so a size of 2 means each character will be 12x16, etc. If color is 0 and bg is 1, the character will be drawn inverted (i.e. black with a white background).
  void drawChar(int16_t x, int16_t y, unsigned char c, uint8_t color, uint8_t bg, uint8_t size);
  
  /// Sets the location of the internal cursor for the purpose of writing on the screen.
  void setCursor(int16_t x, int16_t y);
  
  /// Sets the text size. As mentioned in drawChar(), individual ASCII characters are 6x8 pixels (5x7 with spacing on two edges). The size is a pixel multiplier, so a size of 2 means each character will be 12x16, etc.
  void setTextSize(uint8_t s);
  
  /// Sets whether or not text written with the write() function will wrap when it reaches the edge of the screen. As the function is currently written, text will always wrap back to the left edge of the screen, and not to where you initially set the cursor's X value.
  void setTextWrap(boolean w);
  
  unsigned char* getBuffer();

  /// Writes a single ASCII character to the screen, with properties determined by the use of the above functions (setCursor() etc.). This function is called by the print() function to print entire strings of text. The actual print() function is included in Print.h.
  virtual size_t write(uint8_t);

  /// Returns the load on the CPU as a percentage.
  /**
   * This is based on how much of the time your app is spends rendering
   * frames.  This number can be higher than 100 if your app is rendering
   * really slowly.
   */
  int cpuLoad();
  
  /// Seed the random number generator with entropy from the temperature, voltage reading, and microseconds since boot.
  /**
   * This method is still most effective when called semi-randomly such
   * as after a user hits a button to start a game or other semi-random
   * events
   */
  void initRandomSeed();
  
  /// Swaps the contents of the a and b variables with each other.
  void swap(int16_t& a, int16_t& b);

  void setFrameRate(uint8_t rate);
  bool nextFrame();
  bool everyXFrames(uint8_t frames);

  ArduboyTunes tunes;
  ArduboyAudio audio;
  
  /// \cond NOT_IN_DOCUMENTATION
  uint8_t frameRate;
  uint16_t frameCount;
  uint8_t eachFrameMillis;
  long lastFrameStart;
  long nextFrameStart;
  bool post_render;
  uint8_t lastFrameDurationMs;
  /// \endcond

protected:
  unsigned char sBuffer[(HEIGHT*WIDTH)/8];

  uint8_t readCapacitivePin(int pinToMeasure);
  uint8_t readCapXtal(int pinToMeasure);
  uint16_t rawADC(byte adc_bits);

// Adafruit stuff
protected:
  /// \cond NOT_IN_DOCUMENTATION
  int16_t cursor_x;
  int16_t cursor_y;
  uint8_t textsize;
  boolean wrap; // If set, 'wrap' text at right edge of display
  /// \endcond
};

#endif
