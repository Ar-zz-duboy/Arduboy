#ifndef ArduboyCore_h
#define ArduboyCore_h

#include <avr/power.h>
#include <SPI.h>
#include <avr/sleep.h>
#include <limits.h>


// main hardware compile flags

// #define AB_DEVKIT    //< compile for the official dev kit
#define ARDUBOY_10   //< compile for the production Arduboy v1.0
#ifdef AB_DEVKIT     
#define DEVKIT       //< for compatibilty with older sketches
#define SAFE_MODE    //< compile in safe mode (44 bytes)
#endif 


#ifdef ARDUBOY_10

#define CS 12
#define DC 4
#define RST 6

#define RED_LED 10
#define GREEN_LED 11
#define BLUE_LED 9
#define TX_LED 30
#define RX_LED 17

// pin values for buttons, probably shouldn't use these
#define PIN_LEFT_BUTTON A2
#define PIN_RIGHT_BUTTON A1
#define PIN_UP_BUTTON A0
#define PIN_DOWN_BUTTON A3
#define PIN_A_BUTTON 7
#define PIN_B_BUTTON 8

// bit values for button states
#define LEFT_BUTTON _BV(5)
#define RIGHT_BUTTON _BV(6)
#define UP_BUTTON _BV(7)
#define DOWN_BUTTON _BV(4)
#define A_BUTTON _BV(3)
#define B_BUTTON _BV(2)

#define PIN_SPEAKER_1 5
#define PIN_SPEAKER_2 13

#elif defined(AB_DEVKIT)

#define CS 6
#define DC 4
#define RST 12

// map all LEDs to the single TX LED on DEVKIT
#define RED_LED 17
#define GREEN_LED 17
#define BLUE_LED 17
#define TX_LED 17
#define RX_LED 17

// pin values for buttons, probably shouldn't use these
#define PIN_LEFT_BUTTON 9
#define PIN_RIGHT_BUTTON 5
#define PIN_UP_BUTTON 8
#define PIN_DOWN_BUTTON 10
#define PIN_A_BUTTON A0
#define PIN_B_BUTTON A1

// bit values for button states
#define LEFT_BUTTON _BV(5)
#define RIGHT_BUTTON _BV(2)
#define UP_BUTTON _BV(4)
#define DOWN_BUTTON _BV(6)
#define A_BUTTON _BV(1)
#define B_BUTTON _BV(0)

#define PIN_SPEAKER_1 A2
#define PIN_SPEAKER_2 A3

#endif

#define COLUMN_ADDRESS_END (WIDTH - 1) & 0x7F   // 128 pixels wide
#define PAGE_ADDRESS_END ((HEIGHT/8)-1) & 0x07  // 8 pages high

#define WIDTH 128
#define HEIGHT 64

#define INVERT 2 //< lit/unlit pixel
#define WHITE 1 //< lit pixel
#define BLACK 0 //< unlit pixel

class ArduboyCore
{
public:
    ArduboyCore();

    /// starts up the hardware
    /**
     * setup() here is just a stub - your subclass should define setup
     * and within it call boot() as well as any other startup code your
     * subclass needs to do to start up
     */
    void setup();

    /// allows the CPU to idle between frames
    /**
     * This puts the CPU in "Idle" sleep mode.  You should call this as often
     * as you can for the best power savings.  The timer 0 overflow interrupt
     * will wake up the chip every 1ms - so even at 60 FPS a well written
     * app should be able to sleep maybe half the time in between rendering
     * it's own frames.
     * 
     * See the Arduboy class nextFrame() for an example of how to use idle()
     * in a frame loop.
     */
    void idle();

    void LCDDataMode(); //< put the display in data mode

    /// put the display in command mode
    /**
     * See SSD1306 documents for available commands and command sequences.
     * 
     * Links:
     * - https://www.adafruit.com/datasheets/SSD1306.pdf
     * - http://www.eimodule.com/download/SSD1306-OLED-Controller.pdf
     */
    void LCDCommandMode();

    uint8_t width();    //< return display width
    uint8_t height();   // < return display height

    /// get current state of buttons (bitmask)
    /**
     * Bit mask that is returned:
     * 
     *     High  Low
     *     00000000
     *     ||||||||
     *     |||||||`- A
     *     ||||||`-- B
     *     |||||`--- Right
     *     ||||`---- *reserved
     *     |||`----- Up
     *     ||`------ Left
     *     |`------- Down
     *     `-------- *reserved
     * 
     *     A is 1, Down is 64, etc.
     * 
     * Of course you shouldn't worry about bits (they may change with future
     * hardware) and should instead use the button defines:
     * LEFT_BUTTON, A_BUTTON, UP_BUTTON, etc.
     */

    uint8_t getInput();

    // paints 8 pixels (vertically) from a single byte
    //  - 1 is lit, 0 is unlit
    //
    // NOTE: You probably wouldn't actually use this, you'd build something
    // higher level that does it's own calls to SPI.transfer().  It's
    // included for completeness since it seems there should be some very
    // rudimentary low-level draw function in the core that supports the
    // minimum unit that the hardware allows (which is a strip of 8 pixels)
    //
    // This routine starts in the top left and then across the screen.
    // After each "page" (row) of 8 pixels is drawn it will shift down
    // to start drawing the next page.  To paint the full screen you call
    // this function 1,024 times.
    //
    // Example:
    //
    // X = painted pixels, . = unpainted
    //
    // blank()                      paint8Pixels() 0xFF, 0, 0x0F, 0, 0xF0
    // v TOP LEFT corner (8x9)      v TOP LEFT corner
    // ........ (page 1)            X...X... (page 1)
    // ........                     X...X...
    // ........                     X...X...
    // ........                     X...X...
    // ........                     X.X.....
    // ........                     X.X.....
    // ........                     X.X.....
    // ........ (end of page 1)     X.X..... (end of page 1)
    // ........ (page 2)            ........ (page 2)
    void paint8Pixels(uint8_t pixels);

    /// paints an entire image directly to hardware (from PROGMEM)
    /*
     * Each byte will be 8 vertical pixels, painted in the same order as
     * explained above in paint8Pixels.
     */
    void paintScreen(const unsigned char *image);

    /// paints an entire image directly to hardware (from RAM)
    /*
     * Each byte will be 8 vertical pixels, painted in the same order as
     * explained above in paint8Pixels.
     */
    void paintScreen(unsigned char image[]);

    /// paints a blank (black) screen to hardware
    void blank();

    /// invert the display or set to normal
    /**
     * when inverted, a pixel set to 0 will be on
     */
    void invert(boolean inverse);

    /// turn all display pixels on, or display the buffer contents
    /**
     * when set to all pixels on, the display buffer will be
     * ignored but not altered
     */
    void allPixelsOn(boolean on);

    /// flip the display vertically or set to normal
    void flipVertical(boolean flipped);

    /// flip the display horizontally or set to normal
    void flipHorizontal(boolean flipped);

    /// send a single byte command to the OLED
    void sendLCDCommand(uint8_t command);

    /// set the light output of the RGB LEB
    void setRGBled(uint8_t red, uint8_t green, uint8_t blue);


protected:
    /// boots the hardware
    /**
     * - sets input/output/pullup mode for pins
     * - powers up the OLED screen and initializes it properly
     * - sets up power saving
     * - kicks CPU down to 8Mhz if needed
     * - allows Safe mode to be entered
     */
    void boot();

    /// Safe mode
    /**
     * Safe Mode is engaged by holding down both the LEFT button and UP button
     * when plugging the device into USB.  It puts your device into a tight
     * loop and allows it to be reprogrammed even if you have uploaded a very
     * broken sketch that interferes with the normal USB triggered auto-reboot
     * functionality of the device.
     * 
     * This is most useful on Devkits because they lack a built-in reset
     * button.
     */
    void safeMode() __attribute__((always_inline));

    // internals
    void bootLCD() __attribute__((always_inline));
    void bootPins() __attribute__((always_inline));
    void slowCPU() __attribute__((always_inline));
    void saveMuchPower(); __attribute__((always_inline));


private:
    volatile uint8_t *mosiport, *clkport, *csport, *dcport;
    uint8_t mosipinmask, clkpinmask, cspinmask, dcpinmask;

};

#endif
