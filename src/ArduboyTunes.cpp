#include "ArduboyTunes.h"

// The ArduboyTunes class has been deprecated.

ArduboyTunes::ArduboyTunes()
{
    // constructor
}

ArduboyTunes::ArduboyTunes(bool)
{
    // accept bool for constructor
}

void ArduboyTunes::initChannel(uint8_t pin)
{
    // init channel
}

void ArduboyTunes::playScore(const uint8_t *score)
{
    // play score
}

void ArduboyTunes::stopScore()
{
    // stop score
}

bool ArduboyTunes::playing()
{
  return false;
}

void ArduboyTunes::closeChannels()
{
    // close channels
}

void ArduboyTunes::tone(unsigned int frequency, unsigned long duration)
{
    // generate tone
}

