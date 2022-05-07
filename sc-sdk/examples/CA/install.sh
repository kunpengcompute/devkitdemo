#!/bin/sh

cd cloud

# compile
mkdir build
cd build
cmake ..
make

# install
cd ..
if [ ! -d "/vendor/bin" ]; then
    mkdir -p /vendor/bin
fi

rm -rf /vendor/bin/rsa-demo
cp rsa_demo /vendor/bin

# clean
rm -rf rsa_demo
rm -rf build
cd ..
