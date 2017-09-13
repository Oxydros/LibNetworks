#!/bin/sh

mkdir build;
cd build;

echo "Generating Makefiles"

cmake ..;

echo "Building..."

make;
