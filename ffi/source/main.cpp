/*
 * dart_vlc: A media playback library for Dart & Flutter. Based on libVLC & libVLC++.
 * 
 * Hitesh Kumar Saini, Domingo Montesdeoca Gonzalez & contributors.
 * https://github.com/alexmercerind
 * alexmercerind@gmail.com
 * 
 * GNU Lesser General Public License v2.1
 */

#ifndef EXPORT
#ifdef __WIN32
#define EXPORT declspec(__dllexport)
#else
#define EXPORT
#endif
#endif

#include "eventmanager.hpp"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef DART_VLC_FFI
#define DART_VLC_FFI


EXPORT void Player_create(int id, int videoWidth, int videoHeight) {
    Player* player = players->get(id);
    player->videoWidth = videoWidth;
    player->videoHeight = videoHeight;
    player->onPlay([=]() -> void {
        Player_onPlayPauseStop(player->state);
    });
    player->onPause([=]() -> void {
        Player_onPlayPauseStop(player->state);
    });
    player->onStop([=]() -> void {
        Player_onPlayPauseStop(player->state);
        Player_onPosition(player->state);
    });
    player->onComplete([=]() -> void {
        Player_onComplete(player->state);
    });
    player->onVolume([=](float _) -> void {
        Player_onVolume(player->state);
    });
    player->onRate([=](float _) -> void {
        Player_onRate(player->state);
    });
    player->onPosition([=](int _) -> void {
        Player_onPosition(player->state);
    });
    player->onOpen([=](VLC::Media _) -> void {
        Player_onOpen(player->state);
    });
    player->onPlaylist([=]() -> void {
        Player_onOpen(player->state);
    });
    /// TODO: Handle Player::onException.
}

EXPORT void Player_open(int id, bool autoStart, const char** source, int sourceSize) {
    std::vector<Media*> medias;
    Player* player = players->get(id);
    for (int index = 0; index < 3 * sourceSize; index += 3) {
        Media* media;
        int id = atoi(source[index]);
        const char* type = source[index + 1];
        const char* resource = source[index + 2];
        if (strcmp(type, "MediaType.file") == 0)
            media = Media::file(id, resource, false);
        else if (strcmp(type, "MediaType.network") == 0)
            media = Media::network(id, resource, false);
        else
            media = Media::asset(id, resource, false);
        medias.emplace_back(media);
    }
    player->open(
        new Playlist(medias),
        autoStart
    );
    /// TODO: Improve safety.
}

EXPORT void Player_play(int id) {
    Player* player = players->get(id);
    player->play();
}

EXPORT void Player_pause(int id) {
    Player* player = players->get(id);
    player->pause();
}

EXPORT void Player_playOrPause(int id) {
    Player* player = players->get(id);
    player->playOrPause();
}

EXPORT void Player_stop(int id) {
    Player* player = players->get(id);
    player->stop();
}

EXPORT void Player_next(int id) {
    Player* player = players->get(id);
    player->next();
}

EXPORT void Player_back(int id) {
    Player* player = players->get(id);
    player->back();
}

EXPORT void Player_jump(int id, int index) {
    Player* player = players->get(id);
    player->jump(index);
}

EXPORT void Player_seek(int id, int position) {
    Player* player = players->get(id);
    player->seek(position);
}

EXPORT void Player_setVolume(int id, float volume) {
    Player* player = players->get(id);
    player->setVolume(volume);
}

EXPORT void Player_setRate(int id, float rate) {
    Player* player = players->get(id);
    player->setRate(rate);
}

EXPORT void Player_setPlaylistMode(int id, const char* mode) {
    Player* player = players->get(id);
    PlaylistMode playlistMode;
    if (strcmp(mode, "playlistMode.repeat") == 0)
        playlistMode = PlaylistMode::repeat;
    else if (strcmp(mode, "playlistMode.loop") == 0)
        playlistMode = PlaylistMode::loop;
    else
        playlistMode = PlaylistMode::single;
    player->setPlaylistMode(playlistMode);
}

EXPORT void Player_add(int id, int mediaId, const char* type, const char* resource) {
    Player* player = players->get(id);
    Media* media;
    if (strcmp(type, "MediaType.file") == 0)
        media = Media::file(mediaId, resource, false);
    else if (strcmp(type, "MediaType.network") == 0)
        media = Media::network(mediaId, resource, false);
    else
        media = Media::asset(mediaId, resource, false);
    player->add(media);
}

EXPORT void Player_remove(int id, int index) {
    Player* player = players->get(id);
    player->remove(index);
}

EXPORT void Player_insert(int id, int index, int mediaId, const char* type, const char* resource) {
    Player* player = players->get(id);
    Media* media;
    if (strcmp(type, "MediaType.file") == 0)
        media = Media::file(mediaId, resource, false);
    else if (strcmp(type, "MediaType.network") == 0)
        media = Media::network(mediaId, resource, false);
    else
        media = Media::asset(mediaId, resource, false);
    player->insert(index, media);
}

EXPORT void Player_move(int id, int initialIndex, int finalIndex) {
    Player* player = players->get(id);
    player->move(initialIndex, finalIndex);
}

#endif

#ifdef __cplusplus
}
#endif
