Arduboy
=======

This library provides the core functionality for the Aruboy.


## How to use the library

Install by cloning the repository.
```	
$ git clone https://github.com/Arduboy/Arduboy.git
```

The library should be installed into your user's Arduino `libraries` directory. Refer to the following list for the location of the Arduino `libraries` folder.

Linux
```
/home/username/arduino/libraries
```
Mac
```
/Users/username/Documents/Arduino/libraries
```
Windows
```
C:\Users\username\My Documents\Arduino\libraries
```

### Board Selection
Select the **Leonardo** board as the target platform.

### Examples
Examples like games and how to play a small composition can be found in the `examples` directory.

#### Play
Find and play an example by opening it through the Arduino IDE, compiling, and uploading the example to the Arduboy.
Examples can be found in the Arduino IDE in the navigation menu under, `File > Examples > Arduboy > Example_Name`.


### Sketches Already Including the Arduboy Library

Sketches which include copies of the Arduboy library may not compile if the Arduboy library has been installed system wide. In these cases the Arduino compiler will try and link the system Arduboy library source with the local header file causing compilation errors if the local library source differs from the system's Arduboy source.

To compile sketches including local copies of the Aruboy Library,

- Remove the local `Arduboy.cpp` and `Arduboy.h` files and try recompiling. This will only work in some cases.

**OR**

- Rename `Arduboy.h` to `CustomArduboy.h` (or a similar name) and add `#include "CustomArduboy.h"` to the `.ino` sketch file. 


