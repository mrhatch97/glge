glge
======
[![pipeline status](https://gitlab.com/mrhatch/glge/badges/master/pipeline.svg)](https://gitlab.com/mrhatch/glge/commits/master) [Doxygen Reference](https://mrhatch.gitlab.io/glge)

glge is a library providing high-level abstractions for 3D graphics rendering, 
as well as utilities for tasks common to applications with 3D graphics. It 
aims to abstract away driver-level details and allow clients to work at a
higher level, producing cleaner and more semantic code. This is accomplished
by using modern C++ best practices to wrap typically C-like driver APIs.

glge is currently a work in progress. It supports only OpenGL as its driver,
and uses GLEW for its extension manager.

## Installation
Building glge requires [CMake](https://cmake.org/download/) version 3.13 or higher, 
a C++ compiler supporting [C++17](https://en.cppreference.com/w/cpp/compiler_support),
[glm](https://glm.g-truc.net/0.9.9/index.html), 
[Simple OpenGL Image Library (SOIL)](https://www.lonesock.net/soil.html), and
[GLEW](http://glew.sourceforge.net/).

glge is built as a static library. The build also provides a CMake config
file to allow easily adding glge to your own CMake project.

CMake options used by glge:
* BUILD_DOCUMENTATION - glge will automatically build Doxygen docs if Doxygen 
is available and this option is not set. 
* BUILD_TESTING - glge will build its unit test suite by default.
* GLGE_HEADLESS_TESTS - Configure glge's OpenGL unit tests to run without an 
X server (Linux) or window. Requires EGL and a GLEW install compiled with 
EGL support.
* GLGE_DRIVER - The rendering driver used by glge. Currently only `OPENGL` is supported.

### Linux
CMake should be able to detect the installation of the requisite libraries 
automatically. Run the install target to install the library, headers,
and config file to the desired location.

### Windows
The path to the requisite libraries must be manually specified. Use 
`CMAKE_PREFIX_PATH` and `CMAKE_LIBRARY_PATH` to help CMake find the
installation locations of the dependent libraries. Otherwise 
identical to a Linux install.

## Usage
See the online [Doxygen reference](https://mrhatch.gitlab.io/glge/) 
for information on the provided API.
