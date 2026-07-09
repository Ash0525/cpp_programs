#!/bin/bash
export LD_LIBRARY_PATH="$PWD/lib:$LD_LIBRARY_PATH"
export PATH="$PWD/lib:$PATH"
./run.exe
