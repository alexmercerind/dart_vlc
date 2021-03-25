/*
 * dart_vlc: A media playback library for Dart & Flutter. Based on libVLC & libVLC++.
 * 
 * Hitesh Kumar Saini
 * https://github.com/alexmercerind
 * alexmercerind@gmail.com
 * 
 * GNU Lesser General Public License v2.1
 */

#include <iostream>
#include <fstream>
#include <ios>

#include "player.hpp"
#include "device.hpp"


int main() {
    int index = 0;
    int* indexPointer = &index;
    Player* player = new Player(0);
    player->onEvent([]() -> void {});
    player->onVideo(
        480,
        320,
        [=](uint8_t* frame) -> void {
            (*indexPointer)++;
            std::fstream file(
                "/home/alexmercerind/frames/frame" + std::to_string((*indexPointer)) + ".BMP",
                std::ios::out | std::ios::binary
            );
            file.write((char*)frame, sizeof(frame));
            file.flush();
            file.close();
        }
    );
    player->open(
        Media::file(0, "/home/alexmercerind/video.mp4")
    );
    std::cin.get();
    return 0;
}
