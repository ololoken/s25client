# Copyright (C) 2005 - 2025 Settlers Freaks <sf-team at siedler25.org>
#
# SPDX-License-Identifier: GPL-2.0-or-later

include(RttrTestingCfg)
option(RTTR_EXTERNAL_BUILD_TESTING "Enable tests of external dependencies" OFF)
set(BUILD_TESTING ${RTTR_EXTERNAL_BUILD_TESTING})

if(NOT RTTR_USE_SYSTEM_LIBS AND EXISTS ${CMAKE_CURRENT_LIST_DIR}/dev-tools/CMakeLists.txt)
    set(includeDevToolsDefault ON)
else()
    set(includeDevToolsDefault OFF)
endif()

set(includeDevToolsDefault OFF)
if(EXISTS ${CMAKE_CURRENT_LIST_DIR}/dev-tools/CMakeLists.txt AND NOT DEFINED RTTR_INCLUDE_DEVTOOLS)
    if(APPLE) # Lua 5.2 required which is not easily available on OSX
      set(includeDevToolsDefault ON)
    else()
      message(STATUS "Precompiled binaries are available to use for development."
                     "They are not compatible with all platforms though."
                     "Configure with -DRTTR_INCLUDE_DEVTOOLS=ON to make them available to CMake."
              )
    endif()
endif()
option(RTTR_INCLUDE_DEVTOOLS "Include folder with precompiled binaries for development" ${includeDevToolsDefault})
if(RTTR_INCLUDE_DEVTOOLS)
    add_subdirectory(dev-tools)
    set(CMAKE_SYSTEM_PREFIX_PATH ${CMAKE_SYSTEM_PREFIX_PATH} PARENT_SCOPE)
    set(CMAKE_SYSTEM_PROGRAM_PATH ${CMAKE_SYSTEM_PROGRAM_PATH} PARENT_SCOPE)
    set(CMAKE_FIND_ROOT_PATH ${CMAKE_FIND_ROOT_PATH} PARENT_SCOPE)
endif()

# Building the updater when using non-default paths (especially absolute paths)
# is not recommended as it most likely won't work
set(RTTR_BUILD_UPDATER_DEF ON)
foreach(curDirVar BINDIR DATADIR LIBDIR DOCDIR DRIVERDIR)
    if(IS_ABSOLUTE ${RTTR_${curDirVar}})
        set(RTTR_BUILD_UPDATER_DEF OFF)
    endif()
endforeach()
option(RTTR_BUILD_UPDATER "Build auto-updater. Not advised when changing the default paths as the updater is configured specifically for the official builds." ${RTTR_BUILD_UPDATER_DEF})

add_subdirectory(glad)
add_library(kaguya INTERFACE)
target_include_directories(kaguya SYSTEM INTERFACE kaguya/include)
add_subdirectory(libendian)
set(RTTR_BUILD_LOBBY_CLIENT ON)
add_subdirectory(liblobby)

add_subdirectory(libsiedler2)
add_subdirectory(libutil)
add_subdirectory(mygettext)
if (NOT CMAKE_SYSTEM_NAME MATCHES "Emscripten")
add_subdirectory(s25edit)
endif ()
if(RTTR_BUILD_UPDATER)
    add_subdirectory(s25update)
endif()

option(RTTR_USE_SYSTEM_LIBSAMPLERATE "Use system installed libsamplerate. Fails if not found!" "${RTTR_USE_SYSTEM_LIBS}")
if(RTTR_USE_SYSTEM_LIBSAMPLERATE)
    find_package(SampleRate 0.1.9 REQUIRED)
    unset(samplerate_LIB_DEPENDS CACHE)
else()
    find_package(SampleRate 0.1.9)
    if(NOT SampleRate_FOUND)
        message(STATUS "Downloading libsamplerate")
        include(FetchContent)
        FetchContent_Declare(
            SampleRate
            GIT_REPOSITORY https://github.com/libsndfile/libsamplerate
            GIT_TAG        15c392d47e71b9395a759544b3818a1235fe1a1d # 0.2.2 with update of required CMake version
        )
        set(LIBSAMPLERATE_EXAMPLES OFF CACHE INTERNAL "")
        set(LIBSAMPLERATE_INSTALL OFF CACHE INTERNAL "")
        set(old_BUILD_TESTING ${BUILD_TESTING})
        set(BUILD_TESTING OFF)
        FetchContent_MakeAvailable(SampleRate)
        set(BUILD_TESTING ${old_BUILD_TESTING})
    endif()
endif()

# No tests for turtle
set(BUILD_TESTING OFF)
add_subdirectory(turtle)
get_target_property(inc turtle INTERFACE_INCLUDE_DIRECTORIES)
set_target_properties(turtle PROPERTIES INTERFACE_SYSTEM_INCLUDE_DIRECTORIES "${inc}")
