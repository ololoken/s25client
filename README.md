<!--
Copyright (C) 2005 - 2025 Settlers Freaks <sf-team at siedler25.org>

SPDX-License-Identifier: GPL-2.0-or-later
-->
# emscripten build (wasm)

1. checkout and build `boost-1.86.0` with following command 
```bash
docker run --rm -v $(pwd):/src emscripten-image sh -c './b2 install toolset=emscripten link=static runtime-link=static variant=release -j 1 --prefix=/src/contrib/boost --with-program_options --with-nowide --with-filesystem --with-iostreams --with-locale --with-thread --with-system --with-regex'
```
2. download lua `lua-5.4.7` update config its Makefile add both C and LD flags `-fwasm-exceptions -sSUPPORT_LONGJMP=wasm` and then build
3. run standard build

# Return To The Roots

"Return To The Roots" is a fan-project, which aims to renew the original The Settlers 2.

We aim to extend new features such as a multiplayer mode via internet as well as the support for modern hardware and several operating systems like Windows Vista/Seven/10, Linux and MacOS X.
Likewise we want to invent some smaller upgrades.
Unfortunately it is necessary to rewrite the whole game, but we will stick to the original graphics and sounds, because they are still common and nice to be heard or seen.
So you will still need an original "The Settlers 2 Gold Edition" version to play Return To The Roots.

See more information on <http://www.rttr.info>

## Current Build Info

Build & Tests:
 ![GHA Unit tests](https://github.com/Return-To-The-Roots/s25client/workflows/Unit%20tests/badge.svg)
 ![Static analysis](https://github.com/Return-To-The-Roots/s25client/workflows/Static%20analysis/badge.svg)

Coverage:
[![Coverage Status Coveralls](https://coveralls.io/repos/github/Return-To-The-Roots/s25client/badge.svg?branch=master)](https://coveralls.io/github/Return-To-The-Roots/s25client?branch=master)
 /
[![Coverage Status Codecov](https://codecov.io/gh/Return-To-The-Roots/s25client/branch/master/graph/badge.svg)](https://codecov.io/gh/Return-To-The-Roots/s25client)

## How to install

- Download the game for your OS at [Downloads](https://www.rttr.info/index.php?com=dynamic&mod=2)
  - stable: Usually more stable
  - nightly: Latest features and bug fixes, but might be broken sometimes
- Extract into a folder of your choice
- Locate the file `put your S2-Installation in here` in that folder (usually at the root or in S2)
- Copy the DATA and GFX folder from the original The Settlers II Gold into the folder containing the above file
- Start `rttr.bat`/`rttr.sh` or the bundle (OSX only) to auto-update and start the game
  - Alternatively start `s25client` directly, but updates and music might be missing
- WARNING: Do not use symlinks/junction points/... for subfolders of your installation.
Putting RttR in a symlinked folder should work though.

## How to build

### Development tools / Prebuilt libraries

The `dev-tools` submodule contains prebuild binaries for various platforms that can be used to quickly start building.  
However they are not compatible with all systems, e.g. not with musl libc, so they are not used by default.
You can configure (run CMake) with `-DRTTR_INCLUDE_DEVTOOLS=ON` to allow CMake to find them.

### On Linux or Darwin/MacOSX

#### Prerequisite Linux

- C++14 compatible compiler (e.g. GCC-6)
- cmake
- git
- libboost-dev (at least v1.69.0, i.e <http://www.boost.org/>)
  or only: libboost-test-dev libboost-locale-dev, libboost-iostreams-dev, libboost-filesystem-dev, libboost-program-options-dev (at least v1.69.0)
- libsdl2-dev
- libsdl2-mixer-dev
- libcurl-dev (in libcurl4-openssl-dev)
- libbz2-dev
- lua5.2-dev (Lua 5.1 to 5.3 are supported)
- gettext
- libminiupnpc-dev

All of them can be installed with the package manager.

#### Prerequisite MacOSX

- cmake
- git
- boost
- sdl2
- sdl2_mixer
- gettext (make sure it is in your path with e.g. `brew link --force gettext`)
- miniupnpc
- lua 5.1-5.3 (available via the `dev-tools` submodule included by default)

All of them can be installed via homebrew

#### Prerequisites with Nix

Nix users can open a nix-shell to get a development environment with all packages ready.

#### Checkout and build

```bash
git clone --recursive https://github.com/Return-To-The-Roots/s25client s25client
cd s25client
nix-shell # Optional, for Nix users only
mkdir -p build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
```

Note: by using the `-G` option of `cmake` you can specify a generator, e.g. `cmake -G Xcode -DCMAKE_BUILD_TYPE=Release ..` will generate an Xcode project.
Please check `cmake --help` for more options.

### On Windows

#### Prerequisites

- cmake (i.e from <http://www.cmake.org/download/>)
- boost (i.e from <http://www.boost.org/>)
- Visual Studio (at least 2017, you can get the community edition for free)
- Git Client (i.e TortoiseGit)

#### Steps

- Clone GIT Repository from <https://github.com/Return-To-The-Roots/s25client>
  - Using Git bash:

     ```bash
     git clone --recursive https://github.com/Return-To-The-Roots/s25client s25client
     ```

  - **OR** using TortoiseGit:
    - Rightclick -> "Git clone..."
    - Put in <https://github.com/Return-To-The-Roots/s25client> as URL
    - Select "Directory" to clone to
    - press OK
    - Rightclick on the newly created folder -> TortoiseGit-> Submodule Update
    - Make sure all modules are selected and "Initialize submodules (--init)" is checked
    - press OK
- If you haven't installed boost, install boost
  Fast Way:
  - extract boost-1.69.zip (i.e to external/boost, so that external/boost/bootstrap.bat exist)
  - run that "bootstrap.bat"
  - run created "b2.exe": this should build boost
    Notice: if you have multiple Visual Studio versions installed, use the latest one with the "toolset"-parameter.
    i.e "b2 toolset=msvc-14.0" for Visual Studio 2015
  - run "b2 install --prefix=%CD%": this should install everything so the system can find it
    Notice: Don't forget to add the toolset parameter if you already used it before
- Use cmake-gui:
  - "Where is the source code": Select checked out directory
  - "Where to build the binaries": Select "build" directory (create if required)
  - Press configure
  - Select your compiler version (i.e Visual Studio 2015 x64)
  - If it can't find boost:
    - "Add Entry"
    - Enter as "Name" "BOOST_ROOT" (exact casing!)
    - Select for "Type" "PATH"
    - Enter boost installation path for "Value"
    - Press ok
  - Press generate
- Open and use build/RTTR.sln

### Optimizations

There are various CMake options to control the build and optimization including ARM (Raspberry PI etc.) related ones.
Examples:

- RTTR_ENABLE_OPTIMIZATIONS/RTTR_ENABLE_WERROR
- RTTR_OPTIMIZATION_VECTOR_EXT (Non-Windows x86/x64 only)
- RTTR_OPTIMIZATION_TUNE (Non-Windows only)
- RTTR_TARGET_BOARD (ARM only)
See the description in CMake-GUI/ccmake for details.

Note that due to the use of submodules you always need to `git pull && git submodule update --init --recursive` to get the latest version.
(The `--init` and `--recursive` arguments are only required should we add *new* submodules to the existing set.)

### Tests

Especially for developing you should build in Debug mode (`-DCMAKE_BUILD_TYPE=Debug`) and run the tests after executing `make` via `make test` or `ctest --output-on-failure`.
There is also an option to enable checks for undefined behavior (UBSAN) and memory errors (ASAN) like use-after-free or leaks.
Just pass `-DRTTR_ENABLE_SANITIZERS=ON` to CMake and use a recent GCC or Clang compiler to build.
Then just run (tests or application) as usual.


--

For advanced info or help see [FAQ in the wiki](https://github.com/Return-To-The-Roots/s25client/wiki/How-to-install-RttR) or <http://www.rttr.info>
