#include "ArduboyDev.h"
#include "glcdfont.c"

Arduboy::Arduboy() { }

void Arduboy::start()
{
  pinMode(DC, OUTPUT);
  pinMode(CS, OUTPUT);
  pinMode(8, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  pinMode(A0, INPUT_PULLUP);
  pinMode(A1, INPUT_PULLUP);

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

  *csport |= cspinmask;
  *dcport &= ~dcpinmask;
  *csport &= ~cspinmask;

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

  *csport |= cspinmask;
  *csport |= cspinmask;
  *dcport &= ~dcpinmask;
  *csport &= ~cspinmask;

  SPI.transfer(0x20);     // set display mode
  SPI.transfer(0x00);     // horizontal addressing mode
  SPI.transfer(0x21);     // set col address

  unsigned char start = 0;
  unsigned char end = WIDTH - 1;
  SPI.transfer(start & 0x7F);
  SPI.transfer(end & 0x7F);
  SPI.transfer(0x22); // set page address

  start = 0;
  end = (HEIGHT/8)-1;
  SPI.transfer(start & 0x07);
  SPI.transfer(end & 0x07);

  *dcport |= dcpinmask;
  *csport &= ~cspinmask;
}

void Arduboy::blank()
{
  for (int a = 0; a < (HEIGHT*WIDTH)/8; a++) SPI.transfer(0x00);
}

void Arduboy::clearDisplay()
{
  for (int a = 0; a < (HEIGHT*WIDTH)/8; a++) sBuffer[a] = 0x00;
}

void Arduboy::drawPixel(int x, int y, uint16_t value)
{
  if (x < 0 || x > (WIDTH-1) || y < 0 || y > (HEIGHT-1))
  {
    return;
  }

  int row = y / 8;
  if (value)
  {
    sBuffer[(row*WIDTH) + x] |=   1 << (y % 8);
  }
  else
  {
    sBuffer[(row*WIDTH) + x] &= ~(1 << (y % 8));
  }
}

void Arduboy::drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color)
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
(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color)
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

void Arduboy::fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color)
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
 uint16_t color
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
(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color)
{
  // bresenham's algorithm - thx wikpedia
  int16_t steep = abs(y1 - y0) > abs(x1 - x0);
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
  int16_t ystep;

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
(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
  drawFastHLine(x, y, w, color);
  drawFastHLine(x, y+h-1, w, color);
  drawFastVLine(x, y, h, color);
  drawFastVLine(x+w-1, y, h, color);
}

void Arduboy::drawFastVLine
(int16_t x, int16_t y, int16_t h, uint16_t color)
{
  int end = y+h;
  for (int a = max(0,y); a < min(end,HEIGHT); a++)
  {
    drawPixel(x,a,color);
  }
}

void Arduboy::drawFastHLine
(int16_t x, int16_t y, int16_t w, uint16_t color)
{
  int end = x+w;
  for (int a = max(0,x); a < min(end,WIDTH); a++)
  {
    drawPixel(a,y,color);
  }
}

void Arduboy::fillRect
(int16_t x, int16_t y, int16_t w, int16_t h, int16_t color)
{
  // stupidest version - update in subclasses if desired!
  for (int16_t i=x; i<x+w; i++)
  {
    drawFastVLine(i, y, h, color); 
  }
}

void Arduboy::fillScreen(uint16_t color)
{
  fillRect(0, 0, WIDTH, HEIGHT, color);
}

void Arduboy::drawRoundRect
(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color)
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
(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color)
{
  // smarter version
  fillRect(x+r, y, w-2*r, h, color);

  // draw four corners
  fillCircleHelper(x+w-r-1, y+r, r, 1, h-2*r-1, color);
  fillCircleHelper(x+r, y+r, r, 2, h-2*r-1, color);
}

void Arduboy::drawTriangle
(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color)
{
  drawLine(x0, y0, x1, y1, color);
  drawLine(x1, y1, x2, y2, color);
  drawLine(x2, y2, x0, y0, color);
}

void Arduboy::fillTriangle
(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color)
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
void Arduboy::drawBitmap
(int16_t x,
 int16_t y,
 const uint8_t *bitmap,
 int16_t w,
 int16_t h,
 uint16_t color
)
{
  //bitmap is off screen
  if (x+w < 0 || x > WIDTH-1 || y+h < 0 || y > HEIGHT-1) return;

  int yOffset = abs(y) % 8;
  int sRow = y / 8;

  if (y < 0)
  {
    sRow--;
    yOffset = 8 - yOffset;
  }

  for (int a = 0; a < h/8; a++)
  {
    int bRow = sRow + a;
    if (bRow > (HEIGHT/8)-1) break;
    if (bRow > -2) {
      for (int iCol = 0; iCol<w; iCol++)
      {
        if (iCol + x > (WIDTH-1))
        {
          break;
        }

        if (iCol + x > 0)
        {
          if (bRow >= 0)
          {
            if (color)
            {
              this->sBuffer[ (bRow*WIDTH) + x + iCol  ]
                |= pgm_read_byte(bitmap+(a*w)+iCol) << yOffset;
            }
            else
            {
              this->sBuffer[ (bRow*WIDTH) + x + iCol  ]
                &= ~(pgm_read_byte(bitmap+(a*w)+iCol) << yOffset);
            }
          }
          if (yOffset && bRow<(HEIGHT/8)-1 && bRow > -2)
          {
            if (color)
            {
              this->sBuffer[ ((bRow+1)*WIDTH) + x + iCol  ]
                |= pgm_read_byte(bitmap+(a*w)+iCol) >> (8-yOffset);
            }
            else
            {
              this->sBuffer[ ((bRow+1)*WIDTH) + x + iCol  ]
                &= ~(pgm_read_byte(bitmap+(a*w)+iCol) >> (8-yOffset));
            }
          }
        }
      }
    }
  }
}

void Arduboy::drawChar
(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size) 
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
      line = pgm_read_byte(font+(c*5)+i);
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

uint8_t Arduboy::width() { return WIDTH; }

uint8_t Arduboy::height() { return HEIGHT; }

uint8_t Arduboy::getInput()
{
  // b00lurdab
  uint8_t value = B00000000;

  if (digitalRead(9) == 0) { value = value | B00100000; }  // left
  if (digitalRead(8) == 0) { value = value | B00010000; }  // up
  if (digitalRead(5) == 0) { value = value | B00001000; }  // right
  if (digitalRead(10) == 0) { value = value | B00000100; }  // down
  if (digitalRead(A0) == 0) { value = value | B00000010; }  // a?
  if (digitalRead(A1) == 0) { value = value | B00000001; }  // b?
  return value;
}

void Arduboy::swap(int16_t& a, int16_t& b) {
  int temp = a;
  a = b;
  b = temp;
}

boolean Arduboy::collidePointRect(int16_t x1, int16_t y1 ,int16_t x2 ,int16_t y2, int16_t w, int16_t h){
	if((x1>=x2)&&(x1<x2+w))
	if((y1>=y2)&&(y1<y2+h))
	return true;
	return false;
}

boolean Arduboy::collideRectRect(int16_t x1, int16_t y1, int16_t w1, int16_t h1 ,int16_t x2 ,int16_t y2, int16_t w2, int16_t h2){
  return !( x2     >=  x1+w1  || 
            x2+w2  <=  x1     || 
            y2     >=  y1+h1  ||
            y2+h2  <=  y1     );
}



