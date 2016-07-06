/** 
 * \file ArduboyAudio.h
 * \brief A class implementing the audio functionality for an Arduboy.
 *
 */

#ifndef ARDUBOY_AUDIO_H
#define ARDUBOY_AUDIO_H

#include <Arduino.h>
#include <EEPROM.h>

class ArduboyAudio
{
public:
  void static begin();
  void static on();
  void static off();
  void static saveOnOff();
  bool static enabled();

protected:
  bool static audio_enabled;
};

#endif
