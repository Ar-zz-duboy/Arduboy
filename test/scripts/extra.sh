#!/usr/bin/env bash

_pushd $DEPENDENCIES_FOLDER
if ! test -d $ARDUINO_MAKEFILE_FOLDER
then
    echo "Creating Arduino-Makefile"
    git_url=https://github.com/sudar/Arduino-Makefile.git
    git clone $git_url
fi
_popd
