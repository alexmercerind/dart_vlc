# dart_vlc: A media playback library for Dart & Flutter. Based on libVLC & libVLC++.
#
# Hitesh Kumar Saini, Domingo Montesdeoca Gonzalez & contributors.
# https://github.com/alexmercerind
# alexmercerind@gmail.com
# GNU Lesser General Public License v2.1
#

# Minimal alternative to CMake build script.
#

# Generate object code using the source/main.cpp.
# Assuming include directory contains libVLC++ headers.
# Setting C++ standard to 17 for std::filesystem.
#
g++ -c source/main.cpp -o main.o -fPIC -I./ -I./../linux/include -std=c++17

# Create shared library from the generated object code.
# Linking libVLC shared library.
#
g++ -shared -o dart_vlc.so main.o -lvlc
