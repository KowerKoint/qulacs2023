#!/bin/env sh
set -eux

mkdir -p ./build
cd ./build
cmake -G Ninja ..
ninja -j 4
cd ..