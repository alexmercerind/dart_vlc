cmake_minimum_required(VERSION 3.15)

set(PROJECT_NAME "flutter_vlc")

project(${PROJECT_NAME} LANGUAGES CXX)

set(PLUGIN_NAME "flutter_vlc_plugin")
include_directories(${PLUGIN_NAME} INTERFACE
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
)
add_library(${PLUGIN_NAME} SHARED
  "flutter_vlc_plugin.cpp"
)
apply_standard_settings(${PLUGIN_NAME})
set_target_properties(${PLUGIN_NAME} PROPERTIES
  CXX_VISIBILITY_PRESET hidden)
target_compile_definitions(${PLUGIN_NAME} PRIVATE FLUTTER_PLUGIN_IMPL)
target_include_directories(${PLUGIN_NAME} INTERFACE
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
)
target_link_libraries(${PLUGIN_NAME} PRIVATE
  flutter
  flutter_wrapper_plugin
  "${CMAKE_CURRENT_SOURCE_DIR}/libs/libvlc.lib"
  "${CMAKE_CURRENT_SOURCE_DIR}/libs/libvlccore.lib"
)
set(
  flutter_vlc_bundled_libraries
  "${CMAKE_CURRENT_SOURCE_DIR}/dll/libvlc.dll"
  "${CMAKE_CURRENT_SOURCE_DIR}/dll/libvlccore.dll"
  PARENT_SCOPE
)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4244")
