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


add_subdirectory(../dartvlc "${CMAKE_BINARY_DIR}/${PROJECT_NAME}_core")

# dart_vlc.dll
add_library("${PLUGIN_NAME}" SHARED
  "dart_vlc_plugin.cc"
  "video_outlet.cc"
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
