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

#ifdef __WIN32
#define EXPORT declspec(__dllexport)
#else
#define EXPORT
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifndef DART_VLC_FFI
#define DART_VLC_FFI


EXPORT void Player_create(int id, int videoWidth, int videoHeight) {
    Player* player = players->get(id);
    player->videoWidth = videoWidth;
    player->videoHeight = videoHeight;
    /// TODO: Setup event callbacks.
}

EXPORT void Player_open(int id, bool autoStart, const char** source, int sourceSize) {
    /// [source] is array of [Media] in the following form.
    /// "0", "MediaType.file", "/home/alexmercerind/music.mp3", "1", "MediaType.file", "/home/alexmercerind/audio.mp3", "2", "MediaType.network", "https://example.com/music.mp3".
    /// [sourceSize] describes the number of [Media] passed i.e. 3 in above case.
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


#endif

#ifdef __cplusplus
}
#endif
