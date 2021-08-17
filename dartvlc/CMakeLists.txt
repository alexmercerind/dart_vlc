cmake_minimum_required(VERSION 3.13)
set(CMAKE_CXX_STANDARD 17)

if(UNIX AND NOT APPLE)
  set(LINUX TRUE)
endif()

set(LIBRARY_NAME dart_vlc_core)
project(${LIBRARY_NAME} LANGUAGES C CXX)

set(LIBVLC_VERSION "3.0.9.2")
set(LIBVLC_BINARIES "${CMAKE_CURRENT_SOURCE_DIR}/../bin")

set(LIBVLC_ARCHIVE "${LIBVLC_BINARIES}/vlc-${LIBVLC_VERSION}.7z")
set(LIBVLCPP_ARCHIVE "${LIBVLC_BINARIES}/libvlcpp.zip")

set(LIBVLC_PACKAGE_DIR "${CMAKE_BINARY_DIR}/${LIBRARY_NAME}_packages")
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

add_library(${LIBRARY_NAME} STATIC
  main.cc
  api/api.cc
)
add_dependencies(${LIBRARY_NAME} LIBVLC_EXTRACT)

set_target_properties(${LIBRARY_NAME} PROPERTIES LINKER_LANGUAGE CXX)

get_directory_property(hasParent PARENT_DIRECTORY)
if (hasParent)
  set(DARTVLC_CORE_PATH ${CMAKE_CURRENT_SOURCE_DIR} PARENT_SCOPE)
endif()

target_include_directories(${LIBRARY_NAME} PRIVATE
  # dart_vlc wrapper headers.
  "${CMAKE_CURRENT_SOURCE_DIR}"
  # libVLC++ headers.
  "${LIBVLCPP_SOURCE}"
  # libVLC headers.
  "${LIBVLC_SOURCE}/sdk/include"
)

target_include_directories(${LIBRARY_NAME} INTERFACE
  # libVLC++ headers.
  "${LIBVLCPP_SOURCE}"
  # libVLC headers.
  "${LIBVLC_SOURCE}/sdk/include"
)

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

elseif(LINUX)
  set_target_properties(${LIBRARY_NAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)
  target_link_libraries(${LIBRARY_NAME} PRIVATE
    "vlc"
  )
endif()

# only required by our podspec script phase
install(TARGETS ${LIBRARY_NAME})

