set -e

_pushd $DEPENDENCIES_FOLDER
if ! test -d $DEPENDENCIES_FOLDER/Arduino-Makefile
then
    echo "Creating Arduino-Makefile"
    git_url=https://github.com/sudar/Arduino-Makefile.git
    git clone $git_url
fi
_popd
