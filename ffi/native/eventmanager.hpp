/*
 * dart_vlc: A media playback library for Dart & Flutter. Based on libVLC & libVLC++.
 * 
 * Hitesh Kumar Saini & contributors.
 * https://github.com/alexmercerind
 * alexmercerind@gmail.com
 * 
 * GNU Lesser General Public License v2.1
 */

#include "../../dartvlc/main.hpp"
#include "callbackmanager.hpp"

#ifdef __cplusplus
extern "C" {
#endif

void Player_onPlayPauseStop(PlayerState* state) {
    std::vector<std::string> event;
    event.emplace_back(std::to_string(state->id));
    event.emplace_back("playbackEvent");
    event.emplace_back(std::to_string(state->isPlaying));
    event.emplace_back(std::to_string(state->isSeekable));
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
    event.emplace_back(std::to_string(state->id));
    event.emplace_back("positionEvent");
    event.emplace_back(std::to_string(state->index));
    event.emplace_back(std::to_string(state->position));
    event.emplace_back(std::to_string(state->duration));
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
    event.emplace_back(std::to_string(state->id));
    event.emplace_back("completeEvent");
    event.emplace_back(std::to_string(state->isCompleted));
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
    event.emplace_back(std::to_string(state->id));
    event.emplace_back("volumeEvent");
    event.emplace_back(std::to_string(state->volume));
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
    event.emplace_back(std::to_string(state->id));
    event.emplace_back("rateEvent");
    event.emplace_back(std::to_string(state->rate));
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
    event.emplace_back(std::to_string(state->id));
    event.emplace_back("openEvent");
    event.emplace_back(std::to_string(state->index));
    event.emplace_back(std::to_string(state->isPlaylist));
    for (Media* media: state->medias->medias) {
        event.emplace_back(media->mediaType);
        event.emplace_back(media->resource);
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

void Player_onVideo(int size, PlayerState* state, uint8_t* frame) {
    callbackFrame(
        size,
        state->id,
        frame
    );
}

#ifdef __cplusplus
}
#endif
