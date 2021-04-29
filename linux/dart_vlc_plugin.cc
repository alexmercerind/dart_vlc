#include "include/dart_vlc/dart_vlc_plugin.h"
#include <flutter_linux/flutter_linux.h>
#include <gtk/gtk.h>
#include <cstring>
#define DART_VLC_PLUGIN(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), dart_vlc_plugin_get_type(), DartVlcPlugin))

#include "../dartvlc/main.cpp"


struct _DartVlcPlugin {
    GObject parent_instance;
};

G_DEFINE_TYPE(DartVlcPlugin, dart_vlc_plugin, g_object_get_type())

FlMethodChannel* channel;

void position(PlayerState* &state) {
    auto event = fl_value_new_map();
    fl_value_set_string_take(event, "type", fl_value_new_string("positionEvent"));
    fl_value_set_string_take(event, "id", fl_value_new_int(state->id));
    fl_value_set_string_take(event, "index", fl_value_new_int(state->index));
    fl_value_set_string_take(event, "position", fl_value_new_int(state->position));
    fl_value_set_string_take(event, "duration", fl_value_new_int(state->duration));
    fl_method_channel_invoke_method(channel, "playerState", event, NULL, NULL, NULL);
    fl_value_unref(event);
}

void playback(PlayerState* &state) {
    auto event = fl_value_new_map();
    fl_value_set_string_take(event, "type", fl_value_new_string("playbackEvent"));
    fl_value_set_string_take(event, "id", fl_value_new_int(state->id));
    fl_value_set_string_take(event, "isPlaying", fl_value_new_bool(state->isPlaying));
    fl_value_set_string_take(event, "isSeekable", fl_value_new_bool(state->isSeekable));
    fl_method_channel_invoke_method(channel, "playerState", event, NULL, NULL, NULL);
    fl_value_unref(event);
}

void complete(PlayerState* &state) {
    auto event = fl_value_new_map();
    fl_value_set_string_take(event, "type", fl_value_new_string("completeEvent"));
    fl_value_set_string_take(event, "id", fl_value_new_int(state->id));
    fl_value_set_string_take(event, "isCompleted", fl_value_new_bool(state->isCompleted));
    fl_method_channel_invoke_method(channel, "playerState", event, NULL, NULL, NULL);
    fl_value_unref(event);
}

void open(PlayerState* &state) {
    auto event = fl_value_new_map();
    fl_value_set_string_take(event, "type", fl_value_new_string("openEvent"));
    fl_value_set_string_take(event, "id", fl_value_new_int(state->id));
    fl_value_set_string_take(event, "index", fl_value_new_int(state->index));
    auto medias = fl_value_new_list();
    for (Media* media: state->medias->medias) {
        auto mediaMap = fl_value_new_map();
        fl_value_set_string_take(mediaMap, "id", fl_value_new_int(media->id));
        fl_value_set_string_take(mediaMap, "mediaSourceType", fl_value_new_string(media->mediaSourceType().c_str()));
        fl_value_set_string_take(mediaMap, "mediaType", fl_value_new_string(media->mediaType.c_str()));
        fl_value_set_string_take(mediaMap, "resource", fl_value_new_string(media->resource.c_str()));
        fl_value_append_take(medias, mediaMap);
    }
    fl_value_set_string_take(event, "medias", medias);
    fl_value_set_string_take(event, "isPlaylist", fl_value_new_bool(state->isPlaylist));
    fl_method_channel_invoke_method(channel, "playerState", event, NULL, NULL, NULL);
    fl_value_unref(event);
}

void volume(PlayerState* &state) {
    auto event = fl_value_new_map();
    fl_value_set_string_take(event, "type", fl_value_new_string("volumeEvent"));
    fl_value_set_string_take(event, "id", fl_value_new_int(state->id));
    fl_value_set_string_take(event, "volume", fl_value_new_float(state->volume));
    fl_method_channel_invoke_method(channel, "playerState", event, NULL, NULL, NULL);
    fl_value_unref(event);
}

void rate(PlayerState* &state) {
    auto event = fl_value_new_map();
    fl_value_set_string_take(event, "type", fl_value_new_string("rateEvent"));
    fl_value_set_string_take(event, "id", fl_value_new_int(state->id));
    fl_value_set_string_take(event, "rate", fl_value_new_float(state->rate));
    fl_method_channel_invoke_method(channel, "playerState", event, NULL, NULL, NULL);
    fl_value_unref(event);
}

void exception(PlayerState* &state) {
    auto event = fl_value_new_map();
    fl_value_set_string_take(event, "type", fl_value_new_string("exceptionEvent"));
    fl_value_set_string_take(event, "id", fl_value_new_int(state->id));
    fl_value_set_string_take(event, "index", fl_value_new_int(state->index));
    fl_method_channel_invoke_method(channel, "playerState", event, NULL, NULL, NULL);
    fl_value_unref(event);
}

static void dart_vlc_plugin_handle_method_call(DartVlcPlugin* self, FlMethodCall* method_call) {
    FlMethodResponse* response = nullptr;
    const char* method = fl_method_call_get_name(method_call);
    if (strcmp(method, "Player.create") == 0) {
        int id = fl_value_get_int(fl_value_lookup_string(fl_method_call_get_args(method_call), "id"));
        int videoWidth = fl_value_get_int(fl_value_lookup_string(fl_method_call_get_args(method_call), "videoWidth"));
        int videoHeight = fl_value_get_int(fl_value_lookup_string(fl_method_call_get_args(method_call), "videoHeight"));
        Player* player = players->get(id);
        player->videoWidth = videoWidth;
        player->videoHeight = videoHeight;
        player->onOpen(
            [player] (VLC::Media _) -> void {
                open(player->state);
            }
        );
        player->onPlay(
            [player] () -> void {
                playback(player->state);
            }
        );
        player->onPause(
            [player] () -> void {
                playback(player->state);
            }
        );
        player->onStop(
            [player] () -> void {
                playback(player->state);
                position(player->state);
            }
        );
        player->onPosition(
            [player] (int _) -> void {
                position(player->state);
            }
        );
        player->onVolume(
            [player] (float _) -> void {
                volume(player->state);
            }
        );
        player->onRate(
            [player] (float _) -> void {
                rate(player->state);
            }
        );
        player->onComplete(
            [player] () -> void {
                complete(player->state);
            }
        );
        player->onPlaylist(
            [player] () -> void {
                open(player->state);
            }
        );
        if (videoWidth > 0 && videoHeight > 0) {
            player->onVideo(
                [=](uint8_t* frame) -> void {
                    auto videoFrame = fl_value_new_map();
                    auto videoFrameByteArray = fl_value_new_uint8_list(frame, player->videoWidth * player->videoHeight * 4);
                    fl_value_set_string_take(videoFrame, "id", fl_value_new_int(player->state->id));
                    fl_value_set_string_take(videoFrame, "videoWidth", fl_value_new_int(player->videoWidth));
                    fl_value_set_string_take(videoFrame, "videoHeight", fl_value_new_int(player->videoHeight));
                    fl_value_set_string_take(videoFrame, "byteArray", videoFrameByteArray);
                    fl_method_channel_invoke_method(channel, "videoFrame", videoFrame, NULL, NULL, NULL);
                    fl_value_unref(videoFrame);
                }
            );
        }
        player->onException(
            [player] () -> void {
                exception(player->state);
            }
        );
        response = FL_METHOD_RESPONSE(fl_method_success_response_new(fl_value_new_null()));
    }
    else if (strcmp(method, "Player.open") == 0) {
        int id = fl_value_get_int(fl_value_lookup_string(fl_method_call_get_args(method_call), "id"));
        bool autoStart = fl_value_get_bool(fl_value_lookup_string(fl_method_call_get_args(method_call), "autoStart"));
        auto source = fl_value_lookup_string(fl_method_call_get_args(method_call), "source");
        const char* mediaSourceType = fl_value_get_string(fl_value_lookup_string(source, "mediaSourceType"));
        if (strcmp(mediaSourceType, "MediaSourceType.media") == 0) {
            int mediaId = fl_value_get_int(fl_value_lookup_string(source, "id"));
            const char* mediaType = fl_value_get_string(fl_value_lookup_string(source, "mediaType"));
            const char* resource = fl_value_get_string(fl_value_lookup_string(source, "resource"));
            Media* media = nullptr;
            if (strcmp(mediaType, "MediaType.file") == 0)
                media = Media::file(mediaId, resource, false);
            else if (strcmp(mediaType, "MediaType.network") == 0)
                media = Media::network(mediaId, resource, false);
            else
                media = Media::asset(mediaId, resource, false);
            Player* player = players->get(id);
            player->open(media, autoStart);
        }
        if (strcmp(mediaSourceType, "MediaSourceType.playlist") == 0) {
            std::vector<Media*> medias;
            auto mediasList = fl_value_lookup_string(source, "medias");
            const char* _playlistMode = fl_value_get_string(fl_value_lookup_string(source, "playlistMode"));

            for (int index = 0; index < fl_value_get_length(mediasList); index++) {
                int mediaId = fl_value_get_int(fl_value_lookup_string(fl_value_get_list_value(mediasList, index), "id"));
                const char* mediaType = fl_value_get_string(fl_value_lookup_string(fl_value_get_list_value(mediasList, index), "mediaType"));
                const char* resource = fl_value_get_string(fl_value_lookup_string(fl_value_get_list_value(mediasList, index), "resource"));
                Media* media = nullptr;
                if (strcmp(mediaType, "MediaType.file") == 0)
                    media = Media::file(mediaId, resource);
                else if (strcmp(mediaType, "MediaType.network") == 0)
                    media = Media::network(mediaId, resource);
                else
                    media = Media::asset(mediaId, resource);
                medias.emplace_back(media);
            }
            Player* player = players->get(id);
            PlaylistMode playlistMode;
            if (strcmp(_playlistMode, "playlistMode.repeat") == 0)
                playlistMode = PlaylistMode::repeat;
            else if (strcmp(_playlistMode, "playlistMode.loop") == 0)
                playlistMode = PlaylistMode::loop;
            else
                playlistMode = PlaylistMode::single;
            player->setPlaylistMode(playlistMode);
            player->open(
                new Playlist(medias, playlistMode),
                autoStart
            );
        }
        response = FL_METHOD_RESPONSE(fl_method_success_response_new(fl_value_new_null()));
    }
    else if (strcmp(method, "Player.play") == 0) {
        int id = fl_value_get_int(fl_value_lookup_string(fl_method_call_get_args(method_call), "id"));
        Player* player = players->get(id);
        player->play();
        response = FL_METHOD_RESPONSE(fl_method_success_response_new(fl_value_new_null()));
    }
    else if (strcmp(method, "Player.pause") == 0) {
        int id = fl_value_get_int(fl_value_lookup_string(fl_method_call_get_args(method_call), "id"));
        Player* player = players->get(id);
        player->pause();
        response = FL_METHOD_RESPONSE(fl_method_success_response_new(fl_value_new_null()));
    }
    else if (strcmp(method, "Player.playOrPause") == 0) {
        int id = fl_value_get_int(fl_value_lookup_string(fl_method_call_get_args(method_call), "id"));
        Player* player = players->get(id);
        player->playOrPause();
        response = FL_METHOD_RESPONSE(fl_method_success_response_new(fl_value_new_null()));
    }
    else if (strcmp(method, "Player.stop") == 0) {
        int id = fl_value_get_int(fl_value_lookup_string(fl_method_call_get_args(method_call), "id"));
        Player* player = players->get(id);
        player->stop();
        response = FL_METHOD_RESPONSE(fl_method_success_response_new(fl_value_new_null()));
    }
    else if (strcmp(method, "Player.next") == 0) {
        int id = fl_value_get_int(fl_value_lookup_string(fl_method_call_get_args(method_call), "id"));
        Player* player = players->get(id);
        player->next();
        response = FL_METHOD_RESPONSE(fl_method_success_response_new(fl_value_new_null()));
    }
    else if (strcmp(method, "Player.back") == 0) {
        int id = fl_value_get_int(fl_value_lookup_string(fl_method_call_get_args(method_call), "id"));
        Player* player = players->get(id);
        player->back();
        response = FL_METHOD_RESPONSE(fl_method_success_response_new(fl_value_new_null()));
    }
    else if (strcmp(method, "Player.jump") == 0) {
        int id = fl_value_get_int(fl_value_lookup_string(fl_method_call_get_args(method_call), "id"));
        int index = fl_value_get_int(fl_value_lookup_string(fl_method_call_get_args(method_call), "index"));
        Player* player = players->get(id);
        player->jump(index);
        response = FL_METHOD_RESPONSE(fl_method_success_response_new(fl_value_new_null()));
    }
    else if (strcmp(method, "Player.seek") == 0) {
        int id = fl_value_get_int(fl_value_lookup_string(fl_method_call_get_args(method_call), "id"));
        int duration = fl_value_get_int(fl_value_lookup_string(fl_method_call_get_args(method_call), "duration"));
        Player* player = players->get(id);
        player->seek(duration);
        response = FL_METHOD_RESPONSE(fl_method_success_response_new(fl_value_new_null()));
    }
    else if (strcmp(method, "Player.setVolume") == 0) {
        int id = fl_value_get_int(fl_value_lookup_string(fl_method_call_get_args(method_call), "id"));
        float volume = fl_value_get_float(fl_value_lookup_string(fl_method_call_get_args(method_call), "volume"));
        Player* player = players->get(id);
        player->setVolume(volume);
        response = FL_METHOD_RESPONSE(fl_method_success_response_new(fl_value_new_null()));
    }
    else if (strcmp(method, "Player.setRate") == 0) {
        int id = fl_value_get_int(fl_value_lookup_string(fl_method_call_get_args(method_call), "id"));
        float rate = fl_value_get_float(fl_value_lookup_string(fl_method_call_get_args(method_call), "rate"));
        Player* player = players->get(id);
        player->setRate(rate);
        response = FL_METHOD_RESPONSE(fl_method_success_response_new(fl_value_new_null()));
    }
    else if(strcmp(method, "Player.setPlaylistMode") == 0){
        int id = fl_value_get_int(fl_value_lookup_string(fl_method_call_get_args(method_call), "id"));
        const char* _playlistMode = fl_value_get_string(fl_value_lookup_string(fl_method_call_get_args(method_call), "playlistMode"));
        Player* player = players->get(id);
        PlaylistMode playlistMode;
        if (strcmp(_playlistMode, "playlistMode.repeat") == 0)
            playlistMode = PlaylistMode::repeat;
        else if (strcmp(_playlistMode, "playlistMode.loop") == 0)
            playlistMode = PlaylistMode::loop;
        else
            playlistMode = PlaylistMode::single;
        player->setPlaylistMode(playlistMode);
        response = FL_METHOD_RESPONSE(fl_method_success_response_new(fl_value_new_null()));
    }
    else if (strcmp(method, "Player.add") == 0) {
        int id = fl_value_get_int(fl_value_lookup_string(fl_method_call_get_args(method_call), "id"));
        auto source = fl_value_lookup_string(fl_method_call_get_args(method_call), "source");
        int mediaId = fl_value_get_int(fl_value_lookup_string(source, "id"));
        const char* mediaType = fl_value_get_string(fl_value_lookup_string(source, "mediaType"));
        const char* resource = fl_value_get_string(fl_value_lookup_string(source, "resource"));
        Media* media = nullptr;
        if (strcmp(mediaType, "MediaType.file") == 0)
            media = Media::file(mediaId, resource);
        else if (strcmp(mediaType, "MediaType.network") == 0)
            media = Media::network(mediaId, resource);
        else
            media = Media::asset(mediaId, resource);
        Player* player = players->get(id);
        player->add(media);
        response = FL_METHOD_RESPONSE(fl_method_success_response_new(fl_value_new_null()));
    }
    else if (strcmp(method, "Player.remove") == 0) {
        int id = fl_value_get_int(fl_value_lookup_string(fl_method_call_get_args(method_call), "id"));
        int index = fl_value_get_int(fl_value_lookup_string(fl_method_call_get_args(method_call), "index"));
        Player* player = players->get(id);
        player->remove(index);
        response = FL_METHOD_RESPONSE(fl_method_success_response_new(fl_value_new_null()));
    }
    else if (strcmp(method, "Player.insert") == 0) {
        int id = fl_value_get_int(fl_value_lookup_string(fl_method_call_get_args(method_call), "id"));
        int index = fl_value_get_int(fl_value_lookup_string(fl_method_call_get_args(method_call), "index"));
        auto source = fl_value_lookup_string(fl_method_call_get_args(method_call), "source");
        int mediaId = fl_value_get_int(fl_value_lookup_string(source, "id"));
        const char* mediaType = fl_value_get_string(fl_value_lookup_string(source, "mediaType"));
        const char* resource = fl_value_get_string(fl_value_lookup_string(source, "resource"));
        Media* media = nullptr;
        if (strcmp(mediaType, "MediaType.file") == 0)
            media = Media::file(mediaId, resource);
        else if (strcmp(mediaType, "MediaType.network") == 0)
            media = Media::network(mediaId, resource);
        else
            media = Media::asset(mediaId, resource);
        Player* player = players->get(id);
        player->insert(index, media);
        response = FL_METHOD_RESPONSE(fl_method_success_response_new(fl_value_new_null()));
    }
    else if (strcmp(method, "Player.move") == 0) {
        int id = fl_value_get_int(fl_value_lookup_string(fl_method_call_get_args(method_call), "id"));
        int initial = fl_value_get_int(fl_value_lookup_string(fl_method_call_get_args(method_call), "initial"));
        int final = fl_value_get_int(fl_value_lookup_string(fl_method_call_get_args(method_call), "final"));
        Player* player = players->get(id);
        player->move(initial, final);
        response = FL_METHOD_RESPONSE(fl_method_success_response_new(fl_value_new_null()));
    }
    else if (strcmp(method, "Player.setDevice") == 0) {
        int id = fl_value_get_int(fl_value_lookup_string(fl_method_call_get_args(method_call), "id"));
        auto deviceMap = fl_value_lookup_string(fl_method_call_get_args(method_call), "device");
        Device* device = new Device(
            fl_value_get_string(fl_value_lookup_string(deviceMap, "id")),
            fl_value_get_string(fl_value_lookup_string(deviceMap, "name"))
        );
        Player* player = players->get(id);
        player->setDevice(device);
        response = FL_METHOD_RESPONSE(fl_method_success_response_new(fl_value_new_null()));
    }
    else if (strcmp(method, "Devices.all") == 0) {
        auto devicesList = fl_value_new_list();
        for (auto deviceMap: devices->get()) {
            auto device = fl_value_new_map();
            fl_value_set_string_take(device, "id", fl_value_new_string(deviceMap["id"].c_str()));
            fl_value_set_string_take(device, "name", fl_value_new_string(deviceMap["name"].c_str()));
            fl_value_append_take(devicesList, device);
        }
        response = FL_METHOD_RESPONSE(fl_method_success_response_new(devicesList));
        fl_value_unref(devicesList);
    }
    else if (strcmp(method, "Media.parse") == 0) {
        int timeout = fl_value_get_int(fl_value_lookup_string(fl_method_call_get_args(method_call), "timeout"));
        auto source = fl_value_lookup_string(fl_method_call_get_args(method_call), "source");
        int mediaId = fl_value_get_int(fl_value_lookup_string(source, "id"));
        const char* mediaType = fl_value_get_string(fl_value_lookup_string(source, "mediaType"));
        const char* resource = fl_value_get_string(fl_value_lookup_string(source, "resource"));
        Media* media = nullptr;
        if (strcmp(mediaType, "MediaType.file") == 0)
            media = Media::file(mediaId, resource, true, timeout);
        else if (strcmp(mediaType, "MediaType.network") == 0)
            media = Media::network(mediaId, resource, true, timeout);
        else
            media = Media::asset(mediaId, resource, true, timeout);
        auto metas = fl_value_new_map();
        for (const auto &pair: media->metas) {
            fl_value_set_string_take(metas, pair.first.c_str(), fl_value_new_string(pair.second.c_str()));
        }
        response = FL_METHOD_RESPONSE(fl_method_success_response_new(metas));
        fl_value_unref(metas);
    }
    else if (strcmp(method, "Broadcast.create") == 0) {
        int id = fl_value_get_int(fl_value_lookup_string(fl_method_call_get_args(method_call), "id"));
        auto _media = fl_value_lookup_string(fl_method_call_get_args(method_call), "media");
        int mediaId = fl_value_get_int(fl_value_lookup_string(_media, "id"));
        const char* mediaType = fl_value_get_string(fl_value_lookup_string(_media, "mediaType"));
        const char* resource = fl_value_get_string(fl_value_lookup_string(_media, "resource"));
        Media* media = nullptr;
        if (strcmp(mediaType, "MediaType.file") == 0)
            media = Media::file(mediaId, resource);
        else if (strcmp(mediaType, "MediaType.network") == 0)
            media = Media::network(mediaId, resource);
        else if (strcmp(mediaType, "MediaType.asset") == 0)
            media = Media::asset(mediaId, resource);
        else 
            media = Media::directShow(mediaId, resource);
        auto _configuration = fl_value_lookup_string(fl_method_call_get_args(method_call), "configuration");
        const char* access = fl_value_get_string(fl_value_lookup_string(_configuration, "access"));
        const char* mux = fl_value_get_string(fl_value_lookup_string(_configuration, "mux"));
        const char* dst = fl_value_get_string(fl_value_lookup_string(_configuration, "dst"));
        const char* vcodec = fl_value_get_string(fl_value_lookup_string(_configuration, "vcodec"));
        int vb = fl_value_get_int(fl_value_lookup_string(_configuration, "vb"));
        const char* acodec = fl_value_get_string(fl_value_lookup_string(_configuration, "acodec"));
        int ab = fl_value_get_int(fl_value_lookup_string(_configuration, "ab"));
        BroadcastConfiguration* configuration = new BroadcastConfiguration(
            access, mux, dst, vcodec, vb, acodec, ab
        );
        broadcasts->get(id, media, configuration);
        response = FL_METHOD_RESPONSE(fl_method_success_response_new(fl_value_new_null()));
    }
    else if (strcmp(method, "Broadcast.start") == 0) {
        int id = fl_value_get_int(fl_value_lookup_string(fl_method_call_get_args(method_call), "id"));
        Broadcast* broadcast = broadcasts->get(id, nullptr, nullptr);
        broadcast->start();
        response = FL_METHOD_RESPONSE(fl_method_success_response_new(fl_value_new_null()));
    }
    else if (strcmp(method, "Broadcast.dispose") == 0) {
        int id = fl_value_get_int(fl_value_lookup_string(fl_method_call_get_args(method_call), "id"));
        Broadcast* broadcast = broadcasts->get(id, nullptr, nullptr);
        broadcast->dispose();
        response = FL_METHOD_RESPONSE(fl_method_success_response_new(fl_value_new_null()));
    }
    else if(strcmp(method, "Record.create") == 0){
        int id = fl_value_get_int(fl_value_lookup_string(fl_method_call_get_args(method_call), "id"));
        const char* savingFile = fl_value_get_string(fl_value_lookup_string(fl_method_call_get_args(method_call), "savingFile"));
        auto _media = fl_value_lookup_string(fl_method_call_get_args(method_call), "media");
        int mediaId = fl_value_get_int(fl_value_lookup_string(_media, "id"));
        const char* mediaType = fl_value_get_string(fl_value_lookup_string(_media, "mediaType"));
        const char* resource = fl_value_get_string(fl_value_lookup_string(_media, "resource"));
        Media* media = nullptr;
        if (strcmp(mediaType, "MediaType.file") == 0)
            media = Media::file(mediaId, resource);
        else if (strcmp(mediaType, "MediaType.network") == 0)
            media = Media::network(mediaId, resource);
        else if (strcmp(mediaType, "MediaType.asset") == 0)
            media = Media::asset(mediaId, resource);
        else 
            media = Media::directShow(mediaId, resource);
        records->get(id, media, savingFile);
        response = FL_METHOD_RESPONSE(fl_method_success_response_new(fl_value_new_null()));
    }
    else if(strcmp(method, "Record.start") == 0){
        int id = fl_value_get_int(fl_value_lookup_string(fl_method_call_get_args(method_call), "id"));
        Record* record = records->get(id, nullptr, "");
        record->start();
        response = FL_METHOD_RESPONSE(fl_method_success_response_new(fl_value_new_null()));
    }
    else if(strcmp(method, "Record.dispose") == 0){
        int id = fl_value_get_int(fl_value_lookup_string(fl_method_call_get_args(method_call), "id"));
        Record* record = records->get(id, nullptr, "");
        record->dispose();
        response = FL_METHOD_RESPONSE(fl_method_success_response_new(fl_value_new_null()));
    }
    else if(strcmp(method, "Chromecast.create") == 0){
        int id = fl_value_get_int(fl_value_lookup_string(fl_method_call_get_args(method_call), "id"));
        const char* ipAddress = fl_value_get_string(fl_value_lookup_string(fl_method_call_get_args(method_call), "ipAddress"));
        auto _media = fl_value_lookup_string(fl_method_call_get_args(method_call), "media");
        int mediaId = fl_value_get_int(fl_value_lookup_string(_media, "id"));
        const char* mediaType = fl_value_get_string(fl_value_lookup_string(_media, "mediaType"));
        const char* resource = fl_value_get_string(fl_value_lookup_string(_media, "resource"));
        Media* media = nullptr;
        if (strcmp(mediaType, "MediaType.file") == 0)
            media = Media::file(mediaId, resource);
        else if (strcmp(mediaType, "MediaType.network") == 0)
            media = Media::network(mediaId, resource);
        else if (strcmp(mediaType, "MediaType.asset") == 0)
            media = Media::asset(mediaId, resource);
        else 
            media = Media::directShow(mediaId, resource);
        chromecasts->get(id, media, ipAddress);
        response = FL_METHOD_RESPONSE(fl_method_success_response_new(fl_value_new_null()));
    }
    else if(strcmp(method, "Chromecast.send") == 0){
        int id = fl_value_get_int(fl_value_lookup_string(fl_method_call_get_args(method_call), "id"));
        Chromecast* chromecast = chromecasts->get(id, nullptr, "");
        chromecast->send();
        response = FL_METHOD_RESPONSE(fl_method_success_response_new(fl_value_new_null()));
    }
    else if(strcmp(method, "Chromecast.dispose") == 0){
        int id = fl_value_get_int(fl_value_lookup_string(fl_method_call_get_args(method_call), "id"));
        Chromecast* chromecast = chromecasts->get(id, nullptr, "");
        chromecast->dispose();
        response = FL_METHOD_RESPONSE(fl_method_success_response_new(fl_value_new_null()));
    }
    else {
        response = FL_METHOD_RESPONSE(fl_method_not_implemented_response_new());
    }
    fl_method_call_respond(method_call, response, nullptr);
}

static void dart_vlc_plugin_dispose(GObject* object) {
    G_OBJECT_CLASS(dart_vlc_plugin_parent_class)->dispose(object);
}

static void dart_vlc_plugin_class_init(DartVlcPluginClass* klass) {
    G_OBJECT_CLASS(klass)->dispose = dart_vlc_plugin_dispose;
}

static void dart_vlc_plugin_init(DartVlcPlugin* self) {}

static void method_call_cb(FlMethodChannel* channel, FlMethodCall* method_call, gpointer user_data) {
    DartVlcPlugin* plugin = DART_VLC_PLUGIN(user_data);
    dart_vlc_plugin_handle_method_call(plugin, method_call);
}

void dart_vlc_plugin_register_with_registrar(FlPluginRegistrar* registrar) {
    DartVlcPlugin* plugin = DART_VLC_PLUGIN(g_object_new(dart_vlc_plugin_get_type(), nullptr));
    g_autoptr(FlStandardMethodCodec) codec = fl_standard_method_codec_new();
    channel = fl_method_channel_new(fl_plugin_registrar_get_messenger(registrar), "dart_vlc", FL_METHOD_CODEC(codec));
    fl_method_channel_set_method_call_handler(channel, method_call_cb, g_object_ref(plugin), g_object_unref);
    g_object_unref(plugin);
}