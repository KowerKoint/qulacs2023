#!/usr/bin/env bash

set -eu

find ./qulacs -regex '.*\.\(cu\|cuh\|cpp\|h\|hpp\)' -exec clang-format -style=file -i {} \;