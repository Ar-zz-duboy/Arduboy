/** 
 * @file ArduboyCore.cpp
 * \brief A class implementing the core functionality of an Arduboy.
 *
 */

#include "ArduboyCore.h"

// need to redeclare these here since we declare them static in .h
volatile uint8_t *ArduboyCore::csport, *ArduboyCore::dcport;
uint8_t ArduboyCore::cspinmask, ArduboyCore::dcpinmask;

const uint8_t PROGMEM pinBootProgram[] = {
  // buttons
  PIN_LEFT_BUTTON, INPUT_PULLUP,
  PIN_RIGHT_BUTTON, INPUT_PULLUP,
  PIN_UP_BUTTON, INPUT_PULLUP,
  PIN_DOWN_BUTTON, INPUT_PULLUP,
  PIN_A_BUTTON, INPUT_PULLUP,
  PIN_B_BUTTON, INPUT_PULLUP,

  // RGB LED (or single blue LED on the DevKit)
#ifdef ARDUBOY_10
  RED_LED, INPUT_PULLUP,  // set INPUT_PULLUP to make the pin high when
  RED_LED, OUTPUT,        //   set to OUTPUT
  GREEN_LED, INPUT_PULLUP,
  GREEN_LED, OUTPUT,
#endif
  BLUE_LED, INPUT_PULLUP,
  BLUE_LED, OUTPUT,

  // audio is specifically not included here as those pins are handled
  // separately by `audio.begin()`, `audio.on()` and `audio.off()` in order
  // to respect the EEPROM audio settings

  // OLED SPI
  DC, OUTPUT,
  CS, OUTPUT,
  RST, OUTPUT,
  0
};

const uint8_t PROGMEM lcdBootProgram[] = {
  // boot defaults are commented out but left here incase they
  // might prove useful for reference
  //
  // Further reading: https://www.adafruit.com/datasheets/SSD1306.pdf
  //
  // Display Off
  // 0xAE,

  // Set Display Clock Divisor v = 0xF0
  // default is 0x80
  0xD5, 0xF0,

  // Set Multiplex Ratio v = 0x3F
  // 0xA8, 0x3F,

  // Set Display Offset v = 0
  // 0xD3, 0x00,

  // Set Start Line (0)
  // 0x40,

  // Charge Pump Setting v = enable (0x14)
  // default is disabled
  0x8D, 0x14,

  // Set Segment Re-map (A0) | (b0001)
  // default is (b0000)
  0xA1,

  // Set COM Output Scan Direction
  0xC8,

  // Set COM Pins v
  // 0xDA, 0x12,

  // Set Contrast v = 0xCF
  0x81, 0xCF,

  // Set Precharge = 0xF1
  0xD9, 0xF1,

  // Set VCom Detect
  // 0xDB, 0x40,

  // Entire Display ON
  // 0xA4,

  // Set normal/inverse display
  // 0xA6,

  // Display On
  0xAF,

  // set display mode = horizontal addressing mode (0x00)
  0x20, 0x00,

  // set col address range
  // 0x21, 0x00, COLUMN_ADDRESS_END,

  // set page address range
  // 0x22, 0x00, PAGE_ADDRESS_END
};

ArduboyCore::ArduboyCore() {}

void ArduboyCore::boot()
{
  #ifdef ARDUBOY_SET_CPU_8MHZ
  // ARDUBOY_SET_CPU_8MHZ will be set by the IDE using boards.txt
  setCPUSpeed8MHz();
  #endif

  SPI.begin();
  bootPins();
  bootOLED();

  #ifdef SAFE_MODE
  if (buttonsState() == (LEFT_BUTTON | UP_BUTTON))
    safeMode();
  #endif

  bootPowerSaving();
}

#ifdef ARDUBOY_SET_CPU_8MHZ
// If we're compiling for 8MHz we need to slow the CPU down because the
// hardware clock on the Arduboy is 16MHz.
// We also need to readjust the PLL prescaler because the Arduino USB code
// likely will have incorrectly set it for an 8MHz hardware clock.
void ArduboyCore::setCPUSpeed8MHz()
{
  uint8_t oldSREG = SREG;
  cli();                // suspend interrupts
  PLLCSR = _BV(PINDIV); // dissable the PLL and set prescale for 16MHz)
  CLKPR = _BV(CLKPCE);  // allow reprogramming clock
  CLKPR = 1;            // set clock divisor to 2 (0b0001)
  PLLCSR = _BV(PLLE) | _BV(PINDIV); // enable the PLL (with 16MHz prescale)
  SREG = oldSREG;       // restore interrupts
}
#endif

void ArduboyCore::bootPins()
{
  uint8_t pin, mode;
  const uint8_t *i = pinBootProgram;

  while (true)
  {
    pin = pgm_read_byte(i++);
    mode = pgm_read_byte(i++);
    if (pin==0) break;
    pinMode(pin, mode);
  }

  digitalWrite(RST, HIGH);
  delay(1);           // VDD (3.3V) goes high at start, lets just chill for a ms
  digitalWrite(RST, LOW);   // bring reset low
  delay(10);          // wait 10ms
  digitalWrite(RST, HIGH);  // bring out of reset
}

void ArduboyCore::bootOLED()
{
  // setup the ports we need to talk to the OLED
  csport = portOutputRegister(digitalPinToPort(CS));
  cspinmask = digitalPinToBitMask(CS);
  dcport = portOutputRegister(digitalPinToPort(DC));
  dcpinmask = digitalPinToBitMask(DC);

  SPI.setClockDivider(SPI_CLOCK_DIV2);

  LCDCommandMode();
  // run our customized boot-up command sequence against the
  // OLED to initialize it properly for Arduboy
  for (int8_t i = 0; i < sizeof(lcdBootProgram); i++)
    SPI.transfer(pgm_read_byte(lcdBootProgram + i));

  LCDDataMode();
}

void ArduboyCore::LCDDataMode()
{
  *dcport |= dcpinmask;
  *csport &= ~cspinmask;
}

void ArduboyCore::LCDCommandMode()
{
  *csport |= cspinmask;
  *dcport &= ~dcpinmask;
  *csport &= ~cspinmask;
}

void ArduboyCore::safeMode()
{
  blank(); // clear screen to avoid writing random bytes in image buffer
  while (true) {
    asm volatile("nop \n");
  }
}

/* Power Management */

void ArduboyCore::idle()
{
  set_sleep_mode(SLEEP_MODE_IDLE);
  sleep_mode();
}

void ArduboyCore::bootPowerSaving()
{
  power_adc_disable();
  power_usart0_disable();
  power_twi_disable();
  // timer 0 is for millis()
  // timers 1 and 3 are for music and sounds
  power_timer2_disable();
  power_usart1_disable();
  // we need USB, for now (to allow triggered reboots to reprogram)
  // power_usb_disable()
}

uint8_t ArduboyCore::width() { return WIDTH; }

uint8_t ArduboyCore::height() { return HEIGHT; }

/* Drawing */

void ArduboyCore::paint8Pixels(uint8_t pixels)
{
  SPI.transfer(pixels);
}

void ArduboyCore::paintScreen(const uint8_t *image)
{
  for (int i = 0; i < (HEIGHT*WIDTH)/8; i++)
    SPI.transfer(pgm_read_byte(image + i));
}

// paint from a memory buffer, this should be FAST as it's likely what
// will be used by any buffer based subclass
void ArduboyCore::paintScreen(uint8_t image[])
{
  for (int i = 0; i < (HEIGHT * WIDTH) / 8; i++)
  {
    // SPI.transfer(image[i]);

    // we need to burn 18 cycles between sets of SPDR
    // 4 clock cycles
    SPDR = image[i];
    // 7 clock cycles
    asm volatile(
      "mul __zero_reg__, __zero_reg__ \n" // 2 cycles
      "mul __zero_reg__, __zero_reg__ \n" // 2 cycles
      "mul __zero_reg__, __zero_reg__ \n" // 2 cycles
      );
  }
}

void ArduboyCore::blank()
{
  for (int i = 0; i < (HEIGHT * WIDTH) / 8; i++)
    SPI.transfer(0x00);
}

void ArduboyCore::sendLCDCommand(uint8_t command)
{
  LCDCommandMode();
  SPI.transfer(command);
  LCDDataMode();
}

// invert the display or set to normal
// when inverted, a pixel set to 0 will be on
void ArduboyCore::invert(bool inverse)
{
  sendLCDCommand(inverse ? OLED_PIXELS_INVERTED : OLED_PIXELS_NORMAL);
}

// turn all display pixels on, ignoring buffer contents
// or set to normal buffer display
void ArduboyCore::allPixelsOn(bool on)
{
  sendLCDCommand(on ? OLED_ALL_PIXELS_ON : OLED_PIXELS_FROM_RAM);
}

// flip the display vertically or set to normal
void ArduboyCore::flipVertical(bool flipped)
{
  sendLCDCommand(flipped ? OLED_VERTICAL_FLIPPED : OLED_VERTICAL_NORMAL);
}

// flip the display horizontally or set to normal
void ArduboyCore::flipHorizontal(bool flipped)
{
  sendLCDCommand(flipped ? OLED_HORIZ_FLIPPED : OLED_HORIZ_NORMAL);
}

/* RGB LED */

void ArduboyCore::setRGBled(uint8_t red, uint8_t green, uint8_t blue)
{
#ifdef ARDUBOY_10 // RGB, all the pretty colors
  // inversion is necessary because these are common annode LEDs
  analogWrite(RED_LED, 255 - red);
  analogWrite(GREEN_LED, 255 - green);
  analogWrite(BLUE_LED, 255 - blue);
#elif defined(AB_DEVKIT)
  // only blue on devkit
  digitalWrite(BLUE_LED, ~blue);
#endif
}

void ArduboyCore::digitalWriteRGB(uint8_t red, uint8_t green, uint8_t blue)
{
#ifdef ARDUBOY_10
  digitalWrite(RED_LED, red);
  digitalWrite(GREEN_LED, green);
  digitalWrite(BLUE_LED, blue);
#elif defined(AB_DEVKIT)
  digitalWrite(BLUE_LED, blue);
#endif
}

/* Buttons */

uint8_t ArduboyCore::buttonsState()
{
  uint8_t buttons;

  // using ports here is ~100 bytes smaller than digitalRead()
#ifdef AB_DEVKIT
  // down, left, up
  buttons = ((~PINB) & B01110000);
  // right button
  buttons = buttons | (((~PINC) & B01000000) >> 4);
  // A and B
  buttons = buttons | (((~PINF) & B11000000) >> 6);
#elif defined(ARDUBOY_10)
  // down, up, left right
  buttons = ((~PINF) & B11110000);
  // A (left)
  buttons = buttons | (((~PINE) & B01000000) >> 3);
  // B (right)
  buttons = buttons | (((~PINB) & B00010000) >> 2);
#endif

  return buttons;
}
