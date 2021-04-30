# dart_vlc: A media playback library for Dart & Flutter. Based on libVLC & libVLC++.
#
# Hitesh Kumar Saini
# https://github.com/alexmercerind
# alexmercerind@gmail.com
# GNU Lesser General Public License v2.1
# 

cmake_minimum_required(VERSION 3.15)

set(PROJECT_NAME "dart_vlc")
project(${PROJECT_NAME} LANGUAGES CXX)
set(PLUGIN_NAME "dart_vlc_plugin")

set(LIBVLC_VERSION "3.0.9.2")

include_directories("${PLUGIN_NAME}" INTERFACE
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
)
add_library("${PLUGIN_NAME}" SHARED
  "dart_vlc_plugin.cpp"
)

apply_standard_settings("${PLUGIN_NAME}")
set_target_properties("${PLUGIN_NAME}" PROPERTIES
  CXX_VISIBILITY_PRESET hidden)
target_compile_definitions("${PLUGIN_NAME}" PRIVATE FLUTTER_PLUGIN_IMPL)
target_include_directories("${PLUGIN_NAME}" INTERFACE
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
  "${CMAKE_CURRENT_SOURCE_DIR}/../dartvlc"
)

set(LIBVLC_BINARIES "${CMAKE_CURRENT_SOURCE_DIR}/bin")


set(LIBVLC_URL "https://mirror.netcologne.de/videolan.org/vlc/${LIBVLC_VERSION}/win64/vlc-${LIBVLC_VERSION}-win64.7z")
set(LIBVLCPP_URL "https://hub.fastgit.org/videolan/libvlcpp/archive/master.zip")
set(LIBVLC_ARCHIVE "${LIBVLC_BINARIES}/vlc-${LIBVLC_VERSION}.7z")
set(LIBVLCPP_ARCHIVE "${LIBVLC_BINARIES}/libvlcpp.zip")
set(LIBVLC_SOURCE "${LIBVLC_BINARIES}/vlc-${LIBVLC_VERSION}")
set(LIBVLCPP_SOURCE "${LIBVLC_BINARIES}/libvlcpp-master")


if (NOT EXISTS "${LIBVLC_ARCHIVE}")
  file(DOWNLOAD "${LIBVLC_URL}" "${LIBVLC_ARCHIVE}")
endif()

if (NOT EXISTS "${LIBVLCPP_ARCHIVE}")
  file(DOWNLOAD "${LIBVLCPP_URL}" "${LIBVLCPP_ARCHIVE}")
endif()

add_custom_target(LIBVLC_EXTRACT ALL)
add_custom_command(
  TARGET LIBVLC_EXTRACT PRE_BUILD
  COMMAND ${CMAKE_COMMAND} -E tar xzf \"${LIBVLC_ARCHIVE}\"
  COMMAND ${CMAKE_COMMAND} -E tar xzf \"${LIBVLCPP_ARCHIVE}\"
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${LIBVLC_SOURCE}/sdk/include/vlc ${CMAKE_CURRENT_SOURCE_DIR}/include/vlc
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${LIBVLCPP_SOURCE}/vlcpp ${CMAKE_CURRENT_SOURCE_DIR}/include/vlcpp
  WORKING_DIRECTORY "${LIBVLC_BINARIES}"
  DEPENDS "${LIBVLC_ARCHIVE}"
)

target_link_libraries("${PLUGIN_NAME}" PRIVATE
  flutter
  flutter_wrapper_plugin
  "${LIBVLC_SOURCE}/sdk/lib/libvlc.lib"
  "${LIBVLC_SOURCE}/sdk/lib/libvlccore.lib"
)
set(
  dart_vlc_bundled_libraries
  "${LIBVLC_SOURCE}/libvlc.dll"
  "${LIBVLC_SOURCE}/libvlccore.dll"
  "${LIBVLC_SOURCE}/plugins"
  PARENT_SCOPE
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4244 /wd4996")
