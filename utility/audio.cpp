#include "Arduboy.h"
#include "audio.h"

bool ArduboyAudio::audio_enabled = false;

void ArduboyAudio::on()
{
  audio_enabled = true;
}

bool ArduboyAudio::enabled()
{
  return audio_enabled;
}

void ArduboyAudio::off()
{
  audio_enabled = false;
}

void ArduboyAudio::saveOnOff()
{
  EEPROM.write(EEPROM_AUDIO_ON_OFF, audio_enabled);
}

void ArduboyAudio::begin()
{
  if (EEPROM.read(EEPROM_AUDIO_ON_OFF))
    on();
}

