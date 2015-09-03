#ifndef ArduboyGlove_h
#define ArduboyGlove_h

#include "Arduboy.h"

class ArduboyGlove : public Arduboy
{
public:
  void drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color, bool hflip=false); // hflip added**
};

#endif
