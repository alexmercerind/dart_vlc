#include "include/dart_vlc/dart_vlc_plugin.h"
#include <flutter_linux/flutter_linux.h>
#include <gtk/gtk.h>
#include <cstring>
#define DART_VLC_PLUGIN(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), dart_vlc_plugin_get_type(), DartVlcPlugin))

#include "main.cpp"


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
}

void playback(PlayerState* &state) {
    auto event = fl_value_new_map();
    fl_value_set_string_take(event, "type", fl_value_new_string("playbackEvent"));
    fl_value_set_string_take(event, "id", fl_value_new_int(state->id));
    fl_value_set_string_take(event, "isPlaying", fl_value_new_bool(state->isPlaying));
    fl_value_set_string_take(event, "isSeekable", fl_value_new_bool(state->isSeekable));
    fl_method_channel_invoke_method(channel, "playerState", event, NULL, NULL, NULL);
}

void complete(PlayerState* &state) {
    auto event = fl_value_new_map();
    fl_value_set_string_take(event, "type", fl_value_new_string("completeEvent"));
    fl_value_set_string_take(event, "id", fl_value_new_int(state->id));
    fl_value_set_string_take(event, "isCompleted", fl_value_new_bool(state->isCompleted));
    fl_method_channel_invoke_method(channel, "playerState", event, NULL, NULL, NULL);
}

void open(PlayerState* &state) {
    auto event = fl_value_new_map();
    fl_value_set_string_take(event, "type", fl_value_new_string("openEvent"));
    fl_value_set_string_take(event, "id", fl_value_new_int(state->id));
    fl_value_set_string_take(event, "index", fl_value_new_int(state->index));
    auto medias = fl_value_new_list();
    for (Media* media: state->medias->medias) {
        auto mediaMap = fl_value_new_map();
        fl_value_set_string_take(mediaMap, "mediaSourceType", fl_value_new_string(media->mediaSourceType().c_str()));
        fl_value_set_string_take(mediaMap, "mediaType", fl_value_new_string(media->mediaType.c_str()));
        fl_value_set_string_take(mediaMap, "resource", fl_value_new_string(media->resource.c_str()));
        fl_value_append_take(medias, mediaMap);
    }
    fl_value_set_string_take(event, "medias", medias);
    fl_value_set_string_take(event, "isPlaylist", fl_value_new_bool(state->isPlaylist));
    fl_method_channel_invoke_method(channel, "playerState", event, NULL, NULL, NULL);
}

void volume(PlayerState* &state) {
    auto event = fl_value_new_map();
    fl_value_set_string_take(event, "type", fl_value_new_string("volumeEvent"));
    fl_value_set_string_take(event, "id", fl_value_new_int(state->id));
    fl_value_set_string_take(event, "volume", fl_value_new_float(state->volume));
    fl_method_channel_invoke_method(channel, "playerState", event, NULL, NULL, NULL);
}

void rate(PlayerState* &state) {
    auto event = fl_value_new_map();
    fl_value_set_string_take(event, "type", fl_value_new_string("rateEvent"));
    fl_value_set_string_take(event, "id", fl_value_new_int(state->id));
    fl_value_set_string_take(event, "rate", fl_value_new_float(state->rate));
    fl_method_channel_invoke_method(channel, "playerState", event, NULL, NULL, NULL);
}

void exception(PlayerState* &state) {
    auto event = fl_value_new_map();
    fl_value_set_string_take(event, "type", fl_value_new_string("exceptionEvent"));
    fl_value_set_string_take(event, "id", fl_value_new_int(state->id));
    fl_value_set_string_take(event, "index", fl_value_new_int(state->index));
    fl_method_channel_invoke_method(channel, "playerState", event, NULL, NULL, NULL);
}

static void dart_vlc_plugin_handle_method_call(DartVlcPlugin* self, FlMethodCall* method_call) {
    g_autoptr(FlMethodResponse) response = nullptr;
    const gchar* method = fl_method_call_get_name(method_call);
    if (strcmp(method, "create") == 0) {
        int id = fl_value_get_int(fl_value_lookup_string(fl_method_call_get_args(method_call), "id"));
        Player* player = players->get(id);
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
        player->onException(
            [player] () -> void {
                exception(player->state);
            }
        );
        response = FL_METHOD_RESPONSE(fl_method_success_response_new(fl_value_new_null()));
    }
    else if (strcmp(method, "open") == 0) {
        int id = fl_value_get_int(fl_value_lookup_string(fl_method_call_get_args(method_call), "id"));
        bool autoStart = fl_value_get_bool(fl_value_lookup_string(fl_method_call_get_args(method_call), "autoStart"));
        auto source = fl_value_lookup_string(fl_method_call_get_args(method_call), "source");
        const char* mediaSourceType = fl_value_get_string(fl_value_lookup_string(source, "mediaSourceType"));
        if (strcmp(mediaSourceType, "MediaSourceType.media") == 0) {
            const char* mediaType = fl_value_get_string(fl_value_lookup_string(source, "mediaType"));
            const char* resource = fl_value_get_string(fl_value_lookup_string(source, "resource"));
            Media* media = nullptr;
            if (strcmp(mediaType, "MediaType.file") == 0)
                media = Media::file(resource);
            else if (strcmp(mediaType, "MediaType.network") == 0)
                media = Media::network(resource);
            else
                media = Media::asset(resource);
            Player* player = players->get(id);
            player->open(media, autoStart);
        }
        if (strcmp(mediaSourceType, "MediaSourceType.playlist") == 0) {
            std::vector<Media*> medias;
            auto mediasList = fl_value_lookup_string(source, "medias");
            for (int index = 0; index < fl_value_get_length(mediasList); index++) {
                const char* mediaType = fl_value_get_string(fl_value_lookup_string(fl_value_get_list_value(mediasList, index), "mediaType"));
                const char* resource = fl_value_get_string(fl_value_lookup_string(fl_value_get_list_value(mediasList, index), "resource"));
                Media* media = nullptr;
                if (strcmp(mediaType, "MediaType.file") == 0)
                    media = Media::file(resource);
                else if (strcmp(mediaType, "MediaType.network") == 0)
                    media = Media::network(resource);
                else
                    media = Media::asset(resource);
                medias.emplace_back(media);
            }
            Player* player = players->get(id);
            player->open(
                new Playlist(medias),
                autoStart
            );
        }
        response = FL_METHOD_RESPONSE(fl_method_success_response_new(fl_value_new_null()));
    }
    else if (strcmp(method, "play") == 0) {
        int id = fl_value_get_int(fl_value_lookup_string(fl_method_call_get_args(method_call), "id"));
        Player* player = players->get(id);
        player->play();
        response = FL_METHOD_RESPONSE(fl_method_success_response_new(fl_value_new_null()));
    }
    else if (strcmp(method, "pause") == 0) {
        int id = fl_value_get_int(fl_value_lookup_string(fl_method_call_get_args(method_call), "id"));
        Player* player = players->get(id);
        player->pause();
        response = FL_METHOD_RESPONSE(fl_method_success_response_new(fl_value_new_null()));
    }
    else if (strcmp(method, "playOrPause") == 0) {
        int id = fl_value_get_int(fl_value_lookup_string(fl_method_call_get_args(method_call), "id"));
        Player* player = players->get(id);
        player->playOrPause();
        response = FL_METHOD_RESPONSE(fl_method_success_response_new(fl_value_new_null()));
    }
    else if (strcmp(method, "stop") == 0) {
        int id = fl_value_get_int(fl_value_lookup_string(fl_method_call_get_args(method_call), "id"));
        Player* player = players->get(id);
        player->stop();
        response = FL_METHOD_RESPONSE(fl_method_success_response_new(fl_value_new_null()));
    }
    else if (strcmp(method, "next") == 0) {
        int id = fl_value_get_int(fl_value_lookup_string(fl_method_call_get_args(method_call), "id"));
        Player* player = players->get(id);
        player->next();
        response = FL_METHOD_RESPONSE(fl_method_success_response_new(fl_value_new_null()));
    }
    else if (strcmp(method, "back") == 0) {
        int id = fl_value_get_int(fl_value_lookup_string(fl_method_call_get_args(method_call), "id"));
        Player* player = players->get(id);
        player->back();
        response = FL_METHOD_RESPONSE(fl_method_success_response_new(fl_value_new_null()));
    }
    else if (strcmp(method, "jump") == 0) {
        int id = fl_value_get_int(fl_value_lookup_string(fl_method_call_get_args(method_call), "id"));
        int index = fl_value_get_int(fl_value_lookup_string(fl_method_call_get_args(method_call), "index"));
        Player* player = players->get(id);
        player->jump(index);
        response = FL_METHOD_RESPONSE(fl_method_success_response_new(fl_value_new_null()));
    }
    else if (strcmp(method, "seek") == 0) {
        int id = fl_value_get_int(fl_value_lookup_string(fl_method_call_get_args(method_call), "id"));
        int duration = fl_value_get_int(fl_value_lookup_string(fl_method_call_get_args(method_call), "duration"));
        Player* player = players->get(id);
        player->seek(duration);
        response = FL_METHOD_RESPONSE(fl_method_success_response_new(fl_value_new_null()));
    }
    else if (strcmp(method, "setVolume") == 0) {
        int id = fl_value_get_int(fl_value_lookup_string(fl_method_call_get_args(method_call), "id"));
        float volume = fl_value_get_float(fl_value_lookup_string(fl_method_call_get_args(method_call), "volume"));
        Player* player = players->get(id);
        player->setVolume(volume);
        response = FL_METHOD_RESPONSE(fl_method_success_response_new(fl_value_new_null()));
    }
    else if (strcmp(method, "setRate") == 0) {
        int id = fl_value_get_int(fl_value_lookup_string(fl_method_call_get_args(method_call), "id"));
        float rate = fl_value_get_float(fl_value_lookup_string(fl_method_call_get_args(method_call), "rate"));
        Player* player = players->get(id);
        player->setRate(rate);
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
