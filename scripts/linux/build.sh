#!/bin/sh

mkdir build;
cd build;

echo "Generating Makefiles"

cmake -DCMAKE_BUILD_TYPE=Debug ..;

echo "Building..."

make;
