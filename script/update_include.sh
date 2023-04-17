#!/usr/bin/env bash

set -eu

rm -rf include

for header in $(find qulacs -type f -name '*.hpp'); do
    mkdir -p include/$(dirname $header)
    cp $header include/$header
done