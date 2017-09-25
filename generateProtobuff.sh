#!/bin/sh

cd proto && protoc --cpp_out=$PWD *.proto;
