cmake_minimum_required(VERSION 3.15)

set(PROJECT_NAME "flutter_vlc")
project(${PROJECT_NAME} LANGUAGES CXX)
set(PLUGIN_NAME "flutter_vlc_plugin")

set(LIBVLC_VERSION "3.0.9.2")

include_directories("${PLUGIN_NAME}" INTERFACE
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
)
add_library("${PLUGIN_NAME}" SHARED
  "flutter_vlc_plugin.cpp"
)

apply_standard_settings("${PLUGIN_NAME}")
set_target_properties("${PLUGIN_NAME}" PROPERTIES
  CXX_VISIBILITY_PRESET hidden)
target_compile_definitions("${PLUGIN_NAME}" PRIVATE FLUTTER_PLUGIN_IMPL)
target_include_directories("${PLUGIN_NAME}" INTERFACE
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

set(LIBVLC_URL "http://download.videolan.org/pub/videolan/vlc/${LIBVLC_VERSION}/win64/vlc-${LIBVLC_VERSION}-win64.zip")
set(LIBVLC_BINARIES "${CMAKE_CURRENT_SOURCE_DIR}/bin")
set(LIBVLC_ZIP "${LIBVLC_BINARIES}/vlc-${LIBVLC_VERSION}.zip")
set(LIBVLC_SOURCE "${LIBVLC_BINARIES}/vlc-${LIBVLC_VERSION}")
if (NOT EXISTS "${LIBVLC_ZIP}")
  file(DOWNLOAD "${LIBVLC_URL}" "${LIBVLC_ZIP}")
endif()
add_custom_target(LIBVLC_EXTRACT ALL)
add_custom_command(
  TARGET LIBVLC_EXTRACT PRE_BUILD
  COMMAND ${CMAKE_COMMAND} -E tar xzf \"${LIBVLC_ZIP}\"
  COMMAND ${CMAKE_COMMAND} -E copy ${LIBVLC_SOURCE}/libvlc.dll ${LIBVLC_BINARIES}/dll/libvlc.dll
  COMMAND ${CMAKE_COMMAND} -E copy ${LIBVLC_SOURCE}/libvlccore.dll ${LIBVLC_BINARIES}/dll/libvlccore.dll
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${LIBVLC_SOURCE}/plugins ${LIBVLC_BINARIES}/dll/plugins
  WORKING_DIRECTORY "${LIBVLC_BINARIES}"
  DEPENDS "${LIBVLC_ZIP}"
)

target_link_libraries("${PLUGIN_NAME}" PRIVATE
  flutter
  flutter_wrapper_plugin
  "${CMAKE_CURRENT_SOURCE_DIR}/bin/lib/libvlc.lib"
  "${CMAKE_CURRENT_SOURCE_DIR}/bin/lib/libvlccore.lib"
)
set(
  flutter_vlc_bundled_libraries
  "${CMAKE_CURRENT_SOURCE_DIR}/bin/dll/libvlc.dll"
  "${CMAKE_CURRENT_SOURCE_DIR}/bin/dll/libvlccore.dll"
  "${CMAKE_CURRENT_SOURCE_DIR}/bin/dll/plugins"
  PARENT_SCOPE
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4244")
