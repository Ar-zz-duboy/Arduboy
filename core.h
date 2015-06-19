#ifndef ArduboyCore_h
#define ArduboyCore_h

#include <avr/power.h>
#include <SPI.h>
#include <avr/sleep.h>
#include <limits.h>

#define SAFE_MODE
#define DEVKIT

#define CS 6
#define DC 4
#define RST 12

#define PIN_LEFT_BUTTON 9
#define PIN_RIGHT_BUTTON 5
#define PIN_UP_BUTTON 8
#define PIN_DOWN_BUTTON 10
#define PIN_A_BUTTON A0
#define PIN_B_BUTTON A1

// button states
#define LEFT_BUTTON _BV(5)
#define RIGHT_BUTTON _BV(2)
#define UP_BUTTON _BV(4)
#define DOWN_BUTTON _BV(6)
#define A_BUTTON _BV(1)
#define B_BUTTON _BV(0)

#define COLUMN_ADDRESS_END (WIDTH - 1) & 0x7F
#define PAGE_ADDRESS_END ((HEIGHT/8)-1) & 0x07

#define WIDTH 128
#define HEIGHT 64

#define WHITE 1
#define BLACK 0

class ArduboyCore
{
public:
    ArduboyCore();
    void setup();
    void idle();

    void LCDDataMode();
    void LCDCommandMode();

    uint8_t width();    //< return display width
    uint8_t height();   // < return display height

    /// get current state of buttons (bitmask)
    /**
    Byte value returned:

    00000000
    ||||||||
    |||||||`- A Button
    ||||||`-- B
    |||||`--- Right
    ||||`---- *reserved
    |||`----- Up
    ||`------ Left
    |`------- Down
    `-------- *reserved

    Of course you shouldn't worry about this and should instead use the
    button defines: LEFT_BUTTON, A_BUTTON, UP_BUTTON, etc.
    **/

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
    void paintScreen(const unsigned char *image);
    /// paints an entire image directly to hardware (from RAM)
    void paintScreen(unsigned char image[]);
    /// paints a blank screen to hardware
    void blank();


protected:
    void boot();
    void bootLCD() __attribute__((always_inline));
    void bootPins() __attribute__((always_inline));
    void safeMode() __attribute__((always_inline));
    void slowCPU() __attribute__((always_inline));
    void saveMuchPower(); __attribute__((always_inline));

private:
    volatile uint8_t *mosiport, *clkport, *csport, *dcport;
    uint8_t mosipinmask, clkpinmask, cspinmask, dcpinmask;


};

#endif
