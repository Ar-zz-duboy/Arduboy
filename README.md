ArduboyLib
==========

Core library for the Arduboy.  If you're building an app for the Arduboy this is likely where you want to start.


## How to use the library

Using `git` clone the repository.  Preferably you should clone this into your Arduino `libraries` directory.  This will allow the Arduboy library code to be used by any sketches you write or download later.  On Mac the `libraries` folder can be found in `[HOME]/Documents/Ardiuno/libraries`.

**From the Linux command line or Windows Powershell**
	
    $ git clone https://github.com/Arduboy/ArduBreakout.git

Open the any of the example .ino files in the Arduino IDE and select the **Leonardo** board as the target platform. Compile the source code, and upload to the Arduboy.

### Using sketches that include arduboy.h and arduboy.cpp files

These likely will not compile because Arduino will try and also link in the Arduboy library you installed in `libraries`.  This will result in a bunch of errors during compile.


You have two chocies:

- Remove the `Arduboy.cpp` and `Arduboy.h` and try to compile with the installed library.  This might work if the libraries are both close enough in version.
- Rename `Arduboy.h` to `CustomArduboy.h` (or a similar name) and change the `.ino` sketch file to `#include "CustomArduboy.h"`.  This will trick the Arduino software into thinking CustomArduboy is an entirely different library (which it will be at this point) and not try and use the global library you've installed into Arduino

## Examples

Examples can be found in the `examples` directory.

### Tunes

Shows how to use the `tunes` API to play music.

### ArduBreakout

A simple clone of the classic "Breakout".
