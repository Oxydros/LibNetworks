#!/bin/sh

apt-get purge cmake || true;

mkdir ~/temp
cd ~/temp
wget https://cmake.org/files/v3.9/cmake-3.9.1.tar.gz
tar -xzvf cmake-3.9.1.tar.gz
cd cmake-3.9.1/
./bootstrap
make -j4
make install
cmake --version