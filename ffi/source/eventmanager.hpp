/*
 * dart_vlc: A media playback library for Dart & Flutter. Based on libVLC & libVLC++.
 * 
 * Hitesh Kumar Saini, Domingo Montesdeoca Gonzalez & contributors.
 * https://github.com/alexmercerind
 * alexmercerind@gmail.com
 * 
 * GNU Lesser General Public License v2.1
 */

#include "../dartvlc/main.cpp"
#include "callbackmanager.hpp"

#ifdef __cplusplus
extern "C" {
#endif

void Player_onPlayPauseStop(PlayerState* state) {
    std::vector<std::string> event;
    event.push_back(std::to_string(state->id));
    event.push_back("playbackEvent");
    event.push_back(std::to_string(state->isPlaying));
    event.push_back(std::to_string(state->isSeekable));
    int _size = event.size();
    char** _event = new char*[_size];
    for (int index = 0; index < _size; index++)
        _event[index] = const_cast<char*>(event[index].c_str());
    callbackStringArray(
        _size,
        _event
    );
    delete[] _event;
}

void Player_onPosition(PlayerState* state) {
    std::vector<std::string> event;
    event.push_back(std::to_string(state->id));
    event.push_back("positionEvent");
    event.push_back(std::to_string(state->index));
    event.push_back(std::to_string(state->position));
    event.push_back(std::to_string(state->duration));
    int _size = event.size();
    char** _event = new char*[_size];
    for (int index = 0; index < _size; index++)
        _event[index] = const_cast<char*>(event[index].c_str());
    callbackStringArray(
        _size,
        _event
    );
    delete[] _event;
}

void Player_onComplete(PlayerState* state) {
    std::vector<std::string> event;
    event.push_back(std::to_string(state->id));
    event.push_back("completeEvent");
    event.push_back(std::to_string(state->isCompleted));
    int _size = event.size();
    char** _event = new char*[_size];
    for (int index = 0; index < _size; index++)
        _event[index] = const_cast<char*>(event[index].c_str());
    callbackStringArray(
        _size,
        _event
    );
    delete[] _event;
}

void Player_onVolume(PlayerState* state) {
    std::vector<std::string> event;
    event.push_back(std::to_string(state->id));
    event.push_back("volumeEvent");
    event.push_back(std::to_string(state->volume));
    int _size = event.size();
    char** _event = new char*[_size];
    for (int index = 0; index < _size; index++)
        _event[index] = const_cast<char*>(event[index].c_str());
    callbackStringArray(
        _size,
        _event
    );
    delete[] _event;
}

void Player_onRate(PlayerState* state) {
    std::vector<std::string> event;
    event.push_back(std::to_string(state->id));
    event.push_back("rateEvent");
    event.push_back(std::to_string(state->rate));
    int _size = event.size();
    char** _event = new char*[_size];
    for (int index = 0; index < _size; index++)
        _event[index] = const_cast<char*>(event[index].c_str());
    callbackStringArray(
        _size,
        _event
    );
    delete[] _event;
}

void Player_onOpen(PlayerState* state) {
    std::vector<std::string> event;
    event.push_back(std::to_string(state->id));
    event.push_back("openEvent");
    event.push_back(std::to_string(state->index));
    event.push_back(std::to_string(state->isPlaylist));
    for (Media* media: state->medias->medias) {
        event.push_back(std::to_string(media->id));
        event.push_back(media->mediaType);
        event.push_back(media->resource);
    }
    int _size = event.size();
    char** _event = new char*[_size];
    for (int index = 0; index < _size; index++)
        _event[index] = const_cast<char*>(event[index].c_str());
    callbackStringArray(
        _size,
        _event
    );
    delete[] _event;
}

#ifdef __cplusplus
}
#endif