/**
 * @file Arduboy.h
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

/// Define EEPROM version.
#define EEPROM_VERSION 0
/// Set EEPROM brightness.
#define EEPROM_BRIGHTNESS 1
/// Set EEPROM audio to off.
#define EEPROM_AUDIO_ON_OFF 2
/// Reserve the first 16 bytes of EEPROM for system use.
#define EEPROM_STORAGE_SPACE_START 16

#include "ArduboyAudio.h"

/// Definition for PIXEL_SAFE_MODE.
#define PIXEL_SAFE_MODE

/// Value for inverting color.
#define INVERT 2 // on/off pixel
/// Value for white pixel (on).
#define WHITE  1 // pixel on
/// Value for black pixel (off).
#define BLACK  0 // pixel off

/// Compare Vcc to 1.1 bandgap.
#define ADC_VOLTAGE (_BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1))
/// Compare temperature to 2.5 internal reference and _BV(MUX5).
#define ADC_TEMP (_BV(REFS0) | _BV(REFS1) | _BV(MUX2) | _BV(MUX1) | _BV(MUX0))

class ArduboyBase : public ArduboyCore
{
public:
  /**
   * ArduboyBase constructor
   * \details
   * Constuctor for ArduboyBase. Frame management is initialized.
   */
  ArduboyBase();

  /**
   * Implement audio for an Arduboy.
   * \details Class used to offer a general API for creating audio on an Arduboy
   */
  ArduboyAudio audio;

  /**
   * Returns true if buttons in button_mask are pressed.
   * \param buttons uint8_t argument for a button mask.
   * \return Returns true if all button in button mask are pressed.
   * \details
   * example:
   *   if (pressed(LEFT_BUTTON + A_BUTTON))
   *
   */
  bool pressed(uint8_t buttons);

  /**
   * Returns true if the button mask passed in not pressed.
   * \param buttons uint8_t argument for a button mask.
   * \return Returns true if every button is not pressed.
   * \see pressed()
   * \details
   * example:
   *  if (notPressed(LEFT_BUTTON))
   *
   */
  bool notPressed(uint8_t buttons);

  /**
   * Initialize hardware, boot logo, boot utilities, etc.
   * \details
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

  /**
   * Flashlight mode handler.
   * \details
   * Flashlight mode is a boot up function that checks if the UP button is
   * being held and, if so, turns the RGB LED on full white and lights all the
   * screen pixels. When the DOWN button is pressed, flashlight mode exits and
   * the sketch continues.
   */
  void flashlight();

  /**
   * Provide system control during the boot sequence.
   * \details
   * An opportunity to provide a level of system control is given during the
   * boot sequence. Control is given to systemSetup() if specified buttons 
   * are held during an Arduboy's startup.
   */
  void systemButtons();

  /**
   * Scrolls in the Arduboy logo
   * \details
   * Scrolls the logo stored in memory for Arduboy down the screen. LEDs will
   * flash during the boot sequence.
   */
  void bootLogo();

  /**
   * Do the same as begin() except don't display the boot logo sequence or
   * handle system control buttons.
   * \deprecated This function has been retained for backwards compatibility.
   * It should not be used for new development. Instead, use boot() and
   * optionally add functions back in that begin() performs.
   */
  void beginNoLogo()
      __attribute__((deprecated, warning("use boot() plus optional extra functions instead")));

  /**
   * Clears display.
   * \details
   * Clear the image buffer for the controlled Arduboy.
   */
  void clear();

  /// Deprecated function to clear an Arduboy display. Use clear() instead.
  void clearDisplay() 
      __attribute__((deprecated, warning("use clear() instead")));

  /**
   * Copies the contents of the screen buffer to the screen.
   * \details
   * X and Y positions on the display are from the top left corner, thus a Y of
   * 64 is the bottom of the screen and an X of 128 is the right side of the
   * screen. "Color" or "value" means choosing whether a pixel is lit or not - 
   * if color is 0, the pixel is off (black), if color is 1, the pixel is on
   * (white).
   */
  void display();

  /**
   * Managed draw function for an Arduboy.
   * \todo Thoroughly test this method for handling draws to the screen.
   * \param *f a pointer to a function to be called.
   * \return Returns a uint8_t for status. 0 for no errors.
   * \details
   * The managed draw function for Arduboy. Checks if frame is ready to be
   * drawn, returns if not. The function passed a parameter is executed along
   * with any cleanup routine.
   */
  uint8_t draw(void (*f)());

  /**
   * Sets a pixel in the screen buffer to on or off. Default is on (white).
   * \param x integer for an x position
   * \param y integer for a y position
   * \param color Default color for pixel is set to white.
   */
  void drawPixel(int x, int y, uint8_t color = WHITE);

  /**
   * Get the value of a pixel from the screen buffer at a specified x and y 
   * coordinate.
   * \param x int for an x position to draw at.
   * \param y int for an y position to draw at.
   * \return Pixel value as unsigned byte.
   */
  uint8_t getPixel(uint8_t x, uint8_t y);

  /**
   * Draw a circle of a defined radius.
   * \param x0
   * \param y0
   * \param r
   * \param color Optional, default = WHITE
   * \details
   * Draws a circle in white or black. X and Y are the center point of the
   * circle.
   */
  void drawCircle(int16_t x0, int16_t y0, uint8_t r, uint8_t color = WHITE);

  /**
   * Draws one or more "corners" of a circle.
   * \param x0
   * \param y0
   * \param r
   * \param cornername
   * \param color Optional, default = WHITE
   * \details
   * Helper function to draw the corners of a circle at a given x and y origin.
   */
  void drawCircleHelper(int16_t x0,
                        int16_t y0,
                        uint8_t r,
                        uint8_t cornername,
                        uint8_t color = WHITE);

  /**
   * Draws a filled-in circle.
   * \param x0
   * \param y0
   * \param r
   * \param color Optional, default = WHITE
   */
  void fillCircle(int16_t x0, int16_t y0, uint8_t r, uint8_t color = WHITE);

   /**
    * Draws one or both vertical halves of a filled-in circle.
    * \param x0
    * \param y0
    * \param r
    * \param cornername
    * \param delta
    * \param color Optional, default = WHITE
    */
  void fillCircleHelper(int16_t x0,
                        int16_t y0,
                        uint8_t r,
                        uint8_t cornername,
                        int16_t delta,
                        uint8_t color = WHITE);

  /**
   * Draws a line between two points.
   * \param x0
   * \param y0
   * \param x1
   * \param y1
   * \param color Optional, default = WHITE
   * \details Uses Bresenham's algorithm.
   */
  void drawLine(int16_t x0,
                int16_t y0,
                int16_t x1,
                int16_t y1,
                uint8_t color = WHITE);

  /**
   * Draws a rectangle of a width and height.
   * \param x
   * \param y
   * \param w
   * \param h
   * \param color Optional, default = WHITE
   */
  void drawRect(int16_t x,
                int16_t y,
                uint8_t w,
                uint8_t h,
                uint8_t color = WHITE);

  /**
   * Draws vertical line.
   * \param x
   * \param y
   * \param h
   * \param color Optional, default = WHITE
   */
  void drawFastVLine(int16_t x, int16_t y, uint8_t h, uint8_t color = WHITE);

  /**
   * Draws a horizontal line.
   * \param x
   * \param y
   * \param w
   * \param color
   */
  void drawFastHLine(int16_t x, int16_t y, uint8_t w, uint8_t color = WHITE);

  /**
   * Draws a filled-in rectangle.
   * \param x
   * \param y
   * \param w
   * \param h
   * \param color Optional, default = WHITE
   */
  void fillRect(int16_t x,
                int16_t y,
                uint8_t w,
                uint8_t h,
                uint8_t color = WHITE);

  /**
   * Fills the screen buffer with white or black.
   * \fn fillScreen
   * \param color Optional, default = WHITE
   */
  void fillScreen(uint8_t color = WHITE);

  /**
   * Draws a rectangle with rounded edges.
   * \param x
   * \param y
   * \param w
   * \param h
   * \param r
   * \param color Optional, default = WHITE
   */
  void drawRoundRect(int16_t x,
                     int16_t y,
                     uint8_t w,
                     uint8_t h,
                     uint8_t r,
                     uint8_t color = WHITE);

  /**
   * Draws a filled-in rectangle with rounded edges.
   * \param x
   * \param y
   * \param w
   * \param h
   * \param r
   * \param color Optional, default = WHITE
   */
  void fillRoundRect(int16_t x,
                     int16_t y,
                     uint8_t w,
                     uint8_t h,
                     uint8_t r,
                     uint8_t color = WHITE);

  /**
   * Draws the outline of a triangle.
   * \param x0
   * \param y0
   * \param x1
   * \param y1
   * \param x2
   * \param y2
   * \param color Optional, default = WHITE
   */
  void drawTriangle(int16_t x0,
                    int16_t y0,
                    int16_t x1,
                    int16_t y1,
                    int16_t x2,
                    int16_t y2,
                    uint8_t color = WHITE);

  /**
   * Draws a filled-in triangle.
   * \param x0
   * \param y0
   * \param x1
   * \param y1
   * \param x2
   * \param y2
   * \param color Optional, default = WHITE
   */
  void fillTriangle (int16_t x0,
                     int16_t y0,
                     int16_t x1,
                     int16_t y1,
                     int16_t x2,
                     int16_t y2,
                     uint8_t color = WHITE);

  /**
   * Draws a bitmap from program memory to a specific X/Y
   * \param x
   * \param y
   * \param *bitmap
   * \param w
   * \param h
   * \param color Optional, default = WHITE
   */
  void drawBitmap(int16_t x,
                  int16_t y,
                  const uint8_t *bitmap,
                  uint8_t w,
                  uint8_t h,
                  uint8_t color = WHITE);

  /**
   * Draws images that are bit-oriented horizontally.
   * \param x
   * \param y
   * \param *bitmap
   * \param w
   * \param h
   * \param color Optional, default = WHITE
   * \details
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
                        uint8_t color = WHITE);

  /**
   * Draws an ASCII character at a point.
   * \param x
   * \param y
   * \param c
   * \param color Optional, default = WHITE
   * \param bg Optional, default = BLACK
   * \param size Optional, default = 1
   */
  void drawChar(int16_t x,
                int16_t y,
                uint8_t c,
                uint8_t color = WHITE,
                uint8_t bg = BLACK,
                uint8_t size = 1);

  /**
   * Get a pointer to the display buffer.
   * \return Returns the basepointer to the Arduboy image buffer array 
   * sBuffer[].
   */
  uint8_t* getBuffer();

  /**
   * Seeds the random number generator with entropy from the temperature, 
   * voltage reading, and microseconds since boot.
   * \details
   * This method is still most effective when called semi-randomly such
   * as after a user hits a button to start a game or other semi-random
   * events.
   */
  void initRandomSeed();

  /**
   * Swap the references of two pointers.
   * \param &a reference to an int16_t to swap with b
   * \param &b reference to int16_t to swap with a
   */
  void swap(int16_t &a, int16_t &b);

  /**
   * Sets the number of frames displayed per second to the screen.
   * \param rate Rate to draw frames.
   */
  void setFrameRate(uint8_t rate);

  /**
   * Returns 'true' if the desired time to draw a new frame has elapsed.
   * The time period is set using setFrameRate().
   * \return Returns true if it's time to draw a new frame.
   */
  bool newFrame();

  /**
   * Returns 'true' if it's time to draw the next frame.
   * \deprecated This functon has a bug which can result in the frame rate
   * being slower than what is set, and vary depending on CPU load. It has been
   * retained so that older sketches using it will continue to run at the same
   * speed. New sketches should use newFrame(). It is recommended that sketches
   * already using nextFrame() be modified to use newFrame() if possible.
   * \return Returns true if it's time to draw the next frame.
   */
  bool nextFrame()
      __attribute__((deprecated, warning("consider using newFrame() instead")));

  /**
   * Returns true if the current frame number is evenly divisible by the
   * \param frames
   * \return returns true if frames divides the current frame count.
   * \details
   * If called with the same value each time, then this function will return 
   * true if the same number of 'frames' has elapsed since the last frame. 
   */
  bool everyXFrames(uint8_t frames);

  /**
   * Returns the load on the CPU as a percentage.
   * \todo can this return a uint_8 of 100?
   * \return return an int representing the percentage of cpu use.
   * \details
   * This is based on how much of the time your app is spends rendering
   * frames.  This number can be higher than 100 if your app is rendering
   * really slowly.
   */
  int cpuLoad();

  /**
   * useful for getting raw approximate voltage values
   * \param adc_bits byte to specificy adc bits
   * \return Return ADC as an unsigned 16 bit integer.
   */
  uint16_t rawADC(uint8_t adc_bits);

protected:

  /// Helper to toggle sound from system control.
  void sysCtrlSound(uint8_t buttons, uint8_t led, uint8_t eeVal);

  /// The Arduboy screen buffer.
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
