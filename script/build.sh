#!/bin/sh
set -eux

mkdir -p ./build
cd ./build
cmake -G Ninja ..
ninja -j $(nproc)
cd ..