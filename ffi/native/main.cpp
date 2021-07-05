/*
 * dart_vlc: A media playback library for Dart & Flutter. Based on libVLC & libVLC++.
 * 
 * Hitesh Kumar Saini
 * https://github.com/alexmercerind
 * alexmercerind@gmail.com
 * 
 * GNU Lesser General Public License v2.1
 */

#ifndef EXPORT
#ifdef _WIN32
#define EXPORT __declspec(dllexport)
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


EXPORT void Player_create(int id, int videoWidth, int videoHeight, int commandLineArgumentsCount, const char** commandLineArguments) {
    std::vector<std::string> args;
    for (int index = 0; index < commandLineArgumentsCount; index++) args.emplace_back(commandLineArguments[index]);
    Player* player = players->get(id, args);
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
    player->onVideo([=](uint8_t* frame) -> void {
        Player_onVideo(player->videoHeight * player->videoWidth * 4, frame);
    });
}

EXPORT void Player_open(int id, bool autoStart, const char** source, int sourceSize) {
    std::vector<Media*> medias;
    Player* player = players->get(id);
    for (int index = 0; index < 2 * sourceSize; index += 2) {
        Media* media;
        const char* type = source[index];
        const char* resource = source[index + 1];
        if (strcmp(type, "MediaType.file") == 0)
            media = Media::file(0, resource, false);
        else if (strcmp(type, "MediaType.network") == 0)
            media = Media::network(0, resource, false);
        else
            media = Media::directShow(0, resource);
        medias.emplace_back(media);
    }
    player->open(
        new Playlist(medias),
        autoStart
    );
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

EXPORT void Player_setUserAgent(int id, const char* userAgent) {
    Player* player = players->get(id);
    player->setUserAgent(userAgent);
}

EXPORT void Player_setDevice(int id, const char* deviceId, const char* deviceName) {
    Player* player = players->get(id);
    Device device(deviceId, deviceName);
    player->setDevice(&device);
}

EXPORT void Player_setEqualizer(int id, int equalizerId) {
    Player* player = players->get(id);
    Equalizer* equalizer = equalizers->get(equalizerId);
    player->setEqualizer(equalizer);
}

EXPORT void Player_setPlaylistMode(int id, const char* mode) {
    Player* player = players->get(id);
    PlaylistMode playlistMode;
    if (strcmp(mode, "playlistMode.repeat") == 0)
        playlistMode = PlaylistMode::repeat;
    else if (strcmp(mode, "playlistMode.loop") == 0)
        playlistMode = PlaylistMode::loop;
    else if (strcmp(mode, "playlistMode.single") == 0)
        playlistMode = PlaylistMode::single;
    player->setPlaylistMode(playlistMode);
}

EXPORT void Player_add(int id, const char* type, const char* resource) {
    Player* player = players->get(id);
    Media* media;
    if (strcmp(type, "MediaType.file") == 0)
        media = Media::file(0, resource, false);
    else if (strcmp(type, "MediaType.network") == 0)
        media = Media::network(0, resource, false);
    else if (strcmp(type, "MediaType.directShow") == 0)
        media = Media::directShow(0, resource);
    player->add(media);
}

EXPORT void Player_remove(int id, int index) {
    Player* player = players->get(id);
    player->remove(index);
}

EXPORT void Player_insert(int id, int index, const char* type, const char* resource) {
    Player* player = players->get(id);
    Media* media;
    if (strcmp(type, "MediaType.file") == 0)
        media = Media::file(0, resource, false);
    else if (strcmp(type, "MediaType.network") == 0)
        media = Media::network(0, resource, false);
    else if (strcmp(type, "MediaType.directShow") == 0)
        media = Media::directShow(0, resource);
    player->insert(index, media);
}

EXPORT void Player_move(int id, int initialIndex, int finalIndex) {
    Player* player = players->get(id);
    player->move(initialIndex, finalIndex);
}

EXPORT char** Media_parse(const char* type, const char* resource, int timeout) {
    Media* media;
    if (strcmp(type, "MediaType.file") == 0)
        media = Media::file(0, resource, true);
    else if (strcmp(type, "MediaType.network") == 0)
        media = Media::network(0, resource, true);
    else if (strcmp(type, "MediaType.directShow") == 0)
        media = Media::directShow(0, resource);
    char** metas = new char*[media->metas.size()];
    int index = 0;
    for (auto &meta: media->metas) {
        metas[index] = meta.second.data();
        index++;
    }
    return metas;
}

EXPORT void Broadcast_create(int id, const char* type, const char* resource, const char* access, const char* mux, const char* dst, const char* vcodec, int vb, const char* acodec, int ab) {
    Media* media;
    if (strcmp(type, "MediaType.file") == 0)
        media = Media::file(0, resource, false);
    else if (strcmp(type, "MediaType.network") == 0)
        media = Media::network(0, resource, false);
    else if (strcmp(type, "MediaType.directShow") == 0)
        media = Media::directShow(0, resource);
    BroadcastConfiguration configuration(
        access,
        mux,
        dst,
        vcodec,
        vb,
        acodec,
        ab
    );
    Broadcast* broadcast = broadcasts->get(id, media, &configuration);
}

EXPORT void Broadcast_start(int id) {
    Broadcast* broadcast = broadcasts->get(id, nullptr, nullptr);
    broadcast->start();
}

EXPORT void Broadcast_dispose(int id) {
    Broadcast* broadcast = broadcasts->get(id, nullptr, nullptr);
    broadcast->dispose();
}

EXPORT void Chromecast_create(int id, const char* type, const char* resource, const char* ipAddress) {
    Media* media;
    if (strcmp(type, "MediaType.file") == 0)
        media = Media::file(0, resource, false);
    else if (strcmp(type, "MediaType.network") == 0)
        media = Media::network(0, resource, false);
    else if (strcmp(type, "MediaType.directShow") == 0)
        media = Media::directShow(0, resource);
    Chromecast* chromecast = chromecasts->get(id, media, ipAddress);
}

EXPORT void Chromecast_start(int id) {
    Chromecast* chromecast = chromecasts->get(id, nullptr, nullptr);
    chromecast->start();
}

EXPORT void Chromecast_dispose(int id) {
    Chromecast* chromecast = chromecasts->get(id, nullptr, nullptr);
    chromecast->dispose();
}

EXPORT void Record_create(int id, const char* savingFile, const char* type, const char* resource) {
    Media* media;
    if (strcmp(type, "MediaType.file") == 0)
        media = Media::file(0, resource, false);
    else if (strcmp(type, "MediaType.network") == 0)
        media = Media::network(0, resource, false);
    else if (strcmp(type, "MediaType.directShow") == 0)
        media = Media::directShow(0, resource);
    records->get(id, media, savingFile);
}

EXPORT void Record_start(int id) {
    records->get(id, nullptr, "")->start();
}

EXPORT void Record_dispose(int id) {
    records->get(id, nullptr, "")->dispose();
}

EXPORT char** Devices_all() {
    devices->refresh();
    char** _devices = new char*[(devices->all.size() * 2) + 1];
    _devices[0] = std::to_string(devices->all.size()).data();
    for (int i = 1; i < devices->all.size(); i += 2) {
        _devices[i] = devices->all[i]->id.data();
        _devices[i + 1] = devices->all[i + 1]->name.data();
    }
    devices->all.size();
    return _devices;
}

EXPORT char** Equalizer_createEmpty() {
    int id = equalizers->createEmpty();
    Equalizer* equalizer = equalizers->get(id);
    char** _equalizer = new char*[2 * equalizer->bandAmps.size() + 2];
    _equalizer[0] = new char[200];
    strncpy(_equalizer[0], std::to_string(id).data(), 200);
    _equalizer[1] = new char[200];
    strncpy(_equalizer[1], std::to_string(equalizer->preAmp).data(), 200);
    int index = 0;
    for (const auto&[band, amp]: equalizer->bandAmps) {
        _equalizer[index + 2] = new char[200];
        strncpy(_equalizer[index + 2], std::to_string(band).data(), 200);
        _equalizer[index + 3] = new char[200];
        strncpy(_equalizer[index + 3], std::to_string(amp).data(), 200);
        index += 2;
    }
    return _equalizer;
}

EXPORT char** Equalizer_createMode(int mode) {
    int id = equalizers->createMode(
        static_cast<EqualizerMode>(mode)
    );
    Equalizer* equalizer = equalizers->get(id);
    char** _equalizer = new char*[2 * equalizer->bandAmps.size() + 2];
    _equalizer[0] = new char[200];
    strncpy(_equalizer[0], std::to_string(id).data(), 200);
    _equalizer[1] = new char[200];
    strncpy(_equalizer[1], std::to_string(equalizer->preAmp).data(), 200);
    int index = 0;
    for (const auto&[band, amp]: equalizer->bandAmps) {
        _equalizer[index + 2] = new char[200];
        strncpy(_equalizer[index + 2], std::to_string(band).data(), 200);
        _equalizer[index + 3] = new char[200];
        strncpy(_equalizer[index + 3], std::to_string(amp).data(), 200);
        index += 2;
    }
    return _equalizer;
}

EXPORT void Equalizer_setBandAmp(int id, float band, float amp) {
    equalizers->get(id)->setBandAmp(band, amp);
}

EXPORT void Equalizer_setPreAmp(int id, float amp) {
    equalizers->get(id)->setPreAmp(amp);
}

#endif
#ifdef __cplusplus
}
#endif
