# dart_vlc: A media playback library for Dart & Flutter. Based on libVLC & libVLC++.
#
# Hitesh Kumar Saini
# https://github.com/alexmercerind
# alexmercerind@gmail.com
# GNU Lesser General Public License v2.1
# 

cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_STANDARD 17)

set(PROJECT_NAME "dart_vlc")
set(PLUGIN_NAME "dart_vlc_plugin")
project(${PROJECT_NAME} LANGUAGES CXX)

set(LIBVLC_VERSION "3.0.9.2")

set(LIBVLC_BINARIES "${CMAKE_CURRENT_SOURCE_DIR}/../bin")

set(LIBVLC_ARCHIVE "${LIBVLC_BINARIES}/vlc-${LIBVLC_VERSION}.7z")
set(LIBVLCPP_ARCHIVE "${LIBVLC_BINARIES}/libvlcpp.zip")

set(LIBVLC_PACKAGE_DIR "${CMAKE_BINARY_DIR}/${PLUGIN_NAME}_packages")
set(LIBVLC_SOURCE "${LIBVLC_PACKAGE_DIR}/vlc-${LIBVLC_VERSION}")
set(LIBVLCPP_SOURCE "${LIBVLC_PACKAGE_DIR}/libvlcpp-master")

add_custom_target(LIBVLC_EXTRACT ALL)

if (NOT EXISTS "${LIBVLC_SOURCE}" OR NOT EXISTS "${LIBVLCPP_SOURCE}")
  file(MAKE_DIRECTORY ${LIBVLC_PACKAGE_DIR})
  add_custom_command(
    TARGET LIBVLC_EXTRACT PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E tar xzf \"${LIBVLC_ARCHIVE}\"
    COMMAND ${CMAKE_COMMAND} -E tar xzf \"${LIBVLCPP_ARCHIVE}\"
    WORKING_DIRECTORY "${LIBVLC_PACKAGE_DIR}"
    DEPENDS "${LIBVLC_ARCHIVE}" "${LIBVLCPP_ARCHIVE}"
  )
endif()

# dart_vlc.dll
add_library("${PLUGIN_NAME}" SHARED
  "dart_vlc.cc"
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

# Include dart_vlc, FFI, plugin, libVLC++ & libVLC headers.
include_directories(
  "${PLUGIN_NAME}" INTERFACE
  # dart_vlc wrapper headers.
  "${CMAKE_CURRENT_SOURCE_DIR}/../dartvlc"
  # FFI interface headers.
  "${CMAKE_CURRENT_SOURCE_DIR}/../ffi"
  # Plugin interface headers.
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
  # libVLC++ headers.
  "${LIBVLCPP_SOURCE}"
  # libVLC headers.
  "${LIBVLC_SOURCE}/sdk/include"
)

target_include_directories(
  "${PLUGIN_NAME}" INTERFACE
  # dart_vlc wrapper headers.
  "${CMAKE_CURRENT_SOURCE_DIR}/../dartvlc"
  # FFI interface headers.
  "${CMAKE_CURRENT_SOURCE_DIR}/../ffi"
  # Plugin interface headers.
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
  # libVLC++ headers.
  "${LIBVLCPP_SOURCE}"
  # libVLC headers.
  "${LIBVLC_SOURCE}/sdk/include"
)

# Link libVLC & Flutter embedder.
target_link_libraries("${PLUGIN_NAME}" PRIVATE
  flutter
  flutter_wrapper_plugin
  "${LIBVLC_SOURCE}/sdk/lib/libvlc.lib"
  "${LIBVLC_SOURCE}/sdk/lib/libvlccore.lib"
)

# Add generated shared library & libVLC DLLs.
set(
  dart_vlc_bundled_libraries
  "${LIBVLC_SOURCE}/libvlc.dll"
  "${LIBVLC_SOURCE}/libvlccore.dll"
  "${LIBVLC_SOURCE}/plugins"
  PARENT_SCOPE
)

# Disabled few build warnings.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4244 /wd4996")
