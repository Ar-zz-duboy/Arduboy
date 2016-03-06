#include "Arduboy.h"
#include "audio.h"

bool ArduboyAudio::audio_enabled = false;

void ArduboyAudio::on()
{
  // fire up audio pins
#ifdef ARDUBOY_10
  pinMode(PIN_SPEAKER_1, OUTPUT);
  pinMode(PIN_SPEAKER_2, OUTPUT);
#else
  pinMode(PIN_SPEAKER_1, OUTPUT);
#endif
  audio_enabled = true;
}

bool ArduboyAudio::enabled()
{
  return audio_enabled;
}

void ArduboyAudio::off()
{
  audio_enabled = false;
  // shut off audio pins
  #ifdef ARDUBOY_10
    pinMode(PIN_SPEAKER_1, INPUT);
    pinMode(PIN_SPEAKER_2, INPUT);
  #else
    pinMode(PIN_SPEAKER_1, INPUT);
  #endif
}

void ArduboyAudio::saveOnOff()
{
  EEPROM.update(EEPROM_AUDIO_ON_OFF, audio_enabled);
}

void ArduboyAudio::begin()
{
  if (EEPROM.read(EEPROM_AUDIO_ON_OFF))
    on();
}

