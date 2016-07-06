PlayTune
========

Play a musical composition using the Arduboy.

Demonstrates playing music in the background while the "real" sketch code runs in the foreground.

The ArduboyPlaytune library must be installed to use this sketch

https://github.com/Arduboy/ArduboyPlayTune

A small composition is stored by `byte PROGMEM score`. The score is started in the sketch loop using `playScore(score)`.

D-Pad buttons will move the text and play a tone.

The A button mutes the sound. The screen is inverted when sound is muted.

The B button will turn sound back on if it's muted.

