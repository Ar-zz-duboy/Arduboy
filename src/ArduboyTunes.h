/**
 * @file ArduboyTunes.h
 * \brief Deprecated
 */

#ifndef ARDUBOY_TUNES_H
#define ARDUBOY_TUNES_H

#include <Arduino.h>

/**
 * \class ArduboyTunes ArduboyTunes.h
 * \brief Deprecated
 * \deprecated Use an external library, such as ArduboyPlaytune, ArduboyTones
 * or even just Arduino `tone()` for sound.
 * \details
 * All functions in the ArduboyTunes class have been deprecated. They will all
 * just do nothing and return. They have been left as dummy functions so that
 * any existing sketch using them will compile without errors. The sketch will
 * still run but no sound will be produced. Such a sketch can be modified to
 * use an external library. The ArduboyPlaytune library provides a direct
 * replacement for all functions in this class. However, if only `tone()` is
 * used to generate sound and not `playScore()`, then the ArduboyTones library
 * could also be considered.
 */
class ArduboyTunes
{
 public:
  ArduboyTunes();
  ArduboyTunes(bool);
  /**
   * Assign a timer to an output pin.
   * \deprecated Use an exteral library for sound.
   */
  void initChannel(uint8_t pin)
    __attribute__((deprecated("Use an exteral library for sound")));

  /**
   * Start playing a polyphonic score.
   * \deprecated Will produce no sound. Use an external library. 
   */
  void playScore(const uint8_t *score)
    __attribute__((deprecated("Will produce no sound. Use an external library")));

  /**
   * Stop playing the score.
   * \deprecated Use an exteral library for sound.
   */
  void stopScore()
    __attribute__((deprecated("Use an exteral library for sound")));

  /**
   * Check if a score is playing.
   * \deprecated Use an exteral library for sound.
   */
  bool playing()
    __attribute__((deprecated("Use an exteral library for sound")));

  /**
   * Stop all timers.
   * \deprecated Use an exteral library for sound.
   */
  void closeChannels()
    __attribute__((deprecated("Use an exteral library for sound")));

  /**
   * Play a tone.
   * \deprecated Will produce no sound. Use an external library. 
   */
  void tone(unsigned int frequency, unsigned long duration)
    __attribute__((deprecated("Will produce no sound. Use an external library")));
};

#endif
