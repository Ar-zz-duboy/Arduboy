#!/usr/bin/env bash

set -e

SCRIPTS_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
pushd $SCRIPTS_DIR

source $SCRIPTS_DIR/arduino.sh
source $SCRIPTS_DIR/extra.sh

printf "\nDependencies installed to: $DEPENDENCIES_FOLDER\n\n"

printf "Edit your Makefile to include the following lines:\n\n"

ORANGE="\033[1;33m"
RESET="\033[m"

bldyllw=$(tput bold)$(tput setaf 3)

printf " ${bldyllw}ARDUINO_DIR$txtrst=${DEPENDENCIES_FOLDER}/${ARDUINO_BASENAME}\n"
printf " ${bldyllw}ARDMK_DIR$txtrst=${DEPENDENCIES_FOLDER}/Arduino-Makefile\n"

printf "\n"

echo "${bldgreen}Arduboy project ready!$txtrst"
