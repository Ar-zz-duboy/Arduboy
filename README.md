Arduboy Library · `1.1.1`
===============

Welcome to Arduboy! This is release `1.1.1` of the Arduboy Library.

## About

Arduboy is an open-source hardware platform allowing gamers to easily make and share games using C/C++. The **Arduboy Library** is an open-source library that provides a programming interface for making games and applications that run on an Arduboy. The Arduboy Library supports both the **production** and **development** Arduboy units.

### Contributing to the Arduboy Library

The Github url for this project is https://github.com/Arduboy/Arduboy.

Using `git` and Github, anyone can contribute to the Arduboy library. The source found using the Arduino _Library Manager_ can be found on the [`stable`](https://github.com/Arduboy/Arduboy/tree/stable) branch. For "bleeding-edge" work on the library, see the [`develop`](https://github.com/Arduboy/Arduboy/tree/develop) branch of the Arduboy repository.

## Using the Arduboy Library

To start using the Arduboy Library in your applications, the library must be
available to your own project's source. The most reliable method for including
the latest stable release of the Aruboy Library in your project is to use the
Arduino IDE and its _Library Manager_.

After the Arduboy Library is installed, it must be included for use in your project's source.

### Install

The Arduboy Library can be installed on your system in manys: through the Arduino IDE, using `git` to clone the library, or manually download and install the library. This section outlines the most common methods, and the Arduboy Library must only be installed once. Please use the first method _Install Using the Arduino IDE_ if you are unsure.

#### Install Using the Arduino IDE

Using the Arudino IDE, under the menu item, `Sketch > Include Library`, can be
found `Manage Libraries`. This will open the Arudino IDE's _Library Manager_.
Using this dialog the Arduboy Library can be search for and installed to be
made available to your projects. See the following **Include** section, or add
`#include <Arduboy.h>` to beginning of your project's source to start using the library.

#### Manually Installing

Install the library by cloning the `stable` branch of the Arduboy repository.

    $ git clone -b stable https://github.com/Arduboy/Arduboy.git

The Arduboy Library can also be installed by downloading and unpacking a
[release package](https://github.com/Arduboy/Arduboy/releases) into your
system's Arduino `libraries` folder.

#### Locating The Arduino `libraries/` Folder

The library should be installed into your user's home Arduino `libraries`
directory. Refer to the following list for the location of the Arduino
`libraries` folder.

##### Arduino Library Locations by OS

Arduino library locations listed by operating system (OS).

**Linux**

    /home/username/Documents/Arduino/libraries

**Mac**

    /Users/username/Documents/Arduino/libraries

**Windows**

    C:\Users\username\My Documents\Arduino\libraries

##### Use the Arduino IDE to Find or Change the Location of `libraries/`

If you don't find the `libraries` folder in one of the above locations, you can
determine its location using the navigation menu `File > Preferences`. In the
`Settings` tab will be a `Sketchbook location:` field. The `libraries` folder
will be in the folder set in this field.

## Include

To start using the Arduboy library in your own sketches, the  `Arduboy.h` header
must be included in your project. To do so, add the following line to the top of your `.ino` file.

~~~~~~~~~~~~~~~{.cpp}
    #include "Arduboy.h"
~~~~~~~~~~~~~~~

You can have the Arduino IDE add `#include "Arduboy.h"` to your sketch 
automatically by using the navigation menu `Sketch > Include Library > Arduboy`.

## Board Selection

Select the **Leonardo** board as the target platform, or use the following
instructions to add the Arduboy and Arduboy Devkit to the Arduino IDE.

### Add Arduboy Hardware to Arduino IDE

In the Arudino IDE select, `File > Preferences`. In the *Settings* tab there
will be a field titled *Additional Boards Manager URLs:*, put the following in
this field:

    https://arduboy.github.io/board-support/package_arduboy_index.json

See this [guide](http://community.arduboy.com/t/quickly-add-the-arduboy-arduboy-devkit-to-the-arduino-ide/1069) for more detailed instructions.

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

## Custom Arduboy Library Source

If you find that a modified version of the Arduboy Library has been used to
build a game, and you have already installed a release version of the Aruboy
Library, then several options are available to allow you to compile and run the
application without altering the Arduino IDE's libraries.

Use one of the following options to compile sketches that have included copies
of the Aruboy Library,

**Option 1.**

Remove the local `Arduboy.cpp` and `Arduboy.h` files and try recompiling. 
This will only work in some cases.

**Option 2.**

Rename `Arduboy.h` to `CustomArduboy.h` (or a similar name) and add
`#include "CustomArduboy.h"` to the `.ino` sketch file. 
