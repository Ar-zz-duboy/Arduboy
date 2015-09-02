#include "ArduboyGlove.h"

void ArduboyGlove::drawBitmap
(int16_t x,
 int16_t y,
 const uint8_t *bitmap,
 int16_t w,
 int16_t h,
 uint16_t color,
 bool hflip // hflip added**
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
  
    int fCol; // fCol added**
      for (int iCol = 0; iCol<w; iCol++)
      {
    fCol = hflip ? 7-iCol : iCol; // fCol defined with hflip**
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
                |= pgm_read_byte(bitmap+(a*w)+fCol) << yOffset; // Changed iCol to fCol**
            }
            else
            {
              this->sBuffer[ (bRow*WIDTH) + x + iCol  ]
                &= ~(pgm_read_byte(bitmap+(a*w)+fCol) << yOffset); // Changed iCol to fCol**
            }
          }
          if (yOffset && bRow<(HEIGHT/8)-1 && bRow > -2)
          {
            if (color)
            {
              this->sBuffer[ ((bRow+1)*WIDTH) + x + iCol  ]
                |= pgm_read_byte(bitmap+(a*w)+fCol) >> (8-yOffset); // Changed iCol to fCol**
            }
            else
            {
              this->sBuffer[ ((bRow+1)*WIDTH) + x + iCol  ]
                &= ~(pgm_read_byte(bitmap+(a*w)+fCol) >> (8-yOffset)); // Changed iCol to fCol**
            }
          }
        }
      }
    }
  }
}

