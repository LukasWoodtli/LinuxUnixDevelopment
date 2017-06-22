#!/bin/bash

set -e
set -u

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

pushd "$DIR/make"
make test
popd

pushd "$DIR/libraries/static"
make test
popd


pushd "$DIR/libraries/dynamic"
make test
popd
