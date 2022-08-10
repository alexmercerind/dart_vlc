cmake_minimum_required(VERSION 3.13)
set(CMAKE_CXX_STANDARD 17)

if(UNIX AND NOT APPLE)
  set(LINUX TRUE)
endif()

set(LIBRARY_NAME dart_vlc_core)
project(${LIBRARY_NAME} LANGUAGES C CXX)

set(PACKAGE_DIR "${CMAKE_BINARY_DIR}/${LIBRARY_NAME}_packages")

set(LIBVLC_VERSION "3.0.17.4")
set(DART_API_VERSION "b83a96981f2cbc67010c0045402dbab3c0417ade")
set(LIBVLCPP_VERSION "acb78a24d95029f7b4a5e5e68cbf464bbc76c63d")

set(LIBVLC_ARCHIVE "${CMAKE_CURRENT_SOURCE_DIR}/../bin/vlc-${LIBVLC_VERSION}.7z")
set(DART_API_ARCHIVE "${CMAKE_CURRENT_SOURCE_DIR}/../bin/dart_api-${DART_API_VERSION}.zip")
set(LIBVLCPP_ARCHIVE "${CMAKE_CURRENT_SOURCE_DIR}/../bin/libvlcpp-${LIBVLCPP_VERSION}.zip")

set(LIBVLC_SOURCE "${PACKAGE_DIR}/vlc-${LIBVLC_VERSION}")
set(DART_API_SOURCE "${PACKAGE_DIR}/dart_api-master")
set(LIBVLCPP_SOURCE "${PACKAGE_DIR}/libvlcpp-master")


add_custom_target(LIBVLC_EXTRACT ALL)
add_custom_target(DART_API_EXTRACT ALL)
add_custom_target(LIBVLCPP_EXTRACT ALL)

get_directory_property(_HAS_PARENT_DIRECTORY PARENT_DIRECTORY)
if(NOT _HAS_PARENT_DIRECTORY)
  # Indicates that this is a top-level CMake project.
  set(IS_STANDALONE TRUE)
endif()

if (NOT EXISTS "${LIBVLC_SOURCE}")
  if(NOT WIN32)
    # Headers only.
    set(LIBVLC_FILES sdk)
  else()
    if(DARTVLC_VLC_PLUGINS)
      list(TRANSFORM DARTVLC_VLC_PLUGINS PREPEND "plugins/")
    else()
      # Extract the whole plugins directory
      # by default
      set(DARTVLC_VLC_PLUGINS plugins)
    endif()

    set(LIBVLC_FILES
      sdk
      libvlc.dll
      libvlccore.dll
      ${DARTVLC_VLC_PLUGINS}
    )
  endif()

  set(LIBVLC_ARCHIVE_ROOT "vlc-${LIBVLC_VERSION}/")
  list(TRANSFORM LIBVLC_FILES PREPEND ${LIBVLC_ARCHIVE_ROOT})

  file(MAKE_DIRECTORY ${PACKAGE_DIR})
  add_custom_command(
    TARGET LIBVLC_EXTRACT PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E tar xzf \"${LIBVLC_ARCHIVE}\" -- ${LIBVLC_FILES}
    WORKING_DIRECTORY "${PACKAGE_DIR}"
    DEPENDS "${LIBVLC_ARCHIVE}" "${LIBVLCPP_ARCHIVE}"
  )
  add_custom_command(
    TARGET DART_API_EXTRACT PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E tar xzf \"${DART_API_ARCHIVE}\" 
    WORKING_DIRECTORY "${PACKAGE_DIR}"
    DEPENDS "${DART_API_ARCHIVE}"
  )
  add_custom_command(
    TARGET LIBVLCPP_EXTRACT PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E tar xzf \"${LIBVLCPP_ARCHIVE}\"
    WORKING_DIRECTORY "${PACKAGE_DIR}"
    DEPENDS "${LIBVLC_ARCHIVE}" "${LIBVLCPP_ARCHIVE}"
  )
endif()

add_library(${LIBRARY_NAME} STATIC
  core.cc
  api/api.cc
  player/player.cc
  player/player_state.cc
  media_source/media.cc
  media_source/playlist.cc
  equalizer/equalizer.cc
  devices/devices.cc
  broadcast/broadcast.cc
  chromecast/chromecast.cc
  record/record.cc
)

add_dependencies(${LIBRARY_NAME} LIBVLC_EXTRACT)

set_target_properties(${LIBRARY_NAME} PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(${LIBRARY_NAME} PRIVATE
  # dart_vlc wrapper headers.
  "${CMAKE_CURRENT_SOURCE_DIR}"
  # libVLC++ headers.
  "${LIBVLCPP_SOURCE}"
  # libVLC headers.
  "${LIBVLC_SOURCE}/sdk/include"
  # Dart API headers.
  "${DART_API_SOURCE}"
)

target_include_directories(${LIBRARY_NAME} INTERFACE
  # libVLC++ headers.
  "${LIBVLCPP_SOURCE}"
  # libVLC headers.
  "${LIBVLC_SOURCE}/sdk/include"
)

# Windows
if(WIN32)
  set_target_properties(${LIBRARY_NAME} PROPERTIES CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
  target_link_libraries(${LIBRARY_NAME} PRIVATE
    "${LIBVLC_SOURCE}/sdk/lib/libvlc.lib"
    "${LIBVLC_SOURCE}/sdk/lib/libvlccore.lib"
  )

  # Add generated shared library & libVLC DLLs.
  set(DARTVLC_CORE_LIBS
    # In case we decide to build this as a shared library
    #"$<TARGET_FILE:dart_vlc_core>"
    "${LIBVLC_SOURCE}/libvlc.dll"
    "${LIBVLC_SOURCE}/libvlccore.dll"
    "${LIBVLC_SOURCE}/plugins"
    PARENT_SCOPE
  )

# Linux
elseif(LINUX)
  # Build with -fPIC
  set_target_properties(${LIBRARY_NAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)
  # Link against platform-provided libvlc
  target_link_libraries(${LIBRARY_NAME} PRIVATE
    "vlc"
  )
endif()

# If this is the top-level CMake project (e.g. on macOS where this is being run
# by a CocoaPods script phase) we "install" the library directly
if(IS_STANDALONE)
  install(TARGETS ${LIBRARY_NAME})
else()
  set(DARTVLC_CORE_PATH ${CMAKE_CURRENT_SOURCE_DIR} PARENT_SCOPE)
endif()

