# Okra

## Usage

**TODO**

## Building

### Pre-requisites
1. Python (https://www.python.org/)
1. pip (https://pypi.python.org/pypi/pip/)
1. Conan (https://www.conan.io/)
1. CMake (https://cmake.org/)

### Windows - Visual Studio
1. `mkdir build && cd build`
1. `conan install .. -s arch=x86_64 -s build_type=Debug -s os=Windows -s compiler="Visual Studio" -s compiler.version=14 -s compiler.runtime=MDd --build=missing -sc build_samples=True -sc build_tests=True`
1. `conan build ..`

### Linux - GCC
1. `mkdir build && cd build`
1. `conan install .. -s arch=x86 -s build_type=Release -s os=Linux -s compiler=gcc -s compiler.version=4.8 -s compiler.libcxx=libstdc++ --build=missing -sc build_samples=True -sc build_tests=True`
1. `conan build ..`

## Testing package

### Standard package (no tests and no samples)
1. `conan test_package .`

### Development package (with tests and samples)
1. `conan test_package . -s arch=x86_64 -s build_type=Release -s os=Windows -s compiler="Visual Studio" -s compiler.version=12 -s compiler.runtime=MD --build=missing -sc okra:build_samples=True -sc okra:build_tests=True`

## Copyright & License

Copyright @fpascutti 2016.
Distributed under the Boost Software License, Version 1.0.
\(See accompanying file [LICENSE\_1\_0.txt](LICENSE_1_0.txt) or copy at http://www.boost.org/LICENSE_1_0.txt\)
