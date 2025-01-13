#!/bin/bash

# commands:
# ./build.sh source [--mt] [--bmp] -> to generate the obj files in bin_int and exe file in bin
# ./build.sh clear -> to delete bin_int and bin
# ./build.sh clear_bin -> to delete bin
# ./build.sh clear_bin_int -> to delete bin_int
# ./build.sh info -> debugging information
# ./build.sh render -> to render the bin/EXE

# Checking dependencies
if ! command -v clang &> /dev/null; then
    echo "clang is not installed"
    exit 1
fi

if ! clang --version &> /dev/null; then
    echo "clang installation is not properly configured"
    exit 1
fi

# Building or cleaning test files
case "$1" in
    "source")
        make -f build.mak clear_bin CODE_DIRS="source" BIN_INT_DIR="linux" ARCH="$(uname -m)"
        
        DEFINE_FLAGS=""
        
        for arg in "$@"; do
            case "$arg" in
                "--mt")
                    DEFINE_FLAGS="$DEFINE_FLAGS -DMULTITHREADING"
                    ;;
                "--bmp")
                    DEFINE_FLAGS="$DEFINE_FLAGS -DBMP"
                    ;;
            esac
        done
        
        make -f build.mak all CODE_DIRS="source" BIN_INT_DIR="linux" ARCH="$(uname -m)" DEFINE_FLAGS="$DEFINE_FLAGS"
        ;;
        
    "info")
        make -f build.mak info CODE_DIRS="source" BIN_INT_DIR="linux" ARCH="$(uname -m)"
        ;;
        
    "clear")
        make -f build.mak clear CODE_DIRS="source" BIN_INT_DIR="linux" ARCH="$(uname -m)"
        ;;
        
    "clear_bin")
        make -f build.mak clear_bin CODE_DIRS="source" BIN_INT_DIR="linux" ARCH="$(uname -m)"
        ;;
        
    "clear_bin_int")
        make -f build.mak clear_bin_int CODE_DIRS="source" BIN_INT_DIR="linux" ARCH="$(uname -m)"
        ;;
        
    "render")
        if [ -f "./bin/EXE" ]; then
            ./bin/EXE "$2"
        else
            echo "EXE not found in bin directory"
            exit 1
        fi
        ;;
        
    *)
        echo "Invalid command. Available commands:"
        echo "  source [--mt] [--bmp]"
        echo "  clear"
        echo "  clear_bin"
        echo "  clear_bin_int"
        echo "  info"
        echo "  run"
        exit 1
        ;;
esac

exit 0