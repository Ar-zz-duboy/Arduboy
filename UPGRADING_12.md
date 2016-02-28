Upgrading to 1.2
================

Version 1.2 of the library includes a few large breaking changes.

### AbPrinter

Arduboy no longer subclasses `Print` so no more printing/text operations.  Use the new AbPrinter class instead.

    // Before
    Arduboy arduboy;
    // ...
    arduboy.print("Hello");
    
    // After
    Arduboy arduboy;
    // ...
    AbPrinter text(arduboy);
    text.print("Hello");


### ArduboyTunes

ArduboyTunes is no longer part of the library.  If you want it you can still use it.  You can find it at https://github.com/Arduboy/ArduboyPlayTune.  Look at the Tunes examples for how to use the library now that it's been split into it's own library.

There are also other great choices such as [Squawk](https://github.com/stg/Squawk), etc.  Or you can now use the standard Arduino Tone library without any issues.

We still recommend you use `audio.on()`, `audio.off()`, and `audio.enabled()` to manage the actual state of the audio system.


### Frame rate

The [previous frame rate logic was buggy](https://github.com/Arduboy/Arduboy/pull/115) and would cause your game to run too slow.  

After upgrading your game could possibly run 50-100% faster than before.  You'll need to playtest and make sure you adjust your game timings for the new accurate framerate logic (or set a slower framerate).
