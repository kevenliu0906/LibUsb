@echo off

rm -rf build
mkdir build
cd build

::cmake -DCMAKE_CONFIGURATION_TYPES=ReleasePackage ..

cmake -DCMAKE_CONFIGURATION_TYPES=Release -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --config Release -j 8

pause