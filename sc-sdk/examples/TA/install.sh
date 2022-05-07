#!/bin/bash
cd cloud

# build
mkdir build
cd build
cmake ..
make
cd ..

# install
if [ ! -d"/data" ]; then
    mkdir -p /data
fi

if [ ! -f "/data/f68fd704-6eb1-4d14-b218-722850eb3ef0.sec" ]; then
    rm -rf /data/f68fd704-6eb1-4d14-b218-722850eb3ef0.sec
fi

cp f68fd704-6eb1-4d14-b218-722850eb3ef0.sec /data

# clean temp file
rm -rf build
rm -rf *.sec
rm -rf libcombine.so
rm -rf manifest.txt
cd ..
