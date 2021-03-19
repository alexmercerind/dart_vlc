/*
 * dart_vlc: A media playback library for Dart & Flutter. Based on libVLC & libVLC++.
 * 
 * Hitesh Kumar Saini
 * https://github.com/alexmercerind
 * alexmercerind@gmail.com
 * 
 * GNU Lesser General Public License v2.1
 */

#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>
#include <flutter/standard_method_codec.h>
#include "include/dart_vlc/dart_vlc_plugin.h"
#include "include/flutter_types.hpp"

#include "../dartvlc/main.cpp"


std::unique_ptr<flutter::MethodChannel<flutter::EncodableValue>> channel;


void position(PlayerState* &state) {
    /*
     * Method to notify Dart about position event.
     *
     * Argument:
     * 
     * {
     *      'type': 'positionEvent',
     *      'id': 0,
     *      'index': 1,
     *      'position': 56783,
     *      'duration': 370278
     * }
     * 
     */
    channel->InvokeMethod(
        "playerState",
        std::unique_ptr<flutter::EncodableValue>(
            new flutter::EncodableValue(
                flutter::EncodableMap(
                    {
                        {
                            flutter::EncodableValue("type"),
                            flutter::EncodableValue("positionEvent")
                        },
                        {
                            flutter::EncodableValue("id"),
                            flutter::EncodableValue(state->id)
                        },
                        {
                            flutter::EncodableValue("index"),
                            flutter::EncodableValue(state->index)
                        },
                        {
                            flutter::EncodableValue("position"),
                            flutter::EncodableValue(state->position)
                        },
                        {
                            flutter::EncodableValue("duration"),
                            flutter::EncodableValue(state->duration)
                        }
                    }
                )
            )
        )
    );
}

void playback(PlayerState* &state) {
    /*
     * Method to notify Dart about playback events.
     *
     * Argument:
     * 
     * {
     *      'type': 'playbackEvent',
     *      'id': 0,
     *      'isPlaying': true,
     *      'isSeekable': false
     * }
     * 
     */
    channel->InvokeMethod(
        "playerState",
        std::unique_ptr<flutter::EncodableValue>(
            new flutter::EncodableValue(
                flutter::EncodableMap(
                    {
                        {
                            flutter::EncodableValue("type"),
                            flutter::EncodableValue("playbackEvent")
                        },
                        {
                            flutter::EncodableValue("id"),
                            flutter::EncodableValue(state->id)
                        },
                        {
                            flutter::EncodableValue("isPlaying"),
                            flutter::EncodableValue(state->isPlaying)
                        },
                        {
                            flutter::EncodableValue("isSeekable"),
                            flutter::EncodableValue(state->isSeekable)
                        }
                    }
                )
            )
        )
    );
}

void complete(PlayerState* &state) {
    /*
     * Method to notify Dart about playback completion event.
     *
     * Argument:
     * 
     * {
     *      'type': 'completeEvent',
     *      'id': 0,
     *      'isCompleted': false
     * }
     * 
     */
    channel->InvokeMethod(
        "playerState",
        std::unique_ptr<flutter::EncodableValue>(
            new flutter::EncodableValue(
                flutter::EncodableMap(
                    {
                        {
                            flutter::EncodableValue("type"),
                            flutter::EncodableValue("completeEvent")
                        },
                        {
                            flutter::EncodableValue("id"),
                            flutter::EncodableValue(state->id)
                        },
                        {
                            flutter::EncodableValue("isCompleted"),
                            flutter::EncodableValue(state->isCompleted)
                        }
                    }
                )
            )
        )
    );
}

void open(PlayerState* &state) {
    /*
     * Method to notify Dart about open event.
     *
     * Argument:
     * 
     * {
     *      'type': 'openEvent',
     *      'id': 0,
     *      'index': 1,
     *      'medias': [
     *          {
     *              'mediaSourceType': 'MediaSourceType.media',
     *              'mediaType': 'MediaType.network',
     *              'resource': 'https://alexmercerind.com/music.MP3'
     *          },
     *          {
     *              'mediaSourceType': 'MediaSourceType.media',
     *              'mediaType': MediaType.file',
     *              'resource': 'C:/alexmercerind/music.MP3'
     *          }
     *      ],
     *      'isPlaylist': true
     * }
     * 
     */
    channel->InvokeMethod(
        "playerState",
        std::unique_ptr<flutter::EncodableValue>(
            new flutter::EncodableValue(
                flutter::EncodableMap(
                    {
                        {
                            flutter::EncodableValue("type"),
                            flutter::EncodableValue("openEvent")
                        },
                        {
                            flutter::EncodableValue("id"),
                            flutter::EncodableValue(state->id)
                        },
                        {
                            flutter::EncodableValue("index"),
                            flutter::EncodableValue(state->index)
                        },
                        {
                            flutter::EncodableValue("medias"),
                            InvokeMethodHandler::getValue<std::vector<std::map<std::string, std::string>>>(
                                state->medias->get()
                            )
                        },
                        {
                            flutter::EncodableValue("isPlaylist"),
                            flutter::EncodableValue(state->isPlaylist)
                        }
                    }
                )
            )
        )
    );
}

void volume(PlayerState* &state) {
    /*
     * Method to notify Dart about open event.
     *
     * Argument:
     * 
     * {
     *      'type': 'volumeEvent',
     *      'id': 0,
     *      'volume': 0.345
     * }
     * 
     */
    channel->InvokeMethod(
        "playerState",
        std::unique_ptr<flutter::EncodableValue>(
            new flutter::EncodableValue(
                flutter::EncodableMap(
                    {
                        {
                            flutter::EncodableValue("type"),
                            flutter::EncodableValue("volumeEvent")
                        },
                        {
                            flutter::EncodableValue("id"),
                            flutter::EncodableValue(state->id)
                        },
                        {
                            flutter::EncodableValue("volume"),
                            flutter::EncodableValue(state->volume)
                        }
                    }
                )
            )
        )
    );
}

void rate(PlayerState* &state) {
    /*
     * Method to notify Dart about rate event.
     *
     * Argument:
     * 
     * {
     *      'type': 'openEvent',
     *      'id': 0,
     *      'rate': 1.23
     * }
     * 
     */
    channel->InvokeMethod(
        "playerState",
        std::unique_ptr<flutter::EncodableValue>(
            new flutter::EncodableValue(
                flutter::EncodableMap(
                    {
                        {
                            flutter::EncodableValue("type"),
                            flutter::EncodableValue("rateEvent")
                        },
                        {
                            flutter::EncodableValue("id"),
                            flutter::EncodableValue(state->id)
                        },
                        {
                            flutter::EncodableValue("rate"),
                            flutter::EncodableValue(state->rate)
                        }
                    }
                )
            )
        )
    );
}

void exception(PlayerState* &state) {
    /*
     * Method to notify Dart about exception events.
     *
     * Argument:
     * 
     * {
     *      'type': 'exception',
     *      'id': 0,
     *      'index': 1
     * }
     * 
     */
    channel->InvokeMethod(
        "playerState",
        std::unique_ptr<flutter::EncodableValue>(
            new flutter::EncodableValue(
                flutter::EncodableMap(
                    {
                        {
                            flutter::EncodableValue("type"),
                            flutter::EncodableValue("exceptionEvent")
                        },
                        {
                            flutter::EncodableValue("id"),
                            flutter::EncodableValue(state->id)
                        },
                        {
                            flutter::EncodableValue("index"),
                            flutter::EncodableValue(state->index)
                        }
                    }
                )
            )
        )
    );
}


namespace {


    class DartVlcPlugin : public flutter::Plugin {
    public:
        static void RegisterWithRegistrar(flutter::PluginRegistrarWindows *registrar);

        DartVlcPlugin();

        virtual ~DartVlcPlugin();

    private:
        void HandleMethodCall(const flutter::MethodCall<flutter::EncodableValue> &method_call, std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    };


    void DartVlcPlugin::RegisterWithRegistrar(flutter::PluginRegistrarWindows *registrar) {
        channel = std::make_unique<flutter::MethodChannel<flutter::EncodableValue>>(registrar->messenger(), "dart_vlc", &flutter::StandardMethodCodec::GetInstance());
        auto plugin = std::make_unique<DartVlcPlugin>();
        channel->SetMethodCallHandler(
            [plugin_pointer = plugin.get()](const auto &call, auto result) {
                plugin_pointer->HandleMethodCall(call, std::move(result));
            }
        );
        registrar->AddPlugin(std::move(plugin));
    }

    DartVlcPlugin::DartVlcPlugin() {}

    DartVlcPlugin::~DartVlcPlugin() {}

    void DartVlcPlugin::HandleMethodCall(const flutter::MethodCall<flutter::EncodableValue> &methodCall, std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
        MethodCallHandler* method = new MethodCallHandler(&methodCall, std::move(result));
        /*
         * Creates a new [Player] instance & setups event & exception callbacks.
         * 
         * Argument:
         * 
         * {
         *      'id': 0
         * }
         * 
         */
        if (method->name == "create") {
            int id = method->getArgument<int>("id");
            Player* player = players->get(id);
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
                }
            );
            player->onPosition(
                [player] (int _) -> void {
                    position(player->state);
                }
            );
            player->onOpen(
                [player] (VLC::Media _) -> void {
                    open(player->state);
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
            player->onPlaylist(
                [player] () -> void {
                    open(player->state);
                }
            );
            method->returnNull();
        }
        /*
         * Opens an [MediaSource] i.e [Media] or [Playlist] into the [Player].
         * 
         * Argument for loading an [Media]:
         * 
         * {
         *      'id': 0,
         *      'autoStart': true,
         *      'source': {
         *          'mediaSourceType': 'MediaSourceType.media',
         *          'mediaType': 'MediaType.file',
         *          'resource': 'C:/alexmercerind/music.MP3'
         *      }
         * }
         * 
         * Argument for loading a [Playlist]:
         * 
         * {
         *      'id': 1,
         *      'autoStart': true,
         *      'source': {
         *          'mediaSourceType': 'MediaSourceType.playlist',
         *          'medias': [
         *              {
         *                  'mediaSourceType': 'MediaSourceType.media',
         *                  'mediaType': 'MediaType.file',
         *                  'resource': 'C:/alexmercerind/music.MP3'
         *              },
         *              {
         *                  'mediaSourceType': 'MediaSourceType.media',
         *                  'mediaType': 'MediaType.network',
         *                  'resource': 'C:/alexmercerind/music.MP3'
         *              }
         *          ]
         *      }
         * }
         */
        else if (method->name == "open") {
            int id = method->getArgument<int>("id");
            bool autoStart = method->getArgument<bool>("autoStart");
            std::map<flutter::EncodableValue, flutter::EncodableValue> source = std::get<flutter::EncodableMap>(method->arguments[flutter::EncodableValue("source")]);
            std::string mediaSourceType = std::get<std::string>(source[flutter::EncodableValue("mediaSourceType")]);
            if (mediaSourceType == "MediaSourceType.media") {
                std::string mediaType = std::get<std::string>(source[flutter::EncodableValue("mediaType")]);
                std::string resource = std::get<std::string>(source[flutter::EncodableValue("resource")]);
                Media* media = nullptr;
                if (mediaType == "MediaType.file")
                    media = Media::file(resource);
                else if (mediaType == "MediaType.network")
                    media = Media::network(resource);
                else
                    media = Media::asset(resource);
                Player* player = players->get(id);
                player->open(media, autoStart);
            }
            if (mediaSourceType == "MediaSourceType.playlist") {
                std::vector<Media*> medias;
                std::vector<std::map<std::string, std::string>> mediasMap = MethodCallHandler::getValue<std::vector<std::map<std::string, std::string>>>(source[flutter::EncodableValue("medias")]);
                for (std::map<std::string,std::string> mediaMap: mediasMap) {
                    Media* media;
                    if (mediaMap["mediaType"] == "MediaType.file")
                        media = Media::file(mediaMap["resource"]);
                    else if (mediaMap["mediaType"] == "MediaType.network")
                        media = Media::network(mediaMap["resource"]);
                    else
                        media = Media::asset(mediaMap["resource"]);
                    medias.emplace_back(media);
                }
                Player* player = players->get(id);
                player->open(
                    new Playlist(medias),
                    autoStart
                );
            }
            method->returnNull();
        }
        /*
         * Plays the [Player] instance.
         * 
         * Argument:
         * 
         * {
         *      'id': 0
         * }
         * 
         */
        else if (method->name == "play") {
            int id = method->getArgument<int>("id");
            Player* player = players->get(id);
            player->play();
            method->returnNull();
        }
        /*
         * Pauses the [Player] instance.
         * 
         * Argument:
         * 
         * {
         *      'id': 0
         * }
         * 
         */
        else if (method->name == "pause") {
            int id = method->getArgument<int>("id");
            Player* player = players->get(id);
            player->pause();
            method->returnNull();
        }
        /*
         * Switches between play & pause states of the [Player] instance.
         * 
         * Argument:
         * 
         * {
         *      'id': 0
         * }
         * 
         */
        else if (method->name == "playOrPause") {
            int id = method->getArgument<int>("id");
            Player* player = players->get(id);
            player->playOrPause();
            method->returnNull();
        }
        /*
         * Stops the [Player] instance.
         * 
         * Argument:
         * 
         * {
         *      'id': 0
         * }
         * 
         */
        else if (method->name == "stop") {
            int id = method->getArgument<int>("id");
            Player* player = players->get(id);
            player->stop();
            method->returnNull();
        }
        /*
         * Jumps to next media in the [Playlist] loaded in [Player] instance.
         * 
         * Argument:
         * 
         * {
         *      'id': 0
         * }
         * 
         */
        else if (method->name == "next") {
            int id = method->getArgument<int>("id");
            Player* player = players->get(id);
            player->next();
            method->returnNull();
        }
        /*
         * Jumps to previous media in the [Playlist] loaded in [Player] instance.
         * 
         * Argument:
         * 
         * {
         *      'id': 0
         * }
         * 
         */
        else if (method->name == "back") {
            int id = method->getArgument<int>("id");
            Player* player = players->get(id);
            player->back();
            method->returnNull();
        }
        /*
         * Jumps to specific index in the [Playlist] loaded in [Player] instance.
         * 
         * Argument:
         * 
         * {
         *      'id': 0,
         *      'index': 5
         * }
         * 
         */
        else if (method->name == "jump") {
            int id = method->getArgument<int>("id");
            int index = method->getArgument<int>("index");
            Player* player = players->get(id);
            player->jump(index);
            method->returnNull();
        }
        /*
         * Seeks [Player] instance.
         * 
         * Argument:
         * 
         * {
         *      'id': 0,
         *      'duration': 50000
         * }
         * 
         */
        else if (method->name == "seek") {
            int id = method->getArgument<int>("id");
            int duration = method->getArgument<int>("duration");
            Player* player = players->get(id);
            player->seek(duration);
            method->returnNull();
        }
        /*
         * Sets volume of [Player] instance.
         * 
         * Argument:
         * 
         * {
         *      'id': 0,
         *      'volume': 0.5
         * }
         * 
         */
        else if (method->name == "setVolume") {
            int id = method->getArgument<int>("id");
            float volume = method->getArgument<float>("volume");
            Player* player = players->get(id);
            player->setVolume(volume);
            method->returnNull();
        }
        /*
         * Sets playback rate of [Player] instance.
         * 
         * Argument:
         * 
         * {
         *      'id': 0,
         *      'rate': 1.2
         * }
         * 
         */
        else if (method->name == "setRate") {
            int id = method->getArgument<int>("id");
            float rate = method->getArgument<float>("rate");
            Player* player = players->get(id);
            player->setRate(rate);
            method->returnNull();
        }
        /* Adds new `Media` to the end of the `Playlist` of the `Player`.
         *
         * Argument:
         * 
         * {
         *      'id': 0,
         *      'source': {
         *          'mediaSourceType': 'MediaSourceType.media',
         *          'mediaType': 'MediaType.file',
         *          'resource': 'C:/alexmercerind/music.MP3'
         *      }
         * }
         * 
         */
        else if (method->name == "add") {
            int id = method->getArgument<int>("id");
            std::map<flutter::EncodableValue, flutter::EncodableValue> source = std::get<flutter::EncodableMap>(method->arguments[flutter::EncodableValue("source")]);
            std::string mediaType = std::get<std::string>(source[flutter::EncodableValue("mediaType")]);
            std::string resource = std::get<std::string>(source[flutter::EncodableValue("resource")]);
            Media* media = nullptr;
            if (mediaType == "MediaType.file")
                media = Media::file(resource);
            else if (mediaType == "MediaType.network")
                media = Media::network(resource);
            else
                media = Media::asset(resource);
            Player* player = players->get(id);
            player->add(media);
            method->returnNull();
        }
        /* Removed `Media` from the `Playlist` at the given index.
         *
         * Argument:
         * 
         * {
         *      'id': 0,
         *      'index': 1
         * }
         * 
         */
        else if (method->name == "remove") {
            int id = method->getArgument<int>("id");
            int index = method->getArgument<int>("index");
            Player* player = players->get(id);
            player->remove(index);
            method->returnNull();
        }
        /* Inserts `Media` at an index to the `Playlist` of the `Player`.
         *
         * Argument:
         * 
         * {
         *      'id': 0,
         *      'index': 2,
         *      'source': {
         *          'mediaSourceType': 'MediaSourceType.media',
         *          'mediaType': 'MediaType.file',
         *          'resource': 'C:/alexmercerind/music.MP3'
         *      }
         * }
         * 
         */
        else if (method->name == "insert") {
            int id = method->getArgument<int>("id");
            int index = method->getArgument<int>("index");
            std::map<flutter::EncodableValue, flutter::EncodableValue> source = std::get<flutter::EncodableMap>(method->arguments[flutter::EncodableValue("source")]);
            std::string mediaType = std::get<std::string>(source[flutter::EncodableValue("mediaType")]);
            std::string resource = std::get<std::string>(source[flutter::EncodableValue("resource")]);
            Media* media = nullptr;
            if (mediaType == "MediaType.file")
                media = Media::file(resource);
            else if (mediaType == "MediaType.network")
                media = Media::network(resource);
            else
                media = Media::asset(resource);
            Player* player = players->get(id);
            player->insert(index, media);
            method->returnNull();
        }
        /* Moves `Media` from initial index to final index in the `Playlist`.
         *
         * Argument:
         * 
         * {
         *      'id': 0,
         *      'initial': 2,
         *      'final': 0
         * }
         * 
         */
        else if (method->name == "move") {
            int id = method->getArgument<int>("id");
            int initial = method->getArgument<int>("initial");
            int final = method->getArgument<int>("final");
            Player* player = players->get(id);
            player->move(initial, final);
            method->returnNull();
        }
        /* Sets the playback device.
         *
         * Argument:
         * 
         * {
         *      'id': 0,
         *      'device': {
         *          'name': 'An audio device'
         *          'description': 'Audio device description.'
         *      }
         * }
         * 
         */
        else if (method->name == "setDevice") {
            int id = method->getArgument<int>("id");
            std::map<std::string, std::string> _ = method->getArgument<std::map<std::string, std::string>>("device");
            Device* device = new Device(_["id"], _["name"]);
            Player* player = players->get(id);
            player->setDevice(device);
            method->returnNull();
        }
        /* Gets List of all available devices.
         *
         * Argument:
         * 
         * {}
         * 
         */
        else if (method->name == "getDevices") {
            method->returnValue<std::vector<std::map<std::string, std::string>>>(
                devices->get()
            );
        }
        else {
            method->returnNotImplemented();
        }
        method->returnResult();
    }
}

void DartVlcPluginRegisterWithRegistrar(FlutterDesktopPluginRegistrarRef registrar) {
    DartVlcPlugin::RegisterWithRegistrar(flutter::PluginRegistrarManager::GetInstance()->GetRegistrar<flutter::PluginRegistrarWindows>(registrar));
}
