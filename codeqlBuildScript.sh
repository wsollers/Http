#/bin/sh


echo "Building for CodeQL"
mkdir build
cd build || exit
cmake --build .

