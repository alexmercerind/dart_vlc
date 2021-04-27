# dart_vlc: A media playback library for Dart & Flutter. Based on libVLC & libVLC++.
#
# Hitesh Kumar Saini, Domingo Montesdeoca Gonzalez & contributors.
# https://github.com/alexmercerind
# alexmercerind@gmail.com
# GNU Lesser General Public License v2.1
# 

cmake_minimum_required(VERSION 3.15)

set (CMAKE_CXX_STANDARD 17)

set(PROJECT_NAME dart_vlc)
project(${PROJECT_NAME} LANGUAGES CXX)
set(PLUGIN_NAME dart_vlc_plugin)

set(LIBVLC_VERSION 3.0.9.2)

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

set(LIBVLC_BINARIES ${CMAKE_CURRENT_SOURCE_DIR}/bin)

set(LIBVLCPP_URL https://github.com/videolan/libvlcpp/archive/master.zip)
set(LIBVLCPP_ARCHIVE ${LIBVLC_BINARIES}/libvlcpp.zip)
set(LIBVLCPP_SOURCE ${LIBVLC_BINARIES}/libvlcpp-master)

if (NOT EXISTS ${LIBVLCPP_ARCHIVE})
  file(DOWNLOAD ${LIBVLCPP_URL} ${LIBVLCPP_ARCHIVE})
endif()

add_custom_target(LIBVLC_EXTRACT ALL)
add_custom_command(
  TARGET LIBVLC_EXTRACT PRE_BUILD
  COMMAND ${CMAKE_COMMAND} -E tar xzf \"${LIBVLCPP_ARCHIVE}\"
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${LIBVLCPP_SOURCE}/vlcpp ${CMAKE_CURRENT_SOURCE_DIR}/include/vlcpp
  WORKING_DIRECTORY ${LIBVLC_BINARIES}
  DEPENDS ${LIBVLC_ARCHIVE}
)

target_link_libraries(${PLUGIN_NAME} PRIVATE
  flutter
  PkgConfig::GTK
  "/usr/lib/x86_64-linux-gnu/libvlc.so"
)
set(
  dart_vlc_bundled_libraries
  PARENT_SCOPE
)
