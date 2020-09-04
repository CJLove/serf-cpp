#!/bin/bash
# Driver script to build/test serf-cpp locally in containers under Jenkins and Podman

function ShowUsage()
{
    cat <<EOT
Usage:
    $(basename $0) options
        --image=<image>          - Podman container image to use
        --dir=<directory>        - build directory
        [--podman=<options>]     - option string to pass to Podman
        [--cxx=<path/to/cxx>]    - path for CXX environment variable
        [--cc=<path/to/cc>]      - path for CC env variable
        [--cmake=<options>]      - option string to pass to CMake
EOT
    return 0
}

PARAM_IMAGE=
PARAM_PODMAN=
PARAM_DIR=build
PARAM_CC=
PARAM_CXX=
PARAM_CMAKE=

while test $# -gt 0; do
        param="$1"
        if test "${1::1}" = "-"; then
                if test ${#1} -gt 2 -a "${1::2}" = "--" ; then
                        param="${1:2}"
                else
                        param="${1:1}"
                fi
        else
                break
        fi

        shift

	case $param in
        image=*)
            PARAM_IMAGE=$(echo $param|cut -f2 -d'=')
            ;;
        podman=*)
            PARAM_PODMAN=$(echo $param|cut -f2- -d'=')
            ;;
        dir=*)
            PARAM_DIR=$(echo $param|cut -f2 -d'=')
            ;;
        cc=*)
            PARAM_CC=$(echo $param|cut -f2 -d'=')
            ;;
        cxx=*)
            PARAM_CXX=$(echo $param|cut -f2 -d'=')
            ;;
        cmake=*)
            PARAM_CMAKE=$(echo $param|cut -f2- -d'=')
            ;;
        help|h|?|-?)
            ShowUsage
            exit 0
            ;;
        *)
            echo "Error: Unknown parameter: $param"
            ShowUsage
            exit 2
            ;;
    esac
done

echo "PARAM_IMAGE=$PARAM_IMAGE"
echo "PARAM_PODMAN=$PARAM_PODMAN"
echo "PARAM_DIR=$PARAM_DIR"
echo "PARAM_CC=$PARAM_CC"
echo "PARAM_CXX=$PARAM_CXX"
echo "PARAM_CMAKE=$PARAM_CMAKE"

#
# If image was specified then run the specified container image and call container.sh 
# in the container with the remaining arguments
#
if [ -n "$PARAM_IMAGE" ]; then
    mkdir -p $PARAM_DIR
    rm -rf $PARAM_DIR/*

    PARAM_DIR="--dir=$PARAM_DIR"
    [ -n "$PARAM_CC" ] && PARAM_CC="--cc=$PARAM_CC"
    [ -n "$PARAM_CXX" ] && PARAM_CXX="--cxx=$PARAM_CXX"
    [ -n "$PARAM_CMAKE" ]  && PARAM_CMAKE="--cmake=\"$PARAM_CMAKE\""

    COMMAND="/root/work/container.sh $PARAM_DIR $PARAM_CC $PARAM_CXX $PARAM_CMAKE"

    podman run --rm --pull always $PARAM_PODMAN -v $PWD:/root/work fir.local:3005/$PARAM_IMAGE:latest bash -c "$COMMAND"

    exit $?
fi

#
# Build/run serf-cpp
#
cd /root/work/$PARAM_DIR
if [ -n "$PARAM_CXX" -o -n "$PARAM_CC" ]; then
    # Override CC and CXX
    CC=$PARAM_CC CXX=$PARAM_CXX cmake $PARAM_CMAKE ..
else
    cmake $PARAM_CMAKE ..
fi
ret=$?
[ $ret -ne 0 ] && exit $ret

make
ret=$?
[ $ret -ne 0 ] && exit $ret

./tests/SerfCppTests --gtest_output=xml:unittests.xml
ret=$?

exit $ret