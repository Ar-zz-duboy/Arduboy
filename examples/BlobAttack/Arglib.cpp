#include "Arglib.h"

const byte PROGMEM tune_pin_to_timer_PGM[] = { 3, 1 };
volatile byte *_tunes_timer1_pin_port;
volatile byte _tunes_timer1_pin_mask;
volatile int32_t timer1_toggle_count;
volatile byte *_tunes_timer3_pin_port;
volatile byte _tunes_timer3_pin_mask;
byte _tune_pins[AVAILABLE_TIMERS];
byte _tune_num_chans = 0;
volatile boolean tune_playing; // is the score still playing?
volatile unsigned wait_timer_frequency2;       /* its current frequency */
volatile unsigned wait_timer_old_frequency2;   /* its previous frequency */
volatile boolean wait_timer_playing = false;   /* is it currently playing a note? */
volatile boolean doing_delay = false;          /* are we using it for a tune_delay()? */
volatile boolean tonePlaying = false;
volatile unsigned long wait_toggle_count;      /* countdown score waits */
volatile unsigned long delay_toggle_count;     /* countdown tune_ delay() delays */


// pointers to your musical score and your position in said score
volatile const byte *score_start = 0;
volatile const byte *score_cursor = 0;

Arduboy::Arduboy() { }

// store current and previous buttons state for frame based button events
// you should be using nextFrame() in almost all cases, not calling this
// directly
SimpleButtons::SimpleButtons(Arduboy &a) {arduboy = &a;}

void Arduboy::start()
{
  #if F_CPU == 8000000L
  slowCPU();
  #endif

  SPI.begin();
  pinMode(DC, OUTPUT);
  pinMode(CS, OUTPUT);
  pinMode(PIN_LEFT_BUTTON, INPUT_PULLUP);
  pinMode(PIN_RIGHT_BUTTON, INPUT_PULLUP);
  pinMode(PIN_UP_BUTTON, INPUT_PULLUP);
  pinMode(PIN_DOWN_BUTTON, INPUT_PULLUP);
  pinMode(PIN_A_BUTTON, INPUT_PULLUP);
  pinMode(PIN_B_BUTTON, INPUT_PULLUP);
  tunes.initChannel(PIN_SPEAKER_1);
  tunes.initChannel(PIN_SPEAKER_2);


  csport = portOutputRegister(digitalPinToPort(CS));
  cspinmask = digitalPinToBitMask(CS);
  dcport = portOutputRegister(digitalPinToPort(DC));
  dcpinmask = digitalPinToBitMask(DC);

  /**
   * Setup reset pin direction (used by both SPI and I2C)
   */
  pinMode(RST, OUTPUT);
  digitalWrite(RST, HIGH);
  delay(1);           // VDD (3.3V) goes high at start, lets just chill for a ms
  digitalWrite(RST, LOW);   // bring reset low
  delay(10);          // wait 10ms
  digitalWrite(RST, HIGH);  // bring out of reset

  bootLCD();

  #ifdef SAFE_MODE
  if (pressed(LEFT_BUTTON+UP_BUTTON))
    safeMode();
  #endif

  audio.setup();
  saveMuchPower();
}

#if F_CPU == 8000000L
// if we're compiling for 8Mhz we need to slow the CPU down because the
// hardware clock on the Arduboy is 16MHz
void Arduboy::slowCPU()
{
  uint8_t oldSREG = SREG;
  cli();                // suspend interrupts
  CLKPR = _BV(CLKPCE);  // allow reprogramming clock
  CLKPR = 1;            // set clock divisor to 2 (0b0001)
  SREG = oldSREG;       // restore interrupts
}
#endif

void Arduboy::bootLCD()
{
  LCDCommandMode();
  SPI.transfer(0xAE);  // Display Off
  SPI.transfer(0XD5);  // Set Display Clock Divisor v
  SPI.transfer(0xF0);  //   0x80 is default
  SPI.transfer(0xA8);  // Set Multiplex Ratio v
  SPI.transfer(0x3F);
  SPI.transfer(0xD3);  // Set Display Offset v
  SPI.transfer(0x0);
  SPI.transfer(0x40);  // Set Start Line (0)
  SPI.transfer(0x8D);  // Charge Pump Setting v
  SPI.transfer(0x14);  //   Enable
  // why are we running this next pair twice?
  SPI.transfer(0x20);  // Set Memory Mode v
  SPI.transfer(0x00);  //   Horizontal Addressing
  SPI.transfer(0xA1);  // Set Segment Re-map (A0) | (b0001)
  SPI.transfer(0xC8);  // Set COM Output Scan Direction
  SPI.transfer(0xDA);  // Set COM Pins v
  SPI.transfer(0x12);
  SPI.transfer(0x81);  // Set Contrast v
  SPI.transfer(0xCF);
  SPI.transfer(0xD9);  // Set Precharge
  SPI.transfer(0xF1);
  SPI.transfer(0xDB);  // Set VCom Detect
  SPI.transfer(0x40);
  SPI.transfer(0xA4);  // Entire Display ON
  SPI.transfer(0xA6);  // Set normal/inverse display
  SPI.transfer(0xAF);  // Display On

  LCDCommandMode();
  SPI.transfer(0x20);     // set display mode
  SPI.transfer(0x00);     // horizontal addressing mode

  SPI.transfer(0x21);     // set col address
  SPI.transfer(0x00);
  SPI.transfer(COLUMN_ADDRESS_END);

  SPI.transfer(0x22); // set page address
  SPI.transfer(0x00);
  SPI.transfer(PAGE_ADDRESS_END);

  LCDDataMode();
}

// Safe Mode is engaged by holding down both the LEFT button and UP button
// when plugging the device into USB.  It puts your device into a tight
// loop and allows it to be reprogrammed even if you have uploaded a very
// broken sketch that interferes with the normal USB triggered auto-reboot
// functionality of the device.
void Arduboy::safeMode()
{
  display(); // too avoid random gibberish
  while (true) {
    asm volatile("nop \n");
  }
}

void Arduboy::LCDDataMode()
{
  *dcport |= dcpinmask;
  *csport &= ~cspinmask;
}

void Arduboy::LCDCommandMode()
{
  *csport |= cspinmask; // why are we doing this twice?
  *csport |= cspinmask;
  *dcport &= ~dcpinmask;
  *csport &= ~cspinmask;
}


/* Power Management */

void Arduboy::idle()
{
  set_sleep_mode(SLEEP_MODE_IDLE);
  sleep_mode();
}

void Arduboy::saveMuchPower()
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


/* Frame management */

void Arduboy::setFrameRate(uint8_t rate)
{
  frameRate = rate;
  eachFrameMillis = 1000/rate;
}

bool Arduboy::everyXFrames(uint8_t frames)
{
  return frameCount % frames == 0;
}

bool Arduboy::nextFrame()
{
  long now = millis();
  uint8_t remaining;

  // post render
  if (post_render) {
    lastFrameDurationMs = now - lastFrameStart;
    frameCount++;
    post_render = false;
  }

  // if it's not time for the next frame yet
  if (now < nextFrameStart) {
    remaining = nextFrameStart - now;
    // if we have more than 1ms to spare, lets sleep
    // we should be woken up by timer0 every 1ms, so this should be ok
    if (remaining > 1)
      idle();
    return false;
  }

  // pre-render

  // technically next frame should be last frame + each frame but if we're
  // running a slow render we would constnatly be behind the clock
  // keep an eye on this and see how it works.  If it works well the
  // lastFrameStart variable could be eliminated completely
  nextFrameStart = now + eachFrameMillis;
  lastFrameStart = now;
  post_render = true;
  return post_render;
}

// returns the load on the CPU as a percentage
// this is based on how much of the time your app is spends rendering
// frames.  This number can be higher than 100 if your app is rendering
// really slowly.
int Arduboy::cpuLoad()
{
  return lastFrameDurationMs*100 / eachFrameMillis;
}

// seed the random number generator with entropy from the temperature,
// voltage reading, and microseconds since boot.
// this method is still most effective when called semi-randomly such
// as after a user hits a button to start a game or other semi-random
// events
void Arduboy::initRandomSeed()
{
  power_adc_enable(); // ADC on
  randomSeed(~rawADC(ADC_TEMP) * ~rawADC(ADC_VOLTAGE) * ~micros() + micros());
  power_adc_disable(); // ADC off
}

uint16_t Arduboy::rawADC(byte adc_bits)
{
  ADMUX = adc_bits;
  // we also need MUX5 for temperature check
  if (adc_bits == ADC_TEMP) {
    ADCSRB = _BV(MUX5);
  }

  delay(2); // Wait for ADMUX setting to settle
  ADCSRA |= _BV(ADSC); // Start conversion
  while (bit_is_set(ADCSRA,ADSC)); // measuring

  return ADCW;
}


/* Graphics */

void Arduboy::blank()
{
  for (int a = 0; a < (HEIGHT*WIDTH)/8; a++) SPI.transfer(0x00);
}

void Arduboy::clearDisplay()
{
  for (int a = 0; a < (HEIGHT*WIDTH)/8; a++) sBuffer[a] = 0x00;
}

void Arduboy::drawPixel(int x, int y, uint8_t color)
{
  #ifdef PIXEL_SAFE_MODE
  if (x < 0 || x > (WIDTH-1) || y < 0 || y > (HEIGHT-1))
  {
    return;
  }
  #endif

  uint8_t row = (uint8_t)y / 8;
  if (color)
  {
    sBuffer[(row*WIDTH) + (uint8_t)x] |=   _BV((uint8_t)y % 8);
  }
  else
  {
    sBuffer[(row*WIDTH) + (uint8_t)x] &= ~ _BV((uint8_t)y % 8);
  }
}

uint8_t Arduboy::getPixel(uint8_t x, uint8_t y)
{
  uint8_t row = y / 8;
  uint8_t bit_position = y % 8;
  return (sBuffer[(row*WIDTH) + x] & _BV(bit_position)) >> bit_position;
}

void Arduboy::drawCircle(int16_t x0, int16_t y0, int16_t r, uint8_t color)
{
  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x = 0;
  int16_t y = r;

  drawPixel(x0, y0+r, color);
  drawPixel(x0, y0-r, color);
  drawPixel(x0+r, y0, color);
  drawPixel(x0-r, y0, color);

  while (x<y)
  {
    if (f >= 0)
    {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }

    x++;
    ddF_x += 2;
    f += ddF_x;

    drawPixel(x0 + x, y0 + y, color);
    drawPixel(x0 - x, y0 + y, color);
    drawPixel(x0 + x, y0 - y, color);
    drawPixel(x0 - x, y0 - y, color);
    drawPixel(x0 + y, y0 + x, color);
    drawPixel(x0 - y, y0 + x, color);
    drawPixel(x0 + y, y0 - x, color);
    drawPixel(x0 - y, y0 - x, color);
  }
}

void Arduboy::drawCircleHelper
(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint8_t color)
{
  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x = 0;
  int16_t y = r;

  while (x<y)
  {
    if (f >= 0)
    {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }

    x++;
    ddF_x += 2;
    f += ddF_x;

    if (cornername & 0x4)
    {
      drawPixel(x0 + x, y0 + y, color);
      drawPixel(x0 + y, y0 + x, color);
    }
    if (cornername & 0x2)
    {
      drawPixel(x0 + x, y0 - y, color);
      drawPixel(x0 + y, y0 - x, color);
    }
    if (cornername & 0x8)
    {
      drawPixel(x0 - y, y0 + x, color);
      drawPixel(x0 - x, y0 + y, color);
    }
    if (cornername & 0x1)
    {
      drawPixel(x0 - y, y0 - x, color);
      drawPixel(x0 - x, y0 - y, color);
    }
  }
}

void Arduboy::fillCircle(int16_t x0, int16_t y0, int16_t r, uint8_t color)
{
  drawFastVLine(x0, y0-r, 2*r+1, color);
  fillCircleHelper(x0, y0, r, 3, 0, color);
}

void Arduboy::fillCircleHelper
(
 int16_t x0,
 int16_t y0,
 int16_t r,
 uint8_t cornername,
 int16_t delta,
 uint8_t color
)
{
  // used to do circles and roundrects!
  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x = 0;
  int16_t y = r;

  while (x < y)
  {
    if (f >= 0)
    {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }

    x++;
    ddF_x += 2;
    f += ddF_x;

    if (cornername & 0x1)
    {
      drawFastVLine(x0+x, y0-y, 2*y+1+delta, color);
      drawFastVLine(x0+y, y0-x, 2*x+1+delta, color);
    }

    if (cornername & 0x2)
    {
      drawFastVLine(x0-x, y0-y, 2*y+1+delta, color);
      drawFastVLine(x0-y, y0-x, 2*x+1+delta, color);
    }
  }
}

void Arduboy::drawLine
(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t color)
{
  // bresenham's algorithm - thx wikpedia
  boolean steep = abs(y1 - y0) > abs(x1 - x0);
  if (steep) {
    swap(x0, y0);
    swap(x1, y1);
  }

  if (x0 > x1) {
    swap(x0, x1);
    swap(y0, y1);
  }

  int16_t dx, dy;
  dx = x1 - x0;
  dy = abs(y1 - y0);

  int16_t err = dx / 2;
  int8_t ystep;

  if (y0 < y1)
  {
    ystep = 1;
  }
  else
  {
    ystep = -1;
  }

  for (; x0 <= x1; x0++)
  {
    if (steep)
    {
      drawPixel(y0, x0, color);
    }
    else
    {
      drawPixel(x0, y0, color);
    }

    err -= dy;
    if (err < 0)
    {
      y0 += ystep;
      err += dx;
    }
  }
}

void Arduboy::drawRect
(int16_t x, int16_t y, int16_t w, int16_t h, uint8_t color)
{
  drawFastHLine(x, y, w, color);
  drawFastHLine(x, y+h-1, w, color);
  drawFastVLine(x, y, h, color);
  drawFastVLine(x+w-1, y, h, color);
}

void Arduboy::drawFastVLine
(int16_t x, int16_t y, int16_t h, uint8_t color)
{
  int end = y+h;
  for (int a = max(0,y); a < min(end,HEIGHT); a++)
  {
    drawPixel(x,a,color);
  }
}

void Arduboy::drawFastHLine
(int16_t x, int16_t y, int16_t w, uint8_t color)
{
  int end = x+w;
  for (int a = max(0,x); a < min(end,WIDTH); a++)
  {
    drawPixel(a,y,color);
  }
}

void Arduboy::fillRect
(int16_t x, int16_t y, int16_t w, int16_t h, uint8_t color)
{
  // stupidest version - update in subclasses if desired!
  for (int16_t i=x; i<x+w; i++)
  {
    drawFastVLine(i, y, h, color);
  }
}

void Arduboy::fillScreen(uint8_t color)
{
  fillRect(0, 0, WIDTH, HEIGHT, color);
}

void Arduboy::drawRoundRect
(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint8_t color)
{
  // smarter version
  drawFastHLine(x+r, y, w-2*r, color); // Top
  drawFastHLine(x+r, y+h-1, w-2*r, color); // Bottom
  drawFastVLine(x, y+r, h-2*r, color); // Left
  drawFastVLine(x+w-1, y+r, h-2*r, color); // Right
  // draw four corners
  drawCircleHelper(x+r, y+r, r, 1, color);
  drawCircleHelper(x+w-r-1, y+r, r, 2, color);
  drawCircleHelper(x+w-r-1, y+h-r-1, r, 4, color);
  drawCircleHelper(x+r, y+h-r-1, r, 8, color);
}

void Arduboy::fillRoundRect
(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint8_t color)
{
  // smarter version
  fillRect(x+r, y, w-2*r, h, color);

  // draw four corners
  fillCircleHelper(x+w-r-1, y+r, r, 1, h-2*r-1, color);
  fillCircleHelper(x+r, y+r, r, 2, h-2*r-1, color);
}

void Arduboy::drawTriangle
(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t color)
{
  drawLine(x0, y0, x1, y1, color);
  drawLine(x1, y1, x2, y2, color);
  drawLine(x2, y2, x0, y0, color);
}

void Arduboy::fillTriangle
(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t color)
{

  int16_t a, b, y, last;
  // Sort coordinates by Y order (y2 >= y1 >= y0)
  if (y0 > y1)
  {
    swap(y0, y1); swap(x0, x1);
  }
  if (y1 > y2)
  {
    swap(y2, y1); swap(x2, x1);
  }
  if (y0 > y1)
  {
    swap(y0, y1); swap(x0, x1);
  }

  if(y0 == y2)
  { // Handle awkward all-on-same-line case as its own thing
    a = b = x0;
    if(x1 < a)
    {
      a = x1;
    }
    else if(x1 > b)
    {
      b = x1;
    }
    if(x2 < a)
    {
      a = x2;
    }
    else if(x2 > b)
    {
      b = x2;
    }
    drawFastHLine(a, y0, b-a+1, color);
    return;
  }

  int16_t dx01 = x1 - x0,
      dy01 = y1 - y0,
      dx02 = x2 - x0,
      dy02 = y2 - y0,
      dx12 = x2 - x1,
      dy12 = y2 - y1,
      sa = 0,
      sb = 0;

  // For upper part of triangle, find scanline crossings for segments
  // 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
  // is included here (and second loop will be skipped, avoiding a /0
  // error there), otherwise scanline y1 is skipped here and handled
  // in the second loop...which also avoids a /0 error here if y0=y1
  // (flat-topped triangle).
  if (y1 == y2)
  {
    last = y1;   // Include y1 scanline
  }
  else
  {
    last = y1-1; // Skip it
  }


  for(y = y0; y <= last; y++)
  {
    a   = x0 + sa / dy01;
    b   = x0 + sb / dy02;
    sa += dx01;
    sb += dx02;

    if(a > b)
    {
      swap(a,b);
    }

    drawFastHLine(a, y, b-a+1, color);
  }

  // For lower part of triangle, find scanline crossings for segments
  // 0-2 and 1-2.  This loop is skipped if y1=y2.
  sa = dx12 * (y - y1);
  sb = dx02 * (y - y0);

  for(; y <= y2; y++)
  {
    a   = x1 + sa / dy12;
    b   = x0 + sb / dy02;
    sa += dx12;
    sb += dx02;

    if(a > b)
    {
      swap(a,b);
    }

    drawFastHLine(a, y, b-a+1, color);
  }
}

void Arduboy::drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint8_t color) {
  // no need to dar at all of we're offscreen
  if (x+w < 0 || x > WIDTH-1 || y+h < 0 || y > HEIGHT-1)
    return;

  int yOffset = abs(y) % 8;
  int sRow = y / 8;
  if (y < 0) {
    sRow--;
    yOffset = 8 - yOffset;
  }
  int rows = h/8;
  if (h%8!=0) rows++;
  for (int a = 0; a < rows; a++) {
    int bRow = sRow + a;
    if (bRow > (HEIGHT/8)-1) break;
    if (bRow > -2) {
      for (int iCol = 0; iCol<w; iCol++) {
        if (iCol + x > (WIDTH-1)) break;
        if (iCol + x >= 0) {
          if (bRow >= 0) {
            if (color) this->sBuffer[ (bRow*WIDTH) + x + iCol  ]  |= pgm_read_byte(bitmap+(a*w)+iCol) << yOffset;
            else this->sBuffer[ (bRow*WIDTH) + x + iCol  ]  &= ~(pgm_read_byte(bitmap+(a*w)+iCol) << yOffset);
          }
          if (yOffset && bRow<(HEIGHT/8)-1 && bRow > -2) {
            if (color) this->sBuffer[ ((bRow+1)*WIDTH) + x + iCol  ] |= pgm_read_byte(bitmap+(a*w)+iCol) >> (8-yOffset);
            else this->sBuffer[ ((bRow+1)*WIDTH) + x + iCol  ] &= ~(pgm_read_byte(bitmap+(a*w)+iCol) >> (8-yOffset));
          }
        }
      }
    }
  }
}


// Draw images that are bit-oriented horizontally
//
// This requires a lot of additional CPU power and will draw images much
// more slowly than drawBitmap where the images are stored in a format that
// allows them to be directly written to the screen hardware fast. It is
// recommended you use drawBitmap when possible.
void Arduboy::drawSlowXYBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint8_t color) {
  // no need to dar at all of we're offscreen
  if (x+w < 0 || x > WIDTH-1 || y+h < 0 || y > HEIGHT-1)
    return;

  int16_t xi, yi, byteWidth = (w + 7) / 8;
  for(yi = 0; yi < h; yi++) {
    for(xi = 0; xi < w; xi++ ) {
      if(pgm_read_byte(bitmap + yi * byteWidth + xi / 8) & (128 >> (xi & 7))) {
        drawPixel(x + xi, y + yi, color);
      }
    }
  }
}


void Arduboy::drawChar
(int16_t x, int16_t y, unsigned char c, uint8_t color, uint8_t bg, uint8_t size)
{

  if ((x >= WIDTH) ||         // Clip right
    (y >= HEIGHT) ||        // Clip bottom
    ((x + 5 * size - 1) < 0) ||   // Clip left
    ((y + 8 * size - 1) < 0)    // Clip top
  )
  {
    return;
  }

  for (int8_t i=0; i<6; i++ )
  {
    uint8_t line;
    if (i == 5)
    {
      line = 0x0;
    }
    else
    {
     // line = pgm_read_byte(font+(c*5)+i);
    }

    for (int8_t j = 0; j<8; j++)
    {
      if (line & 0x1)
      {
        if (size == 1) // default size
        {
          drawPixel(x+i, y+j, color);
        }
        else  // big size
        {
          fillRect(x+(i*size), y+(j*size), size, size, color);
        }
      }
      else if (bg != color)
      {
        if (size == 1) // default size
        {
          drawPixel(x+i, y+j, bg);
        }
        else
        {  // big size
          fillRect(x+i*size, y+j*size, size, size, bg);
        }
      }

      line >>= 1;
    }
  }
}

void Arduboy::setCursor(int16_t x, int16_t y)
{
  cursor_x = x;
  cursor_y = y;
}

void Arduboy::setTextSize(uint8_t s)
{
  textsize = (s > 0) ? s : 1;
}

void Arduboy::setTextWrap(boolean w)
{
  wrap = w;
}

size_t Arduboy::write(uint8_t c)
{
  if (c == '\n')
  {
    cursor_y += textsize*8;
    cursor_x = 0;
  }
  else if (c == '\r')
  {
    // skip em
  }
  else
  {
    drawChar(cursor_x, cursor_y, c, 1, 0, textsize);
    cursor_x += textsize*6;
    if (wrap && (cursor_x > (WIDTH - textsize*6)))
    {
      cursor_y += textsize*8;
      cursor_x = 0;
    }
  }
}

void Arduboy::display()
{
  this->drawScreen(sBuffer);
}

void Arduboy::drawScreen(const unsigned char *image)
{
  for (int a = 0; a < (HEIGHT*WIDTH)/8; a++)
  {
    SPI.transfer(pgm_read_byte(image + a));
  }
}

void Arduboy::drawScreen(unsigned char image[])
{
  for (int a = 0; a < (HEIGHT*WIDTH)/8; a++)
  {
    SPI.transfer(image[a]);
  }
}

unsigned char* Arduboy::getBuffer(){
  return sBuffer;
}

uint8_t Arduboy::width() { return WIDTH; }

uint8_t Arduboy::height() { return HEIGHT; }

// returns true if the button mask passed in is pressed
//
//   if (pressed(LEFT_BUTTON + A_BUTTON))
boolean Arduboy::pressed(uint8_t buttons)
{
 uint8_t button_state = getInput();
 return (button_state & buttons) == buttons;
}

// returns true if the button mask passed in not pressed
//
//   if (not_pressed(LEFT_BUTTON))
boolean Arduboy::not_pressed(uint8_t buttons)
{
 uint8_t button_state = getInput();
 return (button_state & buttons) == 0;
}


uint8_t Arduboy::getInput()
{
  // using ports here is ~100 bytes smaller than digitalRead()
  #ifdef DEVKIT
  // down, left, up
  uint8_t buttons = ((~PINB) & B01110000);
  // right button
  buttons = buttons | (((~PINC) & B01000000) >> 4);
  // A and B
  buttons = buttons | (((~PINF) & B11000000) >> 6);
  #endif

  // b0dlu0rab - see button defines in Arduboy.h
  return buttons;
}

void Arduboy::swap(int16_t& a, int16_t& b) {
  int temp = a;
  a = b;
  b = temp;
}


void SimpleButtons::poll()
{
  previousButtonState = currentButtonState;
  currentButtonState = arduboy->getInput();
}
// returns true if a button has just been pressed
// if the button has been held down for multiple frames this will return
// false.  You should only use this to poll a single button.
boolean SimpleButtons::justPressed(uint8_t button)
{
 return (!(previousButtonState & button) && (currentButtonState & button));
}

/* AUDIO */

void ArduboyAudio::on() {
  power_timer1_enable();
  power_timer3_enable();
  audio_enabled = true;
}

bool ArduboyAudio::enabled() {
  return audio_enabled;
}

void ArduboyAudio::off() {
  audio_enabled = false;
  power_timer1_disable();
  power_timer3_disable();
}

void ArduboyAudio::save_on_off() {
  EEPROM.write(EEPROM_AUDIO_ON_OFF, audio_enabled);
}

void ArduboyAudio::setup() {
  tune_playing = false;
  if (EEPROM.read(EEPROM_AUDIO_ON_OFF))
    on();
}

void ArduboyAudio::tone(uint8_t channel, unsigned int frequency, unsigned long duration)
{
  // if (audio_enabled)
    // ::tone(channel, frequency, duration);
}


/* TUNES */

void ArduboyTunes::initChannel(byte pin) {
  byte timer_num;

  // we are all out of timers
  if (_tune_num_chans == AVAILABLE_TIMERS)
    return;

  timer_num = pgm_read_byte(tune_pin_to_timer_PGM + _tune_num_chans);
  _tune_pins[_tune_num_chans] = pin;
  _tune_num_chans++;
  pinMode(pin, OUTPUT);
  switch (timer_num) {
    case 1: // 16 bit timer
      TCCR1A = 0;
      TCCR1B = 0;
      bitWrite(TCCR1B, WGM12, 1);
      bitWrite(TCCR1B, CS10, 1);
      _tunes_timer1_pin_port = portOutputRegister(digitalPinToPort(pin));
      _tunes_timer1_pin_mask = digitalPinToBitMask(pin);
      break;
    case 3: // 16 bit timer
      TCCR3A = 0;
      TCCR3B = 0;
      bitWrite(TCCR3B, WGM32, 1);
      bitWrite(TCCR3B, CS30, 1);
      _tunes_timer3_pin_port = portOutputRegister(digitalPinToPort(pin));
      _tunes_timer3_pin_mask = digitalPinToBitMask(pin);
      playNote(0, 60);  /* start and stop channel 0 (timer 3) on middle C so wait/delay works */
      stopNote(0);
      break;
  }
}


void ArduboyTunes::playNote(byte chan, byte note) {
  byte timer_num;
  byte prescalar_bits;
  unsigned int frequency2; /* frequency times 2 */
  unsigned long ocr;

  // we can't plan on a channel that does not exist
  if (chan >= _tune_num_chans)
    return;

  // we only have frequencies for 128 notes
  if (note > 127)
    note = 127;

  timer_num = pgm_read_byte(tune_pin_to_timer_PGM + chan);
  //frequency2 = pgm_read_word(_midi_note_frequencies + note);

  //******  16-bit timer  *********
  // two choices for the 16 bit timers: ck/1 or ck/64
  ocr = F_CPU / frequency2 - 1;
  prescalar_bits = 0b001;
  if (ocr > 0xffff) {
    ocr = F_CPU / frequency2 / 64 - 1;
    prescalar_bits = 0b011;
  }
  // Set the OCR for the given timer, then turn on the interrupts
  switch (timer_num) {
    case 1:
      TCCR1B = (TCCR1B & 0b11111000) | prescalar_bits;
      OCR1A = ocr;
      bitWrite(TIMSK1, OCIE1A, 1);
      break;
    case 3:
      TCCR3B = (TCCR3B & 0b11111000) | prescalar_bits;
      OCR3A = ocr;
      wait_timer_frequency2 = frequency2;  // for "tune_delay" function
      wait_timer_playing = true;
      bitWrite(TIMSK3, OCIE3A, 1);
      break;
  }
}

void ArduboyTunes::stopNote(byte chan) {
  byte timer_num;
  timer_num = pgm_read_byte(tune_pin_to_timer_PGM + chan);
  switch (timer_num) {
    case 1:
      TIMSK1 &= ~(1 << OCIE1A);                 // disable the interrupt
      *_tunes_timer1_pin_port &= ~(_tunes_timer1_pin_mask);   // keep pin low after stop
      break;
    case 3:
      wait_timer_playing = false;
      *_tunes_timer3_pin_port &= ~(_tunes_timer3_pin_mask);   // keep pin low after stop
      break;
  }
}

void ArduboyTunes::playScore(const byte *score) {
  score_start = score;
  score_cursor = score_start;
  step();  /* execute initial commands */
  tune_playing = true;  /* release the interrupt routine */
}



bool ArduboyTunes::playing()
{
  return tune_playing;
}


/* Do score commands until a "wait" is found, or the score is stopped.
This is called initially from tune_playcore, but then is called
from the interrupt routine when waits expire.
*/
/* if CMD < 0x80, then the other 7 bits and the next byte are a 15-bit big-endian number of msec to wait */
void ArduboyTunes::step() {
  byte command, opcode, chan;
  unsigned duration;

  while (1) {
    command = pgm_read_byte(score_cursor++);
    opcode = command & 0xf0;
    chan = command & 0x0f;
    if (opcode == TUNE_OP_STOPNOTE) { /* stop note */
      stopNote(chan);
    }
    else if (opcode == TUNE_OP_PLAYNOTE) { /* play note */
      playNote(chan, pgm_read_byte(score_cursor++));
    }
    else if (opcode == TUNE_OP_RESTART) { /* restart score */
      score_cursor = score_start;
    }
    else if (opcode == TUNE_OP_STOP) { /* stop score */
      tune_playing = false;
      break;
    }
    else if (opcode < 0x80) { /* wait count in msec. */
      duration = ((unsigned)command << 8) | (pgm_read_byte(score_cursor++));
      wait_toggle_count = ((unsigned long) wait_timer_frequency2 * duration + 500) / 1000;
      if (wait_toggle_count == 0) wait_toggle_count = 1;
      break;
    }
  }
}

void ArduboyTunes::delay (unsigned duration) {
  boolean notdone;
  noInterrupts();
  delay_toggle_count = ((unsigned long) wait_timer_frequency2 * duration + 500) / 1000;
  doing_delay = true;
  interrupts();
  do { // wait until the interrupt routines decrements the toggle count to zero
    noInterrupts();
    notdone = delay_toggle_count != 0;  /* interrupt-safe test */
    interrupts();
  }
  while (notdone);
  doing_delay = false;
}

void ArduboyTunes::closeChannels(void) {
  byte timer_num;
  for (uint8_t chan=0; chan < _tune_num_chans; chan++) {
    timer_num = pgm_read_byte(tune_pin_to_timer_PGM + chan);
    switch (timer_num) {
      case 1:
        TIMSK1 &= ~(1 << OCIE1A);
        break;
      case 3:
        TIMSK3 &= ~(1 << OCIE3A);
        break;
    }
    digitalWrite(_tune_pins[chan], 0);
  }
  _tune_num_chans = 0;
  tune_playing = false;
}

void ArduboyTunes::soundOutput()
{
  if (wait_timer_playing) { // toggle the pin if we're sounding a note
    *_tunes_timer3_pin_port ^= _tunes_timer3_pin_mask;
  }
  if (tune_playing && wait_toggle_count && --wait_toggle_count == 0) {
    // end of a score wait, so execute more score commands
    wait_timer_old_frequency2 = wait_timer_frequency2;  // save this timer's frequency
    ArduboyTunes::step();  // execute commands
    // If this timer's frequency has changed and we're using it for a tune_delay(),
    // recompute the number of toggles to wait for
    if (doing_delay && wait_timer_old_frequency2 != wait_timer_frequency2) {
      if (delay_toggle_count >= 0x20000UL && wait_timer_frequency2 >= 0x4000U) {
        // Need to avoid 32-bit overflow...
        delay_toggle_count = ( (delay_toggle_count+4>>3) * (wait_timer_frequency2+2>>2) / wait_timer_old_frequency2 )<<5;
      }
      else {
        delay_toggle_count = delay_toggle_count * wait_timer_frequency2 / wait_timer_old_frequency2;
      }
    }
  }
  if (doing_delay && delay_toggle_count) --delay_toggle_count;  // countdown for tune_delay()
}
void ArduboyTunes::tone(unsigned int frequency, unsigned long duration) {
  tonePlaying = true;
  uint8_t prescalarbits = 0b001;
  int32_t toggle_count = 0;
  uint32_t ocr = 0;

  // two choices for the 16 bit timers: ck/1 or ck/64
  ocr = F_CPU / frequency / 2 - 1;
  prescalarbits = 0b001;
  if (ocr > 0xffff) {
    ocr = F_CPU / frequency / 2 / 64 - 1;
    prescalarbits = 0b011;
  }
  TCCR1B = (TCCR1B & 0b11111000) | prescalarbits;

  // Calculate the toggle count
  if (duration > 0) {
    toggle_count = 2 * frequency * duration / 1000;
  }
  else {
    toggle_count = -1;
  }
  // Set the OCR for the given timer,
  // set the toggle count,
  // then turn on the interrupts
  OCR1A = ocr;
  timer1_toggle_count = toggle_count;
  bitWrite(TIMSK1, OCIE1A, 1);
}

ISR(TIMER1_COMPA_vect) {  // TIMER 1
  if (tonePlaying) {
    if (timer1_toggle_count != 0) {
      // toggle the pin
      *_tunes_timer1_pin_port ^= _tunes_timer1_pin_mask;
      if (timer1_toggle_count > 0) timer1_toggle_count--;
    }
    else {
      tonePlaying = false;
      TIMSK1 &= ~(1 << OCIE1A);                 // disable the interrupt
      *_tunes_timer1_pin_port &= ~(_tunes_timer1_pin_mask);   // keep pin low after stop
    }
  }
  else {
    *_tunes_timer1_pin_port ^= _tunes_timer1_pin_mask;  // toggle the pin
  }
}
ISR(TIMER3_COMPA_vect) {  // TIMER 3
  // Timer 3 is the one assigned first, so we keep it running always
  // and use it to time score waits, whether or not it is playing a note.
  ArduboyTunes::soundOutput();
}

