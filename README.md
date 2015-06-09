ArduboyLib
==========

Core library for the Arduboy.  If you're building an app for the Arduboy this is likely where you want to start.


## How to use the library

Using `git` clone the repository.  Preferably you should clone this into your Arduino `libraries` directory.  This will allow the Arduboy library code to be used by any sketches you write or download later.  On Mac the `libraries` folder can be found in `[HOME]/Documents/Ardiuno/libraries`.

**From the Linux command line or Windows Powershell**
	
    $ git clone https://github.com/Arduboy/ArduBreakout.git

Open the any of the example .ino files in the Arduino IDE and select the **Leonardo** board as the target platform. Compile the source code, and upload to the Arduboy.

### Developing on the core lib while having a "stable" version installed in your libraries folder

I haven't found a great way to do this yet.  The scenario is you want to compile random sketches off the Internet using the installed core library but you also want to do your own development work on the core library - and compile some sketches against your modified version of the core lib.

What I've done for now is just rename the stable version in `libraries` to `ArduinoStable` and keep the library I'm developing against in `Arduboy`.  Most everything I'm doing is compiling against the development version.  If I had to switch back and forth a lot I'd probably write a helper script to symlink in whichever library I needed.

If anyone else has a better idea here please let us know.

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
