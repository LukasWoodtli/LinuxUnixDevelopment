#!/bin/bash
set -e
set -u

mkdir build
cd build
cmake -G 'Unix Makefiles' ..
make
make test

