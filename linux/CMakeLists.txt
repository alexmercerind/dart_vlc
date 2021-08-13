# dart_vlc: A media playback library for Dart & Flutter. Based on libVLC & libVLC++.
#
# Hitesh Kumar Saini
# https://github.com/alexmercerind
# alexmercerind@gmail.com
# GNU Lesser General Public License v2.1
# 
cmake_minimum_required(VERSION 3.10)
set (CMAKE_CXX_STANDARD 17)

set(PACKAGE dartvlc)
add_library(${PACKAGE} SHARED
  ../ffi/native/dart_vlc.cpp
)

set(LIBVLC_BINARIES ${CMAKE_CURRENT_SOURCE_DIR}/../bin)

set(LIBVLCPP_ARCHIVE ${LIBVLC_BINARIES}/libvlcpp.zip)

set(LIBVLC_PACKAGE_DIR ${CMAKE_BINARY_DIR}/${PLUGIN_NAME}_packages)
set(LIBVLCPP_SOURCE ${LIBVLC_PACKAGE_DIR}/libvlcpp-master)

add_custom_target(LIBVLC_EXTRACT ALL)

if (NOT EXISTS ${LIBVLCPP_SOURCE})
  add_custom_command(
    TARGET LIBVLC_EXTRACT PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E tar xzf \"${LIBVLCPP_ARCHIVE}\"
    WORKING_DIRECTORY ${LIBVLC_PACKAGE_DIR}
    DEPENDS ${LIBVLCPP_ARCHIVE}
  )
endif()

include_directories(${PACKAGE} INTERFACE
  # libVLC++ Headers.
  ${LIBVLCPP_SOURCE}
)

target_include_directories(${PACKAGE} INTERFACE
  # DartVLC Headers.
  ${CMAKE_CURRENT_SOURCE_DIR}/../ffi/native
  # libVLC++ Headers.
  ${LIBVLCPP_SOURCE}
)
target_link_libraries(${PACKAGE} PRIVATE
  vlc
)


# Not used Flutter plugin template.

set(PROJECT_NAME dart_vlc)
project(${PROJECT_NAME} LANGUAGES CXX)
set(PLUGIN_NAME dart_vlc_plugin)
include_directories(${PLUGIN_NAME} INTERFACE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)
add_library(${PLUGIN_NAME} SHARED
  dart_vlc_plugin.cc
)

apply_standard_settings(${PLUGIN_NAME})
set_target_properties(${PLUGIN_NAME} PROPERTIES
  CXX_VISIBILITY_PRESET hidden)
target_compile_definitions(${PLUGIN_NAME} PRIVATE FLUTTER_PLUGIN_IMPL)
target_include_directories(${PLUGIN_NAME} INTERFACE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/../dartvlc
)

target_link_libraries(${PLUGIN_NAME} PRIVATE
  flutter
  PkgConfig::GTK
)

# Add generated shared library.

set(
  dart_vlc_bundled_libraries
  # libdartvlc.so
  plugins/dart_vlc/libdartvlc.so
  PARENT_SCOPE
)
