# Build instructions

## Prerequisites
1. Download and install [CMake](https://cmake.org/download/).

2. A compiler with C++14 support.

3. If you are already cloned the repository, make sure a recursive clone of the project is done to download all submodules.
If you already cloned the repository without specifying `--recursive` you can initialize and checkout
the required submodules with the following git command:
```bash
$ git submodule update --init --recursive
```

4. Windows: Download and install [Visual Studio 2017](https://www.visualstudio.com/downloads/)

5. Linux: Install 'gcc-c++'.

6. Linux: Install the 'make' utility.

7. OS X: Install XCode.

8. OS X: Install XCode command-line tools.

## Windows (Visual Studio 2017)
```bat
:: Clone the project.
$ git clone --recursive https://github.com/alkurbatov/cpp-sc2.git

:: Enter the working directory.
$ cd cpp-sc2

:: Create build directory.
$ mkdir build
$ cd build

:: Create Visual Studio project files.
$ cmake ../ -G "Visual Studio 15 2017 Win64"

:: Build the project using Visual Studio.
$ start cpp-sc2.sln
```

## Windows (nmake)
```bat
:: Clone the project.
$ git clone --recursive https://github.com/alkurbatov/cpp-sc2.git

:: Enter the working directory.
$ cd cpp-sc2

:: Create build directory.
$ mkdir build
$ cd build

:: Create makefiles.
$ cmake ../ -G "NMake Makefiles"

:: Build the project with nmake.
$ nmake
```

## Mac (Xcode)
```bash
# Clone the project.
$ git clone --recursive https://github.com/alkurbatov/cpp-sc2.git

# Enter the working directory.
$ cd cpp-sc2

# Create build directory.
$ mkdir build
$ cd build

# Create Xcode project files.
$ cmake ../ -G Xcode

# Build the project using Xcode.
$ open cpp-sc2.xcodeproj/
```

## Mac (make)
```bash
# Clone the project.
$ git clone --recursive https://github.com/alkurbatov/cpp-sc2.git

# Enter the working directory.
$ cd cpp-sc2

# Create build directory.
$ mkdir build
$ cd build

# Create makefiles.
$ cmake ../

# Build the project with make.
$ make
```

## Linux (make)
```bash
# Clone the project.
$ git clone --recursive https://github.com/alkurbatov/cpp-sc2.git

# Enter the working directory.
$ cd cpp-sc2

# Create build directory.
$ mkdir build
$ cd build

# Create makefiles.
$ cmake ../

# Build the project with make.
$ make
```

## Linux (gmake)
```bash
# Clone the project.
$ git clone --recursive https://github.com/alkurbatov/cpp-sc2.git

# Enter the working directory.
$ cd cpp-sc2

# Create build directory.
$ mkdir build
$ cd build

# Create makefiles.
$ cmake ../ -G "Unix Makefiles"

# Build the project with gmake.
$ gmake
```
