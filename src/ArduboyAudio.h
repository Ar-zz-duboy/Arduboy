/** 
 * @file ArduboyAudio.h
 * \brief A class implementing the audio functionality for an Arduboy.
 */

#ifndef ARDUBOY_AUDIO_H
#define ARDUBOY_AUDIO_H

#include <Arduino.h>
#include <EEPROM.h>

class ArduboyAudio
{
public:
  /**
   * \fn begin
   */
  void static begin();
  /**
   * \fn on
   */
  void static on();
  /**
   * \fn off
   */
  void static off();
  /**
   * \fn saveOnOff
   */
  void static saveOnOff();
  /**
   * \fn enabled
   */
  bool static enabled();

protected:
  /**
   * Boolean flag to determine if audio is enabled.
   */
  bool static audio_enabled;
};

#endif
