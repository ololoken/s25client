# Copyright (C) 2005 - 2024 Settlers Freaks <sf-team at siedler25.org>
#
# SPDX-License-Identifier: GPL-2.0-or-later

# Library containing old files that are still useful for tests
file(GLOB_RECURSE HEADERS src/*.h src/*.hpp)
add_library(legacyTestfiles STATIC src/legacy/TerrainData.cpp ${HEADERS})
target_include_directories(legacyTestfiles INTERFACE src)

target_link_libraries(legacyTestfiles PUBLIC s25Common gamedata)
enable_warnings(legacyTestfiles)