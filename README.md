Arduboy Library
===============

[Arduboy](https://www.arduboy.com/) is an [Arduino](https://www.arduino.cc/) compatible open-source hardware platform allowing gamers to easily make and share games using C/C++. The **Arduboy Library** is an open-source library that provides a programming interface for making games and applications that run on an Arduboy. The Arduboy Library supports both the **production** and **development (DevKit)** Arduboy units.

It is recommended that this library be installed using the Arduino IDE Library Manager. Information on how to do this is included in the [_Quick Start Guide_](http://community.arduboy.com/t/kickstarter-quick-start-guide/725/1) in the [Arduboy community forum](http://community.arduboy.com/).

## Include

To use the Arduboy library in your own sketches, include the `Arduboy.h` header
file. To do so, add the following line to the top of your `.ino` file.

~~~~~~~~~~~~~~~{.cpp}
#include <Arduboy.h>
~~~~~~~~~~~~~~~

You can have the Arduino IDE add `#include <Arduboy.h>` to your sketch 
automatically by using the navigation menu `Sketch > Include Library > Arduboy`

## Repository Branches
A list of important Arduboy branches.

<b>`stable`</b>

Please use the `stable` branch for the current stable build. If you are unsure
which branch to use for your game, then this is the branch you are looking for.

<b>`develop`</b>

The `develop` branch for the Arduboy Library holds the bleeding-edge release
for the Arduboy project. It may contain code that is not stable or not fully
debugged. It may also contain code that is never included in a stable release
or differs from how it becomes implemented in a stable release. Use it at your
own risk and don't use it for sketches that you intend to release for general
use by others.
