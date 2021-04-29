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
     *              'id': 0,
     *              'mediaSourceType': 'MediaSourceType.media',
     *              'mediaType': 'MediaType.network',
     *              'resource': 'https://alexmercerind.com/music.MP3'
     *          },
     *          {
     *              'id': 1,
     *              'mediaSourceType': 'MediaSourceType.media',
     *              'mediaType': MediaType.file',
     *              'resource': 'C:/alexmercerind/music.MP3'
     *          }
     *      ],
     *      'isPlaylist': true
     * }
     * 
     */
    flutter::EncodableList mediaMaps = flutter::EncodableList();
    for (Media* media: state->medias->medias) {
        flutter::EncodableMap mediaMap = flutter::EncodableMap();
        mediaMap[flutter::EncodableValue("id")] = flutter::EncodableValue(media->id);
        mediaMap[flutter::EncodableValue("mediaSourceType")] = flutter::EncodableValue("MediaSourceType.media");
        mediaMap[flutter::EncodableValue("mediaType")] = flutter::EncodableValue(media->mediaType);
        mediaMap[flutter::EncodableValue("resource")] = flutter::EncodableValue(media->resource);
        mediaMaps.emplace_back(mediaMap);
    }
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
                            mediaMaps,
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
        std::unique_ptr<MethodCallHandler> method = std::make_unique<MethodCallHandler>(&methodCall, std::move(result));

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
        if (method->name == "Player.create") {
            int id = method->getArgument<int>("id");
            int videoWidth = method->getArgument<int>("videoWidth");
            int videoHeight = method->getArgument<int>("videoHeight");
            Player* player = players->get(id);
            player->videoWidth = videoWidth;
            player->videoHeight = videoHeight;
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
            if (videoWidth > 0 && videoHeight > 0) {
                player->onVideo(
                    [=](uint8_t* frame) -> void {
                        channel->InvokeMethod(
                            "videoFrame",
                            std::unique_ptr<flutter::EncodableValue>(
                                new flutter::EncodableValue(
                                    flutter::EncodableMap(
                                        {
                                            {
                                                flutter::EncodableValue("id"),
                                                flutter::EncodableValue(player->state->id)
                                            },
                                            {
                                                flutter::EncodableValue("videoWidth"),
                                                flutter::EncodableValue(player->videoWidth)
                                            },
                                            {
                                                flutter::EncodableValue("videoHeight"),
                                                flutter::EncodableValue(player->videoHeight)
                                            },
                                            {
                                                flutter::EncodableValue("byteArray"),
                                                flutter::EncodableValue(
                                                    std::vector<uint8_t>(frame, frame + (player->videoWidth * player->videoHeight * 4))
                                                )
                                            }
                                        }
                                    )
                                )
                            )
                        );
                    }
                );
            }
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
         *          'id': 0,
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
         *                  'id': 0,
         *                  'mediaSourceType': 'MediaSourceType.media',
         *                  'mediaType': 'MediaType.file',
         *                  'resource': 'C:/alexmercerind/music.MP3'
         *              },
         *              {
         *                  'id': 0,
         *                  'mediaSourceType': 'MediaSourceType.media',
         *                  'mediaType': 'MediaType.network',
         *                  'resource': 'C:/alexmercerind/music.MP3'
         *              }
         *          ]
         *      }
         * }
         */
        else if (method->name == "Player.open") {
            int id = method->getArgument<int>("id");
            bool autoStart = method->getArgument<bool>("autoStart");
            std::map<flutter::EncodableValue, flutter::EncodableValue> source = std::get<flutter::EncodableMap>(method->arguments[flutter::EncodableValue("source")]);
            std::string mediaSourceType = std::get<std::string>(source[flutter::EncodableValue("mediaSourceType")]);
            if (mediaSourceType == "MediaSourceType.media") {
                int mediaId = std::get<int>(source[flutter::EncodableValue("id")]);
                std::string mediaType = std::get<std::string>(source[flutter::EncodableValue("mediaType")]);
                std::string resource = std::get<std::string>(source[flutter::EncodableValue("resource")]);
                Media* media = nullptr;
                if( mediaType == "MediaType.file" ){
                    media = Media::file(mediaId, resource);
                } else if ( mediaType == "MediaType.network" ){
                    media = Media::network(mediaId, resource);
                } else {
                    media = Media::asset(mediaId, resource);
                }
                Player* player = players->get(id);
                player->open(media, autoStart);
            }
            if (mediaSourceType == "MediaSourceType.playlist") {
                std::vector<Media*> medias;
                flutter::EncodableList mediaList = std::get<flutter::EncodableList>(source[flutter::EncodableValue("medias")]);
                std::string _playlistMode = std::get<std::string>(source[flutter::EncodableValue("playlistMode")]);
                for (flutter::EncodableValue encodedMedia: mediaList) {
                    flutter::EncodableMap mediaMap = std::get<flutter::EncodableMap>(encodedMedia);
                    int mediaId = std::get<int>(mediaMap[flutter::EncodableValue("id")]);
                    std::string mediaType = std::get<std::string>(mediaMap[flutter::EncodableValue("mediaType")]);
                    std::string resource = std::get<std::string>(mediaMap[flutter::EncodableValue("resource")]);
                    Media* media;
                    if(mediaType == "MediaType.file")
                        media = Media::file(mediaId, resource);
                    else if (mediaType == "MediaType.network")
                        media = Media::network(mediaId, resource);
                    else
                        media = Media::asset(mediaId, resource);                     
                    medias.emplace_back(media);
                }
                Player* player = players->get(id);
                PlaylistMode playlistMode;
                if (_playlistMode == "PlaylistMode.repeat")
                    playlistMode = PlaylistMode::repeat;
                else if (_playlistMode == "PlaylistMode.loop")
                    playlistMode = PlaylistMode::loop;
                else
                    playlistMode = PlaylistMode::single;
                player->setPlaylistMode(playlistMode);
                player->open(
                    new Playlist(medias, playlistMode),
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
        else if (method->name == "Player.play") {
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
        else if (method->name == "Player.pause") {
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
        else if (method->name == "Player.playOrPause") {
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
        else if (method->name == "Player.stop") {
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
        else if (method->name == "Player.next") {
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
        else if (method->name == "Player.back") {
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
        else if (method->name == "Player.jump") {
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
        else if (method->name == "Player.seek") {
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
        else if (method->name == "Player.setVolume") {
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
        else if (method->name == "Player.setRate") {
            int id = method->getArgument<int>("id");
            float rate = method->getArgument<float>("rate");
            Player* player = players->get(id);
            player->setRate(rate);
            method->returnNull();
        }
        /*
         * Assign PlaylistMode to [Playlist].
         * 
         * Argument:
         * 
         * {
         *      'id': 0,
         *      'playlistMode': 'PlaylistMode.single'
         * }
         * 
         */
        else if (method->name == "Player.setPlaylistMode") {
            int id = method->getArgument<int>("id");
            std::string _playlistMode = method->getArgument<std::string>("playlistMode");
            Player* player = players->get(id);
            PlaylistMode playlistMode;
            if (_playlistMode == "PlaylistMode.repeat")
                playlistMode = PlaylistMode::repeat;
            else if (_playlistMode == "PlaylistMode.loop")
                playlistMode = PlaylistMode::loop;
            else
                playlistMode = PlaylistMode::single;
            player->setPlaylistMode(playlistMode);
            method->returnNull();
        }
        /* Adds new `Media` to the end of the `Playlist` of the `Player`.
         *
         * Argument:
         * 
         * {
         *      'id': 0,
         *      'source': {
         *          'id': 0,
         *          'mediaSourceType': 'MediaSourceType.media',
         *          'mediaType': 'MediaType.file',
         *          'resource': 'C:/alexmercerind/music.MP3'
         *      }
         * }
         * 
         */
        else if (method->name == "Player.add") {
            int id = method->getArgument<int>("id");
            std::map<flutter::EncodableValue, flutter::EncodableValue> source = std::get<flutter::EncodableMap>(method->arguments[flutter::EncodableValue("source")]);
            int mediaId = std::get<int>(source[flutter::EncodableValue("id")]);
            std::string mediaType = std::get<std::string>(source[flutter::EncodableValue("mediaType")]);
            std::string resource = std::get<std::string>(source[flutter::EncodableValue("resource")]);
            Media* media = nullptr;
            if (mediaType == "MediaType.file")
                media = Media::file(mediaId, resource);
            else if (mediaType == "MediaType.network")
                media = Media::network(mediaId, resource);
            else
                media = Media::asset(mediaId, resource);
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
        else if (method->name == "Player.remove") {
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
        else if (method->name == "Player.insert") {
            int id = method->getArgument<int>("id");
            int index = method->getArgument<int>("index");
            std::map<flutter::EncodableValue, flutter::EncodableValue> source = std::get<flutter::EncodableMap>(method->arguments[flutter::EncodableValue("source")]);
            int mediaId = std::get<int>(source[flutter::EncodableValue("id")]);
            std::string mediaType = std::get<std::string>(source[flutter::EncodableValue("mediaType")]);
            std::string resource = std::get<std::string>(source[flutter::EncodableValue("resource")]);
            Media* media = nullptr;
            if (mediaType == "MediaType.file")
                media = Media::file(mediaId, resource);
            else if (mediaType == "MediaType.network")
                media = Media::network(mediaId, resource);
            else
                media = Media::asset(mediaId, resource);
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
        else if (method->name == "Player.move") {
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
         *          'id': '{0.0.0.00000000}.{743d4fd8-998f-44ab-9a46-cef1a7ae37dc}'
         *          'name': 'Speakers (High Definition Audio Device)'
         *      }
         * }
         * 
         */
        else if (method->name == "Player.setDevice") {
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
        else if (method->name == "Devices.all") {
            method->returnValue<std::vector<std::map<std::string, std::string>>>(
                devices->get()
            );
        }
        /* Parses [Media] for retrieving its meta.
         *
         * Argument:
         * 
         * {
         *      'timeout': 10000,
         *      'source': {
         *          'mediaSourceType': 'MediaSourceType.media',
         *          'mediaType': 'MediaType.file',
         *          'resource': 'C:/alexmercerind/music.MP3'
         *      }
         * }
         * 
         */
        else if (method->name == "Media.parse") {
            int timeout = method->getArgument<int>("timeout");
            std::map<flutter::EncodableValue, flutter::EncodableValue> source = std::get<flutter::EncodableMap>(method->arguments[flutter::EncodableValue("source")]);
            int mediaId = std::get<int>(source[flutter::EncodableValue("id")]);
            std::string mediaType = std::get<std::string>(source[flutter::EncodableValue("mediaType")]);
            std::string resource = std::get<std::string>(source[flutter::EncodableValue("resource")]);
            Media* media = nullptr;
            if (mediaType == "MediaType.file")
                media = Media::file(mediaId, resource, true, timeout);
            else if (mediaType == "MediaType.network")
                media = Media::network(mediaId, resource, true, timeout);
            else
                media = Media::asset(mediaId, resource, true, timeout);
            method->returnValue<std::map<std::string, std::string>>(media->metas);
        }
        /* Creates new [Broadcast] instance.
         *
         * Argument:
         * 
         * {
         *      'id': 0,
         *      'media': {
         *          'id': 0,
         *          'mediaSourceType': 'MediaSourceType.media',
         *          'mediaType': 'MediaType.file',
         *          'resource': 'C:/alexmercerind/music.MP3'
         *      },
         *      'configuration': {
         *          'access': 'http',
         *          'mux': 'mpeg1',
         *          'dst': '127.0.0.1:8080',
         *          'vcodec': 'mp1v',
         *          'vb': 1024,
         *          'acodec': 'mpga',
         *          'ab': 128
         *     }
         * }
         * 
         */
        else if (method->name == "Broadcast.create") {
            int id = method->getArgument<int>("id");
            std::map<flutter::EncodableValue, flutter::EncodableValue> _media = std::get<flutter::EncodableMap>(method->arguments[flutter::EncodableValue("media")]);
            int mediaId = std::get<int>(_media[flutter::EncodableValue("id")]);
            std::string mediaType = std::get<std::string>(_media[flutter::EncodableValue("mediaType")]);
            std::string resource = std::get<std::string>(_media[flutter::EncodableValue("resource")]);
            Media* media = nullptr;
            if (mediaType == "MediaType.file")
                media = Media::file(mediaId, resource);
            else if (mediaType == "MediaType.network")
                media = Media::network(mediaId, resource);
            else if (mediaType == "MediaType.asset")
                media = Media::asset(mediaId, resource);
            else 
                media = Media::directShow(mediaId, resource);
            std::map<flutter::EncodableValue, flutter::EncodableValue> _configuration = std::get<flutter::EncodableMap>(method->arguments[flutter::EncodableValue("configuration")]);
            std::string access = std::get<std::string>(_configuration[flutter::EncodableValue("access")]);
            std::string mux = std::get<std::string>(_configuration[flutter::EncodableValue("mux")]);
            std::string dst = std::get<std::string>(_configuration[flutter::EncodableValue("dst")]);
            std::string vcodec = std::get<std::string>(_configuration[flutter::EncodableValue("vcodec")]);
            int vb = std::get<int>(_configuration[flutter::EncodableValue("vb")]);
            std::string acodec = std::get<std::string>(_configuration[flutter::EncodableValue("acodec")]);
            int ab = std::get<int>(_configuration[flutter::EncodableValue("ab")]);
            BroadcastConfiguration* configuration = new BroadcastConfiguration(
                access, mux, dst, vcodec, vb, acodec, ab
            );
            broadcasts->get(id, media, configuration);
            method->returnNull();
        }
        /*
         * Starts the [Broadcast].
         * 
         * Argument:
         * 
         * {
         *      'id': 0
         * }
         * 
         */
        else if (method->name == "Broadcast.start") {
            int id = method->getArgument<int>("id");
            Broadcast* broadcast = broadcasts->get(id, nullptr, nullptr);
            broadcast->start();
            method->returnNull();
        }
        /*
         * Disposes the [Broadcast] instance.
         * 
         * Argument:
         * 
         * {
         *      'id': 0
         * }
         * 
         */
        else if (method->name == "Broadcast.dispose") {
            int id = method->getArgument<int>("id");
            Broadcast* broadcast = broadcasts->get(id, nullptr, nullptr);
            broadcast->dispose();
            method->returnNull();
        } 
        
        /* Creates new [Chromecast] instance.
        *
        * Argument:
        * 
        * {
        *      'id': 0,
        *      'media': {
        *          'id': 0,
        *          'mediaSourceType': 'MediaSourceType.media',
        *          'mediaType': 'MediaType.file',
        *          'resource': 'C:/alexmercerind/music.MP3'
        *      },
        *      'ipAddress': '192.168.1.XXX'
        * }
        * 
        */
        else if(method->name == "Chromecast.create"){
            int id = method->getArgument<int>("id");
            std::string ipAddress = method->getArgument<std::string>("ipAddress");
            std::map<flutter::EncodableValue, flutter::EncodableValue> _media = std::get<flutter::EncodableMap>(method->arguments[flutter::EncodableValue("media")]);
            int mediaId = std::get<int>(_media[flutter::EncodableValue("id")]);
            std::string mediaType = std::get<std::string>(_media[flutter::EncodableValue("mediaType")]);
            std::string resource = std::get<std::string>(_media[flutter::EncodableValue("resource")]);            
            Media* media = nullptr;
            if (mediaType == "MediaType.file")
                media = Media::file(mediaId, resource);
            else if (mediaType == "MediaType.network")
                media = Media::network(mediaId, resource);
            else if (mediaType == "MediaType.asset")
                media = Media::asset(mediaId, resource);
            else 
                media = Media::directShow(mediaId, resource);

            chromecasts->get(id, media, ipAddress);
            method->returnNull();
        } 
        
        /* Send content [Chromecast] instance.
        *
        * Argument:
        * 
        * {
        *      'id': 0,
        * }
        * 
        */
        
        else if(method->name == "Chromecast.send"){
            int id = method->getArgument<int>("id");
            Chromecast* chromecast = chromecasts->get(id, nullptr, "");
            chromecast->send();
            method->returnNull();
        } 
        
        /* Disposes the [Chromecast] instance.
        *
        * Argument:
        * 
        * {
        *      'id': 0,
        * }
        * 
        */
        else if(method->name == "Chromecast.dispose"){
            int id = method->getArgument<int>("id");
            Chromecast* chromecast = chromecasts->get(id, nullptr, "");
            chromecast->dispose();
            method->returnNull();
        }

        /* Creates new [Record] instance.
         *
         * Argument:
         * 
         * {
         *      'id': 0,
         *      'media': {
         *          'id': 0,
         *          'mediaSourceType': 'MediaSourceType.media',
         *          'mediaType': 'MediaType.file',
         *          'resource': 'C:/alexmercerind/music.MP3'
         *      },
         *      'savingFile': 'C:/alexmercerind/recordAudio.MP3'
         * }
         * 
         */
        else if (method->name == "Record.create") {
            int id = method->getArgument<int>("id");
            std::string savingFile = method->getArgument<std::string>("savingFile");
            std::map<flutter::EncodableValue, flutter::EncodableValue> _media = std::get<flutter::EncodableMap>(method->arguments[flutter::EncodableValue("media")]);
            int mediaId = std::get<int>(_media[flutter::EncodableValue("id")]);
            std::string mediaType = std::get<std::string>(_media[flutter::EncodableValue("mediaType")]);
            std::string resource = std::get<std::string>(_media[flutter::EncodableValue("resource")]);            
            Media* media = nullptr;
            if (mediaType == "MediaType.file")
                media = Media::file(mediaId, resource);
            else if (mediaType == "MediaType.network")
                media = Media::network(mediaId, resource);
            else if (mediaType == "MediaType.asset")
                media = Media::asset(mediaId, resource);
            else 
                media = Media::directShow(mediaId, resource);

            records->get(id, media, savingFile);
            method->returnNull();
        }
        /*
         * Starts the [Record] instance.
         * 
         * Argument:
         * 
         * {
         *      'id': 0
         * }
         * 
        */
        else if (method->name == "Record.start") {
            int id = method->getArgument<int>("id");
            Record* record = records->get(id, nullptr, "");
            record->start();
            method->returnNull();
        }
        /*
         * Disposes the [Record] instance.
         * 
         * Argument:
         * 
         * {
         *      'id': 0
         * }
         * 
        */
        else if (method->name == "Record.dispose") {
            int id = method->getArgument<int>("id");
            Record* record = records->get(id, nullptr, "");
            record->dispose();
            method->returnNull();
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
