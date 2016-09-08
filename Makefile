# Makefile
#
# Build and upload a sketch using make. Arduino-Makefile must be installed.
# See `test/scripts/arduino.sh` for a working client bootstrap.
#
# The PerformExample.ino is set as the source to compile, and the current
# directory for the project is included as a library in the build process.
#
# the `make` command can be performed after changes to the libary have been
# made in order to test a revision or compile a source file after setting
# LOCAL_INO_SRCS.
# 
# Build a project's hex:
#  make
# Upload a compiled hex:
#  make upload


print_output = $(info $(1))

# Use the PerformExample.ino file as our source to build
TARGET = PerformExample

LOCAL_INO_SRCS := ./examples/$(TARGET)/$(TARGET).ino

# another option, not yet enabled
# LOCAL_INO_SRCS := $(wildcard *.ino) $(wildcard examples/*/*.ino)

# Get all examples in 'examples/'
EXAMPLES = $(sort $(dir $(wildcard ./examples/*/)))

define arduboy_message

Arduboy Library
-------------------------
Other Arduboy Examples:

endef

#$(info $(arduboy_message))

#$(foreach example,$(EXAMPLES:./examples/%/=%),$(info  - $(example)))

# set PROJECT_DIR to the location of the Arduboy library
# example:
#  `git clone -b develop ~/Arduboy-Project/libraries/Arduboy'
PROJECT_DIR := $(realpath $(dir $(realpath $(lastword $(MAKEFILE_LIST)))))

# set ARDUINO_DIR to the location of your Arduino installation
# default setting generated from `test/scripts/arduino.sh`
ARDUINO_DIR = /var/tmp/Arduboy/arduino-nightly

# set to location Arduino-Makefile install
#  https://github.com/sudar/Arduino-Makefile
ARDMK_DIR = /var/tmp/Arduboy/Arduino-Makefile

##############################################################################
# Windows
# Provide a relative path to the Arduino project files
##############################################################################
# get a path from cygpath
cygpath_relative = $(shell cygpath -m -a ${1})
# make two paths relative respecting cygwin paths
cygwin_make_relative = $(shell realpath \
                       --relative-to=$(call cygpath_relative,$(1)) \
                       $(call cygpath_relative,$(ARDUINO_DIR)))

ifeq ($(OS),Windows_NT)
    ARDUINO_DIR := $(call cygwin_make_relative,$(PROJECT_DIR),$(ARDUINO_DIR))
endif

# use the Arduboy boards.txt
BOARDS_TXT = extras/boards.txt

# set our board to arduboy
BOARD_TAG = arduboy

# port to attempt.
#  example:
#   *       - try all ports
#   com3    - Windows
#   ttyACM0 - Linux
#
MONITOR_PORT = *


# Set the user library path one directory higher than our current working
# directory. This allows the Arduboy folder to be added to our source.
USER_LIB_PATH := ..

# Libraries to link include
ARDUINO_LIBS = Arduboy SPI EEPROM

# Required CXXFLAGS
CXXFLAGS_STD = -std=gnu++11 -fno-threadsafe-statics

# Get the current working directory
CURRENT_DIR = $(shell basename $(CURDIR))

# Folder where objects, hexes and other files are generated
OBJDIR = bin/Arduboy

# Ensure we are using the arduino core
CORE = arduino

# Ensure we are using the leonardo variant
VARIANT = leonardo

##############################################################################
# Explictiy include VID and PID in CPPFLAGS. Arduino Makefile workaround.
# TODO The makefile should take these from boards.txt, but may not be including
# them in CPPFLAGS. Related to BOARD_TAG?
##############################################################################

USB_VID = 0x2341
USB_PID = 0x8039

CPPFLAGS = -DUSB_VID=$(USB_VID) -DUSB_PID=$(USB_PID)

# Include the Arduino Makefile
include $(ARDMK_DIR)/Arduino.mk
