/**
 * \file Arduboy.h
 * \brief The Arduboy class.
 *
 */

#ifndef ARDUBOY_H
#define ARDUBOY_H

#include "ArduboyCore.h"
#include <Print.h>
#include <limits.h>

// Library version.
// A version number, 'x.y.z', is stored in the form xxyyzz,
// where ((x * 10000) + (y * 100) + (z)),
// resulting in 'xxxyyzz', with no leading zeros.
#define ARDUBOY_LIB_VER 10200 // 1.2.0

// EEPROM settings
#define EEPROM_VERSION 0
#define EEPROM_BRIGHTNESS 1
#define EEPROM_AUDIO_ON_OFF 2

// we reserve the first 16 bytes of EEPROM for system use
#define EEPROM_STORAGE_SPACE_START 16

// eeprom settings above are neded for audio
#include "ArduboyAudio.h"

#define PIXEL_SAFE_MODE

// pixel colors
#define INVERT 2 // on/off pixel
#define WHITE  1 // pixel on
#define BLACK  0 // pixel off

// compare Vcc to 1.1 bandgap
#define ADC_VOLTAGE (_BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1))
// compare temperature to 2.5 internal reference and _BV(MUX5)
#define ADC_TEMP (_BV(REFS0) | _BV(REFS1) | _BV(MUX2) | _BV(MUX1) | _BV(MUX0))


class ArduboyBase : public ArduboyCore
{
public:
  /// Arduboy constructor
  /**
   * Constuctor for ArduboyBase sets up frame managment
   */
  ArduboyBase();

  /// implemenet audio
  /**
   * Class used to offer a general API for creating audio on an Arduboy
   */
  ArduboyAudio audio;

  /// Returns true if buttons in button_mask are pressed.
  /**
   * \fn pressed
   * \param buttons uint8_t argument for a button mask.
   * \return Returns true if all button in button mask are pressed.
   *
   * example:
   *   if (pressed(LEFT_BUTTON + A_BUTTON))
   *
   */
  boolean pressed(uint8_t buttons);

  /// Returns true if the button mask passed in not pressed.
  /**
   * \fn notPressed
   * \param buttons uint8_t argument for a button mask.
   * \return Returns true if every button is not pressed.
   * \see pressed()
   *
   * example:
   *  if (notPressed(LEFT_BUTTON))
   *
   */
  boolean notPressed(uint8_t buttons);

  /// Initialize hardware, boot logo, boot utilities, etc.
  void begin();

  /// Init just hardware, no logo, no boot utilities.
  /**
   * Look at the source for `begin()` and just rip out what you do not
   * need and start there.  Calling just `boot()` might work also
   * depending on your requirements.
   *
   * The minimum recommended `begin` replacement:
   *
   *   arduboy.boot()         // raw hardware init
   *   arduboy.audio.begin()  // if you need audio
   */
  // void boot(); // defined in core.cpp
  void start() __attribute__((deprecated, warning("use begin() instead")));

  /// Scrolls in the Arduboy logo
  void bootLogo();

  /// Clears display.
  void clear();

  /// Flashlight mode
  /**
   * Hold up key when booting to enable, press down key to exit
   * or simply turn off your Arduboy.  Your sketches can also
   * call this at any time.  It goes into a tight loop until the
   * down buttn is pressed.
   */
  void flashlight();
  void flashlight(uint8_t off_button);
  void flashlight(uint8_t on_button, uint8_t off_button);

  void clearDisplay() 
      __attribute__((deprecated, warning("use clear() instead")));

  /// Copies the contents of the screen buffer to the screen.
  /**
   * X and Y positions on the display are from the top left corner, thus a Y of
   * 64 is the bottom of the screen and an X of 128 is the right side of the
   * screen. "Color" or "value" means choosing whether a pixel is lit or not - 
   * if color is 0, the pixel is off (black), if color is 1, the pixel is on
   * (white).
   */
  void display();

  /// Manages the draw cycle for Arduboy
  /**
   * The managed draw function for Arduboy. Checks if frame is ready to be
   * drawn, returns if not. The function passed a parameter is executed along
   * with any cleanup routine.
   */
  int draw(void (*f)());

  /// Sets a pixel in the screen buffer to on or off. Default is on (white).
  void drawPixel(int x, int y, uint8_t color);

  /// Get the value of a pixel from the screen buffer at a specified x and y 
  /// coordinate.
  uint8_t getPixel(uint8_t x, uint8_t y);

  /// Draw a circle of a defined radius.
  /**
   * Draws a circle in white or black. X and Y are the center point of the
   * circle.
   */
  void drawCircle(int16_t x0, int16_t y0, uint8_t r, uint8_t color);

  /// Draws one or more "corners" of a circle.
  void drawCircleHelper(int16_t x0,
                        int16_t y0,
                        uint8_t r,
                        uint8_t cornername,
                        uint8_t color);

  /// Draws a filled-in circle.
  void fillCircle(int16_t x0, int16_t y0, uint8_t r, uint8_t color);

   /// Draws one or both vertical halves of a filled-in circle.
  void fillCircleHelper(int16_t x0,
                        int16_t y0,
                        uint8_t r,
                        uint8_t cornername,
                        int16_t delta,
                        uint8_t color);

  /// Draws a line between two points.
  /**
   * Uses Bresenham's algorithm.
   */
  void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t color);

  /// Draws a rectangle of a width and height.
  void drawRect(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t color);

  /// Draws vertical line.
  void drawFastVLine(int16_t x, int16_t y, uint8_t h, uint8_t color);

  /// Draws a horizontal line.
  void drawFastHLine(int16_t x, int16_t y, uint8_t w, uint8_t color);

  /// Draws a filled-in rectangle.
  void fillRect(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t color);

  /// Fills the screen buffer with white or black.
  void fillScreen(uint8_t color);

  /// Draws a rectangle with rounded edges.
  void drawRoundRect(int16_t x,
                     int16_t y,
                     uint8_t w,
                     uint8_t h,
                     uint8_t r,
                     uint8_t color);

  /// Draws a filled-in rectangle with rounded edges.
  void fillRoundRect(int16_t x,
                     int16_t y,
                     uint8_t w,
                     uint8_t h,
                     uint8_t r,
                     uint8_t color);

   /// Draws the outline of a triangle.
  void drawTriangle(int16_t x0,
                    int16_t y0,
                    int16_t x1,
                    int16_t y1,
                    int16_t x2,
                    int16_t y2,
                    uint8_t color);

  /// Draws a filled-in triangle.
  void fillTriangle (int16_t x0,
                     int16_t y0,
                     int16_t x1,
                     int16_t y1,
                     int16_t x2,
                     int16_t y2,
                     uint8_t color);

  /// Draws a bitmap from program memory to a specific X/Y
  void drawBitmap(int16_t x,
                  int16_t y,
                  const uint8_t *bitmap,
                  uint8_t w,
                  uint8_t h,
                  uint8_t color);

  /// Draws images that are bit-oriented horizontally.
  /**
   * This requires a lot of additional CPU power and will draw images slower
   * than drawBitmap, where the images are stored in a format that
   * allows them to be directly written to the screen. It is
   * recommended you use drawBitmap when possible.
   */
  void drawSlowXYBitmap(int16_t x,
                        int16_t y,
                        const uint8_t *bitmap,
                        uint8_t w,
                        uint8_t h,
                        uint8_t color);

  /// Draws an ASCII character at a point.
  void drawChar(int16_t x,
                int16_t y,
                unsigned char c,
                uint8_t color,
                uint8_t bg,
                uint8_t size);

  /// Get a pointer to the display buffer.
  unsigned char* getBuffer();

  /// Seeds the random number generator with entropy from the temperature, voltage reading, and microseconds since boot.
  /**
   * This method is still most effective when called semi-randomly such
   * as after a user hits a button to start a game or other semi-random
   * events
   */
  void initRandomSeed();

  /// Swap the references of two pointers.
  void swap(int16_t& a, int16_t& b);

  /// Sets the number of frames displayed per second to the screen.
  void setFrameRate(uint8_t rate);

  /// Returns 'true' if the system is ready to draw the next frame.
  bool nextFrame();

  /// Returns 'true' if the current frame number is evenly divisible by the
  /// passed integers
  /**
   * If called with the same value each time, will return "true" if the given
   * number of frames has elapsed since the last frame in which it was "true".
   */
  bool everyXFrames(uint8_t frames);

  /// Returns the load on the CPU as a percentage.
  /**
   * This is based on how much of the time your app is spends rendering
   * frames.  This number can be higher than 100 if your app is rendering
   * really slowly.
   */
  int cpuLoad();

  /// useful for getting raw approximate voltage values
  uint16_t rawADC(byte adc_bits);

  /// Provide system control during the boot sequence.
  /**
   * An opportunity to provide a level of system control is given during the
   * boot sequence. Control is given to systemSetup() if specified buttons 
   * are held during an Arduboy's startup.
   */
  void systemButtons();

protected:
  // helper to toggle sound from system control
  void sysCtrlSound(uint8_t buttons, uint8_t led, uint8_t eeVal);

  // screen buffer
  static uint8_t sBuffer[(HEIGHT * WIDTH) / 8];

  uint8_t frameRate;
  uint16_t frameCount;
  uint8_t eachFrameMillis;
  long lastFrameStart;
  long nextFrameStart;
  bool post_render;
  uint8_t lastFrameDurationMs;
};

class Arduboy : public Print, public ArduboyBase
{
public:
  Arduboy();

  /// Writes a single ASCII character to the screen.
  virtual size_t write(uint8_t);

  /// Sets the location of the text cursor.
  void setCursor(int16_t x, int16_t y);

  /// Get the text cursor X position
  uint16_t getCursorX();

  /// Get the text cursor Y position
  uint16_t getCursorY();

  /// Sets the text foreground color
  void setTextColor(uint8_t color);

  /// Sets the text background color
  void setTextBackground(uint8_t bg);

  /// Set the text size
  /**
   * Individual ASCII characters are 6x8 pixels
   * (5x7 with spacing on two edges). The size is a pixel multiplier,
   * so a size of 2 means each character will be 12x16, etc.
   */
  void setTextSize(uint8_t s);

  /// Sets whether text will wrap at screen edges.
  void setTextWrap(bool w);

  /// Clears the display and sets the cursor to 0, 0
  void clear();

protected:
  int16_t cursor_x;
  int16_t cursor_y;
  uint8_t textColor;
  uint8_t textBackground;
  uint8_t textSize;
  bool textWrap;
};

#endif
