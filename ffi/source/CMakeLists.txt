cmake_minimum_required(VERSION 3.15)

project(${PROJECT_NAME} LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(PROJECT_NAME dart_vlc)
set(LIBVLC_VERSION 3.0.9.2)
set(SOURCE main.cpp)

set(LIBVLC_BINARIES ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(LIBVLC_URL http://download.videolan.org/pub/videolan/vlc/${LIBVLC_VERSION}/win64/vlc-${LIBVLC_VERSION}-win64.7z)
set(LIBVLCPP_URL https://github.com/videolan/libvlcpp/archive/master.zip)
set(LIBVLC_ARCHIVE ${LIBVLC_BINARIES}/vlc-${LIBVLC_VERSION}.7z)
set(LIBVLCPP_ARCHIVE ${LIBVLC_BINARIES}/libvlcpp.zip)
set(LIBVLC_SOURCE ${LIBVLC_BINARIES}/vlc-${LIBVLC_VERSION})
set(LIBVLCPP_SOURCE ${LIBVLC_BINARIES}/libvlcpp-master)
if (NOT EXISTS ${LIBVLC_ARCHIVE})
  file(DOWNLOAD ${LIBVLC_URL} ${LIBVLC_ARCHIVE})
endif()
add_custom_target(LIBVLC_EXTRACT ALL)
add_custom_command(
  TARGET LIBVLC_EXTRACT PRE_BUILD
  COMMAND ${CMAKE_COMMAND} -E tar xzf \"${LIBVLC_ARCHIVE}\"
  COMMAND ${CMAKE_COMMAND} -E tar xzf \"${LIBVLCPP_ARCHIVE}\"
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${LIBVLC_SOURCE}/sdk/include/vlc ${CMAKE_CURRENT_SOURCE_DIR}/include/vlc
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${LIBVLCPP_SOURCE}/vlcpp ${CMAKE_CURRENT_SOURCE_DIR}/include/vlcpp
  COMMAND ${CMAKE_COMMAND} -E copy ${LIBVLC_SOURCE}/libvlc.dll ${CMAKE_CURRENT_SOURCE_DIR}/dll/libvlc.dll
  COMMAND ${CMAKE_COMMAND} -E copy ${LIBVLC_SOURCE}/libvlccore.dll ${CMAKE_CURRENT_SOURCE_DIR}/dll/libvlccore.dll
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${LIBVLC_SOURCE}/plugins ${CMAKE_CURRENT_SOURCE_DIR}/dll/plugins
  WORKING_DIRECTORY ${LIBVLC_BINARIES}
  DEPENDS ${LIBVLC_ARCHIVE}
)

add_library(${PROJECT_NAME} SHARED ${SOURCE})
add_executable(DART_VLC ${SOURCE})

include_directories(${PROJECT_NAME} INTERFACE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

link_libraries(${PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/lib/libvlc.lib
  ${CMAKE_CURRENT_SOURCE_DIR}/lib/libvlccore.lib
)

add_custom_command(
  TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/Debug/dart_vlc.dll ${CMAKE_CURRENT_SOURCE_DIR}/dll/dart_vlc.dll
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  DEPENDS ${LIBVLC_ARCHIVE}
)
