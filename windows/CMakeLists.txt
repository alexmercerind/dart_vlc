# This file is a part of dart_vlc (https://github.com/alexmercerind/dart_vlc)
#
# Copyright (C) 2021-2022 Hitesh Kumar Saini <saini123hitesh@gmail.com>
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 3 of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program; if not, write to the Free Software Foundation,
# Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_STANDARD 17)

set(PROJECT_NAME "dart_vlc")
set(PLUGIN_NAME "dart_vlc_plugin")
project(${PROJECT_NAME} LANGUAGES CXX)


add_subdirectory(../core "${CMAKE_BINARY_DIR}/${PROJECT_NAME}_core")

# dart_vlc.dll
add_library("${PLUGIN_NAME}" SHARED
  "dart_vlc_plugin.cc"
  "video_outlet.cc"
  "win32_window.cc"
)

apply_standard_settings("${PLUGIN_NAME}")
set_target_properties(
  "${PLUGIN_NAME}" PROPERTIES
  CXX_VISIBILITY_PRESET hidden
)
target_compile_definitions(
  "${PLUGIN_NAME}" PRIVATE
  FLUTTER_PLUGIN_IMPL
)

target_include_directories(${PLUGIN_NAME} INTERFACE
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_include_directories(${PLUGIN_NAME} PRIVATE
  "${DARTVLC_CORE_PATH}"
)

# Link libVLC & Flutter embedder.
target_link_libraries("${PLUGIN_NAME}" PRIVATE
  flutter
  flutter_wrapper_plugin
  dart_vlc_core
)

# We're currently building the core library as a static
# library. So we need to force exporting all symbols.
target_link_options("${PLUGIN_NAME}" PRIVATE "/WHOLEARCHIVE:dart_vlc_core")

# Add generated shared library & libVLC DLLs.
set(
  dart_vlc_bundled_libraries
  # the core library dependencies
  ${DARTVLC_CORE_LIBS}
  PARENT_SCOPE
)

# Disabled few build warnings.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4244 /wd4996")
