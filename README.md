## develop [![Build status](https://ci.appveyor.com/api/projects/status/p2ddxq76pqmboj74/branch/develop?svg=true)](https://ci.appveyor.com/project/OzanCansel/speech/branch/develop)
## master [![Build status](https://ci.appveyor.com/api/projects/status/p2ddxq76pqmboj74/branch/master?svg=true)](https://ci.appveyor.com/project/OzanCansel/speech/branch/master)


# speech

This project aims to develop a qt-based generic serialization/deserialization mechanism and strongly-typed messaging over network. It is under-development for now.
The library is composed from only headers. It is not a single-header library because of compilation speed concern. Also, we are trying to give compile time errors when something wrong as much as possible.

It is under GNU General Public License.

# CMake
speech library uses CMake build system. We are using modern-cmake approach and paradigm which enforces us to use CMake 3.1x.

# QMake
speech library doesn't use qmake as build system currently but it will be supported at future.

# Dependencies and required(recommended) tools
- Qt5
- CMake
- A compiler which supports C++11 and higher

# Compiler Support
- GCC
- MinGW
- VS Compiler
