Arduboy Library · `develop`
===============

Information and instructions on using the <b>`develop`</b> branch of the Arduboy Library.

## About

Arduboy is an open-source hardware platform allowing gamers to easily make and share games using C/C++. The **Arduboy Library** is an open-source library that provides a programming interface for making games and applications that run on an Arduboy. The Arduboy Library supports both the **production** and **development** Arduboy units.

### Using the `develop` Branch

This is a development branch and considered highly unstable. Do not use this branch for your game or application.

### Testing

The included `Makefile` can be used to compile and test modifications to the library. See `test/scripts/arduino.sh` for a system boostrap; the variables in the `Makefile` will need to be adjusted to match your system.


#### Branches
A list of important Arduboy branches.

<b>`stable`</b>

Please use the `stable` branch for the current stable build. If you are unsure
which branch to use for your game, then this is the branch you are looking for.

<b>`develop`</b>

The `develop` branch for the Arduboy Library holds the bleeding-edge release
for the Arduboy project.

### Using the Aruboy Library

To use the Arduboy library, it must be installed and then included in your
project. An Arduboy or Leonardo must board must be selected as a target board.
Compile and upload the source to an Arduboy.

## Install

Install the library by cloning its repository.

    $ git clone -b develop https://github.com/Arduboy/Arduboy.git

### Where to Install

The library should be installed into your user's home Arduino `libraries`
directory. Refer to the following list for the location of the Arduino
`libraries` folder.

#### Arduino Library Locations by OS

Arduino library locations listed by operating system (OS).

**Linux**

    /home/username/Documents/Arduino/libraries

**Mac**

    /Users/username/Documents/Arduino/libraries

**Windows**

    C:\Users\username\My Documents\Arduino\libraries

#### Find the Location of Arduino `libraries/`

If you don't find the `libraries` folder in one of the above locations, you can
determine its location using the navigation menu `File > Preferences`. In the
`Settings` tab will be a `Sketchbook location:` field. The `libraries` folder
will be in the folder set in this field.

## Include

To use the Arduboy library in your own sketches, include the `Arduboy.h` header
file. To do so, add the following line to the top of your `.ino` file.

~~~~~~~~~~~~~~~{.cpp}
    #include "Arduboy.h"
~~~~~~~~~~~~~~~

You can have the Arduino IDE add `#include "Arduboy.h"` to your sketch 
automatically by using the navigation menu `Sketch > Include Library > Arduboy`.

## Board Selection

Select the **Leonardo** board as the target platform.

## Arduboy Examples

Example games and source can be found in the `examples` directory.

### Playing Examples

Find and play an example by opening it through the Arduino IDE, compiling, 
and uploading the example to the Arduboy.

Examples can be found in the Arduino IDE in the navigation menu under, 
`File > Examples > Arduboy > Example_Name`.

## Running on a Development Board

To run this library on a development Arduboy board, edit `src/core/core.h` so 
that `#define AB_DEVKIT` is uncommented and `#define ARDUBOY_10` is comment out.

~~~~~~~~~~~~~~~{.cpp}
    //#define ARDUBOY_10   //< compile for the production Arduboy v1.0
    #define AB_DEVKIT    //< compile for the official dev kit
~~~~~~~~~~~~~~~

## Sketches Already Including the Arduboy Library

Sketches that include copies of the Arduboy library may not compile if the
Arduboy library has been installed system wide. In these cases the Arduino 
compiler will try and link the system Arduboy library source with the local 
header file, which can cause compilation errors if the local library source 
differs from the system's Arduboy source.

Use one of the following options to compile sketches that have included copies
of the Aruboy Library,

**Option 1.**

Remove the local `Arduboy.cpp` and `Arduboy.h` files and try recompiling. 
This will only work in some cases.

**Option 2.**

Rename `Arduboy.h` to `CustomArduboy.h` (or a similar name) and add
`#include "CustomArduboy.h"` to the `.ino` sketch file. 

## Using `Doxygen`

Doxygen can be used to create the reference documentation.

To create the documentation, run `doxygen` if the root of the project folder.

### Prerequisites

The following packages must be installed.

    doxygen graphviz
