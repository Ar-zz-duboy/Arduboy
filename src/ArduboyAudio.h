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
   * Run begin routines for ArduboyAudio.
   */
  void static begin();

  /**
   * Turn on audio system.
   */
  void static on();

  /**
   * Turn off the audio system.
   */
  void static off();

  /**
   * Save on off state.
   */
  void static saveOnOff();

  /**
   * Check if audio is enabled.
   */
  bool static enabled();

protected:

  /**
   * Boolean flag to determine if audio is enabled.
   */
  bool static audio_enabled;
};

#endif
