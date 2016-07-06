/**
 * \file Arduboy.h
 * \brief The Arduboy class definition.
 *
 */

#ifndef ARDUBOY_H
#define ARDUBOY_H

#include "ArduboyCore.h"
#include <Print.h>
#include <limits.h>

/// Library version.
/**
 * A version number, 'x.y.z', is stored in the form xxyyzz,
 * where ((x * 10000) + (y * 100) + (z)),
 * resulting in 'xxxyyzz', with no leading zeros.
 */
#define ARDUBOY_LIB_VER 10200 // 1.2.0

/**
 * EEPROM settings for audio
 * \defgroup EEPROM
 *
 * \{
 */

/// Define EEPROM version.
#define EEPROM_VERSION 0
/// Set EEPROM brightness.
#define EEPROM_BRIGHTNESS 1
/// Set EEPROM audio to off.
#define EEPROM_AUDIO_ON_OFF 2
/// Reserve the first 16 bytes of EEPROM for system use.
#define EEPROM_STORAGE_SPACE_START 16
/** \} */

#include "ArduboyAudio.h"

/**
 * Color settings
 * \defgroup COLOR
 *
 * \{
 */

/// Definition for PIXEL_SAFE_MODE.
#define PIXEL_SAFE_MODE

/// Value for inverting color.
#define INVERT 2 // on/off pixel
/// Value for white pixel (on).
#define WHITE  1 // pixel on
/// Value for black pixel (off).
#define BLACK  0 // pixel off
/** \} */

/**
 * \defgroup ADC_SETTINGS
 *
 * \{
 */
/// Compare Vcc to 1.1 bandgap.
#define ADC_VOLTAGE (_BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1))
/// Compare temperature to 2.5 internal reference and _BV(MUX5).
#define ADC_TEMP (_BV(REFS0) | _BV(REFS1) | _BV(MUX2) | _BV(MUX1) | _BV(MUX0))
/** \} */

class ArduboyBase : public ArduboyCore
{
public:
  /// Arduboy constructor
  /**
   * Constuctor for ArduboyBase sets up frame managment
   */
  ArduboyBase();

  /// implement audio
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
  /**
   * \fn begin
   * Look at the source for `begin()` and just rip out what you do not
   * need and start there.  Calling just `boot()` might work also
   * depending on your requirements.
   *
   * The minimum recommended `begin` replacement:
   *
   *   arduboy.boot()         // raw hardware init
   *   arduboy.audio.begin()  // if you need audio
   */
  void begin();

  /// Depreciated function. Use begin instead.
  void start() __attribute__((deprecated, warning("use begin() instead")));

  /// Scrolls in the Arduboy logo
  /*
   * \fn bootLogo
   * Scrolls the logo stored in memory for Arduboy down the screen. LEDs will
   * flash during the boot sequence.
   */
  void bootLogo();

  /// Clears display.
  /*
   * \fn clear
   * Clear the image buffer for the controlled Arduboy.
   */
  void clear();

  /// Provide flashlight mode, providing a default on and off button..
  /**
   * \fn flashlight
   * The flashlight mode will places the programmble LED on an Arduboy to white
   * and turn all of the pixels on the screen on. The default buttons provided
   * to check before turning on the flashlight are UP_BUTTON and DOWN_BUTTON
   * for on and off, respectivly.
   */
  void flashlight();

  /// Flashlight mode, provide off button only.
  /**
   * \param off_button uint8_t button to press to turn off flashlight.
   */
  void flashlight(uint8_t off_button);

  /// Flashlight mode, provide on and off button.
  /**
   * \fn flashlight
   * \param on_button uint8_t button to check before turning on flashlight.
   * \param off_button uint8_t button to press to turn off flashlight.
   * Hold a key when booting to enable, press a provided key to exit;
   * or simply turn off your Arduboy.  Your sketches can also
   * call this at any time.  It goes into a tight loop until the
   * off_button is pressed.
   */
  void flashlight(uint8_t on_button, uint8_t off_button);

  /// Deprecitated function to clear an Arduboy display. Use clear() instead.
  void clearDisplay() 
      __attribute__((deprecated, warning("use clear() instead")));

  /// Copies the contents of the screen buffer to the screen.
  /**
   * \fn display
   * X and Y positions on the display are from the top left corner, thus a Y of
   * 64 is the bottom of the screen and an X of 128 is the right side of the
   * screen. "Color" or "value" means choosing whether a pixel is lit or not - 
   * if color is 0, the pixel is off (black), if color is 1, the pixel is on
   * (white).
   */
  void display();

  /// Managed draw function for an Arduboy.
  /**
   * \fn draw
   * \param *f a pointer to a function to be called.
   * \return Returns a uint8_t for status. 0 for no errors.
   * The managed draw function for Arduboy. Checks if frame is ready to be
   * drawn, returns if not. The function passed a parameter is executed along
   * with any cleanup routine.
   */
  uint8_t draw(void (*f)());

  /// Sets a pixel in the screen buffer to on or off. Default is on (white).
  /**
   * \fn drawPixel
   * \param x integer for an x position
   * \param y integer for a y position
   * \return void return
   */
  void drawPixel(int x, int y, uint8_t color);

  /// Get the value of a pixel from the screen buffer at a specified x and y 
  /// coordinate.
  /**
   * \fn getPixel
   * \param x int for an x position to draw at.
   * \param y int for an y position to draw at.
   * \param color a uint8_t for a color.
   *
   */
  uint8_t getPixel(uint8_t x, uint8_t y);

  /// Draw a circle of a defined radius.
  /**
   * \fn drawCircle
   * \param x0
   * \param y0
   * \param r
   * \param color
   * Draws a circle in white or black. X and Y are the center point of the
   * circle.
   */
  void drawCircle(int16_t x0, int16_t y0, uint8_t r, uint8_t color);

  /// Draws one or more "corners" of a circle.
  /**
   * \fn drawCircleHelper
   * \param x0
   * \param y0
   * \param r
   * \param cornername
   * \param color
   * Helper function to draw the corners of a circle at a given x and y origin.
   */
  void drawCircleHelper(int16_t x0,
                        int16_t y0,
                        uint8_t r,
                        uint8_t cornername,
                        uint8_t color);

  /// Draws a filled-in circle.
  /**
   * \fn fillCircle
   * \param x0
   * \param y0
   * \param r
   * \param color
   *
   */
  void fillCircle(int16_t x0, int16_t y0, uint8_t r, uint8_t color);

   /// Draws one or both vertical halves of a filled-in circle.
   /**
    * \fn fillCircleHelper
    * \param x0
    * \param y0
    * \param r
    * \param cornername
    * \param delta
    * \param color
    */
  void fillCircleHelper(int16_t x0,
                        int16_t y0,
                        uint8_t r,
                        uint8_t cornername,
                        int16_t delta,
                        uint8_t color);

  /// Draws a line between two points.
  /**
   * \fn drawLine
   * \param x0
   * \param y0
   * \param x1
   * \param y1
   * \param color
   * Uses Bresenham's algorithm.
   */
  void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t color);

  /**
   * \fn drawRect
   * \param x
   * \param y
   * \param w
   * \param h
   * \param color
   */
  /// Draws a rectangle of a width and height.
  void drawRect(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t color);

  /**
   * \fn drawFastVLine
   * \param x
   * \param y
   * \param h
   * \param color
   */
  /// Draws vertical line.
  void drawFastVLine(int16_t x, int16_t y, uint8_t h, uint8_t color);

  /**
   * \fn drawFastHLine
   * \param x
   * \param y
   * \param w
   * \param color
   */
  /// Draws a horizontal line.
  void drawFastHLine(int16_t x, int16_t y, uint8_t w, uint8_t color);

  /**
   * \fn fillRect
   * \param x
   * \param y
   * \param w
   * \param h
   * \param color
   */
  /// Draws a filled-in rectangle.
  void fillRect(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t color);

  /**
   * \fn fillScreen
   * \param color
   */
  /// Fills the screen buffer with white or black.
  void fillScreen(uint8_t color);

  /// Draws a rectangle with rounded edges.
  /**
   * \fn drawRoundRect
   * \param x
   * \param y
   * \param w
   * \param h
   * \param r
   * \param color
   */
  void drawRoundRect(int16_t x,
                     int16_t y,
                     uint8_t w,
                     uint8_t h,
                     uint8_t r,
                     uint8_t color);

  /// Draws a filled-in rectangle with rounded edges.
  /**
   * \fn fillRoundRect
   * \param x
   * \param y
   * \param w
   * \param h
   * \param r
   * \param color
   */
  void fillRoundRect(int16_t x,
                     int16_t y,
                     uint8_t w,
                     uint8_t h,
                     uint8_t r,
                     uint8_t color);

   /// Draws the outline of a triangle.
  /**
   * \fn drawTriangle
   * \param x0
   * \param y0
   * \param x1
   * \param y1
   * \param x2
   * \param y2
   * \param color
   */
  void drawTriangle(int16_t x0,
                    int16_t y0,
                    int16_t x1,
                    int16_t y1,
                    int16_t x2,
                    int16_t y2,
                    uint8_t color);

  /// Draws a filled-in triangle.
  /**
   * \fn drawTriangle
   * \param x0
   * \param y0
   * \param x1
   * \param y1
   * \param x2
   * \param y2
   * \param color
   */
  void fillTriangle (int16_t x0,
                     int16_t y0,
                     int16_t x1,
                     int16_t y1,
                     int16_t x2,
                     int16_t y2,
                     uint8_t color);

  /// Draws a bitmap from program memory to a specific X/Y
  /**
   * \fn drawBitmap
   * \param x
   * \param y
   * \param *bitmap
   * \param w
   * \param h
   * \param color
   */
  void drawBitmap(int16_t x,
                  int16_t y,
                  const uint8_t *bitmap,
                  uint8_t w,
                  uint8_t h,
                  uint8_t color);

  /// Draws images that are bit-oriented horizontally.
  /**
   * \fn drawSlowXYBitmap
   * \param x
   * \param y
   * \param *bitmap
   * \param w
   * \param h
   * \param color
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
  /**
   * \fn drawChar
   * \param x
   * \param y
   * \param c
   * \param color
   * \param bg
   * \param size
   *
   */
  void drawChar(int16_t x,
                int16_t y,
                unsigned char c,
                uint8_t color,
                uint8_t bg,
                uint8_t size);

  /// Get a pointer to the display buffer.
  /**
   * \fn getBuffer
   * \return Returns the basepointer to the Arduboy image buffer array 
   * sBuffer[].
   */
  unsigned char* getBuffer();

  /// Seeds the random number generator with entropy from the temperature, voltage reading, and microseconds since boot.
  /**
   * \fn initRandomSeed
   * This method is still most effective when called semi-randomly such
   * as after a user hits a button to start a game or other semi-random
   * events
   */
  void initRandomSeed();

  /// Swap the references of two pointers.
  /**
   * \fn swap
   * \param reference to an int16_t to swap with b
   * \param reference to int16_t to swap with a
   */
  void swap(int16_t &a, int16_t &b);

  /// Sets the number of frames displayed per second to the screen.
  /**
   * \fn setFrameRate
   * \param rate Rate to draw frames.
   */
  void setFrameRate(uint8_t rate);

  /// Returns 'true' if the system is ready to draw the next frame.
  /**
   * \fn nextFrame
   *
   */
  bool nextFrame();

  /// Returns 'true' if the current frame number is evenly divisible by the
  /// passed integers
  /**
   * \fn everyXFrames
   * \param frames
   * If called with the same value each time, will return "true" if the given
   * number of frames has elapsed since the last frame in which it was "true".
   */
  bool everyXFrames(uint8_t frames);

  /// Returns the load on the CPU as a percentage.
  /**
   * \fn cpuLoad
   * This is based on how much of the time your app is spends rendering
   * frames.  This number can be higher than 100 if your app is rendering
   * really slowly.
   */
  int cpuLoad();

  /// useful for getting raw approximate voltage values
  /**
   * \fn rawADC
   * \param adc_bits byte to specificy adc bits
   */
  uint16_t rawADC(byte adc_bits);

  /// Provide system control during the boot sequence.
  /**
   * \fn systemButtons
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

  /// Framerate to update image buffer at.
  uint8_t frameRate;

  /// Current count of frames.
  uint16_t frameCount;

  /// Target time for each frame in ms.
  uint8_t eachFrameMillis;

  /// Time of the frame last started.
  long lastFrameStart;

  /// Time to start next frame.
  long nextFrameStart;

  /// Flag to enable or disable post render processes.
  bool post_render;

  /// Time elapsed between current and last frame.
  uint8_t lastFrameDurationMs;
};

class Arduboy : public Print, public ArduboyBase
{
public:
  /// Arduboy constructor.
  Arduboy();

  /// Writes a single ASCII character to the screen.
  /*
   * \fn write
   * \param char Takes character to write.
   * \return returns size_t.
   */
  virtual size_t write(uint8_t);

  /// Sets the location of the text cursor.
  /*
   * \fn setCursore
   * \param x
   * \param y
   *
   */
  void setCursor(int16_t x, int16_t y);

  /// Get the text cursor X position
  /*
   * \fn
   * \param x int16_t for x position.
   * \param x int16_t for y position.
   *
   */
  uint16_t getCursorX();

  /// Get the text cursor Y position
  /*
   * \fn getCursorX
   *
   */
  uint16_t getCursorY();

  /// Sets the text foreground color
  /**
   * \fn setTextColor
   * \param color Pass a usigned byte to set as color.
   */
  void setTextColor(uint8_t color);

  /// Sets the text background color
  void setTextBackground(uint8_t bg);

  /// Set the text size
  /**
   * \fn setTextSize
   * \param s Size of text passed as uint8_t.
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
