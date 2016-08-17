set -e
BOOTSTRAP_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
source $BOOTSTRAP_DIR/common.sh

echo "Installing dependencies for building for the Arduino"

if [ -z "$ARDUINO_DIR" ] || ! test -e $ARDUINO_DIR || [ $OS == "cygwin" ]; then

    echo "Installing Arduino..."

    ARDUINO_BASENAME="arduino-nightly"
    if [ $OS == "cygwin" ]; then
        ARDUINO_FILE="$ARDUINO_BASENAME-windows".zip
        EXTRACT_COMMAND="unzip -q"
    elif [ $OS == "mac" ]; then
        ARDUINO_FILE="$ARDUINO_BASENAME-macosx".zip
        EXTRACT_COMMAND="unzip -q"
    else
        ARDUINO_FILE="$ARDUINO_BASENAME-linux64".tar.xz
        EXTRACT_COMMAND="tar xf"
    fi

    ARDUINO_URL=http://arduino.cc/download.php?f=/$ARDUINO_FILE

    _pushd $DEPENDENCIES_FOLDER
    if ! test -e $ARDUINO_FILE
    then
        echo "Downloading Arduino IDE..."
        download $ARDUINO_URL $ARDUINO_FILE
    fi

    if ! test -d $ARDUINO_BASENAME
    then
        echo "Installing Arduino to local folder..."
	echo "$ARDUINO_FILE"
        $EXTRACT_COMMAND $ARDUINO_FILE
        echo "Arduino installed"
    fi

    _popd

fi

echo
echo "${bldgreen}Arduino dependencies installed.$txtrst"
