# Okra

## Usage

**TODO**

## Development

### Pre-requisites
1. Conan (https://www.conan.io/)
1. CMake (https://cmake.org/)

### Building samples
1. `mkdir build && cd build`
1. `conan install ../samples -s arch=x86_64 -s build_type=Debug -s os=Windows -s compiler="Visual Studio" -s compiler.version=14 -s compiler.runtime=MDd --build=missing`
1. `conan build ../samples`

## Building and running tests
1. `mkdir build && cd build`
1. `conan install ../tests -s arch=x86 -s build_type=Release -s os=Linux -s compiler=gcc -s compiler.version=4.8 -s compiler.libcxx=libstdc++ --build=missing`
1. `conan build ../tests`

## Testing package
1. `conan test_package .`

## Copyright & License

Copyright Franck Pascutt 2016.
Distributed under the Boost Software License, Version 1.0.
\(See accompanying file [LICENSE\_1\_0.txt](LICENSE_1_0.txt) or copy at http://www.boost.org/LICENSE_1_0.txt\)
