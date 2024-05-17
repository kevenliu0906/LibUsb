#!/bin/bash

rm -rf build_linux
mkdir build_linux
cd build_linux
cmake -DCMAKE_CONFIGURATION_TYPES=Release -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --config Release -j 8
