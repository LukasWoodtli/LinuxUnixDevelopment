set -u
set -e

mkdir build
pushd build
cmake -G 'Unix Makefiles' ..
make
make test
popd

# makefile example
pushd 17_Tools/make
make
./mytest
