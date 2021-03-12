#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>
#include <flutter/standard_method_codec.h>
#include "include/flutter_vlc/flutter_vlc_plugin.h"
#include "include/flutter_types.hpp"

#include "main.cpp"


std::unique_ptr<flutter::MethodChannel<flutter::EncodableValue>> channel;

void event(AudioPlayerState* &state) {
    /*
     * Method to notify Dart about playback events.
     *
     * Argument:
     * 
     * {
     *      'type': 'event',
     *      'id': 0,
     *      'index': 1,
     *      'audios': [
     *          {
     *              'audioSourceType': 'AudioSourceType.audio',
     *              'audioType': 'AudioType.network',
     *              'resource': 'https://alexmercerind.com/music.MP3'
     *          },
     *          {
     *              'audioSourceType': 'AudioSourceType.audio',
     *              'audioType': AudioType.'file',
     *              'resource': 'C:/alexmercerind/music.MP3'
     *          }
     *      ],
     *      'isPlaying': true,
     *      'isValid': true,
     *      'isSeekable': false,
     *      'isCompleted': false,
     *      'position': 56783,
     *      'duration': 370278,
     *      'volume': 1.0,
     *      'rate': 1.25,
     *      'isPlaylist': true
     * }
     * 
     */
    channel->InvokeMethod(
        "audioPlayerState",
        std::unique_ptr<flutter::EncodableValue>(
            new flutter::EncodableValue(
                flutter::EncodableMap(
                    {
                        {
                            flutter::EncodableValue("type"),
                            flutter::EncodableValue("event")
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
                            flutter::EncodableValue("audios"),
                            InvokeMethodHandler::getValue<std::vector<std::map<std::string, std::string>>>(
                                state->audios->get()
                            )
                        },
                        {
                            flutter::EncodableValue("isPlaying"),
                            flutter::EncodableValue(state->isPlaying)
                        },
                        {
                            flutter::EncodableValue("isValid"),
                            flutter::EncodableValue(state->isValid)
                        },
                        {
                            flutter::EncodableValue("isSeekable"),
                            flutter::EncodableValue(state->isSeekable)
                        },
                        {
                            flutter::EncodableValue("isCompleted"),
                            flutter::EncodableValue(state->isCompleted)
                        },
                        {
                            flutter::EncodableValue("position"),
                            flutter::EncodableValue(state->position)
                        },
                        {
                            flutter::EncodableValue("duration"),
                            flutter::EncodableValue(state->duration)
                        },
                        {
                            flutter::EncodableValue("volume"),
                            flutter::EncodableValue(state->volume)
                        },
                        {
                            flutter::EncodableValue("rate"),
                            flutter::EncodableValue(state->rate)
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

void exception(AudioPlayerState* &state) {
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
        "audioPlayerState",
        std::unique_ptr<flutter::EncodableValue>(
            new flutter::EncodableValue(
                flutter::EncodableMap(
                    {
                        {
                            flutter::EncodableValue("type"),
                            flutter::EncodableValue("exception")
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


    class FlutterVlcPlugin : public flutter::Plugin {
    public:
        static void RegisterWithRegistrar(flutter::PluginRegistrarWindows *registrar);

        FlutterVlcPlugin();

        virtual ~FlutterVlcPlugin();

    private:
        void HandleMethodCall(const flutter::MethodCall<flutter::EncodableValue> &method_call, std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    };


    void FlutterVlcPlugin::RegisterWithRegistrar(flutter::PluginRegistrarWindows *registrar) {
        channel = std::make_unique<flutter::MethodChannel<flutter::EncodableValue>>(registrar->messenger(), "flutter_vlc", &flutter::StandardMethodCodec::GetInstance());
        auto plugin = std::make_unique<FlutterVlcPlugin>();
        channel->SetMethodCallHandler(
            [plugin_pointer = plugin.get()](const auto &call, auto result) {
                plugin_pointer->HandleMethodCall(call, std::move(result));
            }
        );
        registrar->AddPlugin(std::move(plugin));
    }

    FlutterVlcPlugin::FlutterVlcPlugin() {}

    FlutterVlcPlugin::~FlutterVlcPlugin() {}

    void FlutterVlcPlugin::HandleMethodCall(const flutter::MethodCall<flutter::EncodableValue> &methodCall, std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
        MethodCallHandler* method = new MethodCallHandler(&methodCall, std::move(result));
        /*
         * Creates a new [AudioPlayer] instance & setups event & exception callbacks.
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
            AudioPlayer* audioPlayer = audioPlayers->get(id);
            audioPlayer->onEvent(
                [audioPlayer] () -> void {
                    event(audioPlayer->state);
                }
            );
            audioPlayer->onException(
                [audioPlayer] () -> void {
                    exception(audioPlayer->state);
                }
            );
            method->returnNull();
        }
        /*
         * Opens an [AudioSource] i.e [Audio] or [Playlist] into the [AudioPlayer].
         * 
         * Argument for loading an [Audio]:
         * 
         * {
         *      'id': 0,
         *      'autoStart': true,
         *      'source': {
         *          'audioSourceType': 'AudioSourceType.audio',
         *          'audioType': 'AudioType.file',
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
         *          'audioSourceType': 'AudioSourceType.playlist',
         *          'start': 0,
         *          'audios': [
         *              {
         *                  'audioSourceType': 'AudioSourceType.audio',
         *                  'audioType': 'AudioType.file',
         *                  'resource': 'C:/alexmercerind/music.MP3'
         *              },
         *              {
         *                  'audioSourceType': 'AudioSourceType.audio',
         *                  'type': 'AudioType.network',
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
            std::string audioSourceType = std::get<std::string>(source[flutter::EncodableValue("audioSourceType")]);
            if (audioSourceType == "AudioSourceType.audio") {
                std::string audioType = std::get<std::string>(source[flutter::EncodableValue("audioType")]);
                std::string resource = std::get<std::string>(source[flutter::EncodableValue("resource")]);
                Audio* audio = nullptr;
                if (audioType == "AudioType.file")
                    audio = Audio::file(resource);
                else if (audioType == "AudioType.network")
                    audio = Audio::network(resource);
                else
                    audio = Audio::asset(resource);
                AudioPlayer* audioPlayer = audioPlayers->get(id);
                audioPlayer->open(audio, autoStart);
            }
            if (audioSourceType == "AudioSourceType.playlist") {
                std::vector<Audio*> audios;
                std::vector<std::map<std::string, std::string>> audiosMap = MethodCallHandler::getValue<std::vector<std::map<std::string, std::string>>>(source[flutter::EncodableValue("audios")]);
                for (std::map<std::string,std::string> audioMap: audiosMap) {
                    Audio* audio;
                    if (audioMap["audioType"] == "AudioType.file")
                        audio = Audio::file(audioMap["resource"]);
                    else if (audioMap["audioType"] == "AudioType.network")
                        audio = Audio::network(audioMap["resource"]);
                    else
                        audio = Audio::asset(audioMap["resource"]);
                    audios.emplace_back(audio);
                }
                AudioPlayer* audioPlayer = audioPlayers->get(id);
                audioPlayer->open(
                    new Playlist(audios),
                    autoStart
                );
            }
            method->returnNull();
        }
        /*
         * Plays the [AudioPlayer] instance.
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
            AudioPlayer* audioPlayer = audioPlayers->get(id);
            audioPlayer->play();
            method->returnNull();
        }
        /*
         * Pauses the [AudioPlayer] instance.
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
            AudioPlayer* audioPlayer = audioPlayers->get(id);
            audioPlayer->pause();
            method->returnNull();
        }
        /*
         * Stops the [AudioPlayer] instance.
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
            AudioPlayer* audioPlayer = audioPlayers->get(id);
            audioPlayer->stop();
            method->returnNull();
        }
        /*
         * Jumps to next audio in the [Playlist] loaded in [AudioPlayer] instance.
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
            AudioPlayer* audioPlayer = audioPlayers->get(id);
            audioPlayer->next();
            method->returnNull();
        }
        /*
         * Jumps to previous audio in the [Playlist] loaded in [AudioPlayer] instance.
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
            AudioPlayer* audioPlayer = audioPlayers->get(id);
            audioPlayer->back();
            method->returnNull();
        }
        /*
         * Jumps to specific index in the [Playlist] loaded in [AudioPlayer] instance.
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
            AudioPlayer* audioPlayer = audioPlayers->get(id);
            audioPlayer->jump(index);
            method->returnNull();
        }
        /*
         * Seeks [AudioPlayer] instance.
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
            AudioPlayer* audioPlayer = audioPlayers->get(id);
            audioPlayer->seek(duration);
            method->returnNull();
        }
        /*
         * Sets volume of [AudioPlayer] instance.
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
            AudioPlayer* audioPlayer = audioPlayers->get(id);
            audioPlayer->setVolume(volume);
            method->returnNull();
        }
        /*
         * Sets playback rate of [AudioPlayer] instance.
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
            AudioPlayer* audioPlayer = audioPlayers->get(id);
            audioPlayer->setRate(rate);
            method->returnNull();
        }
        else {
            method->returnNotImplemented();
        }
        method->returnResult();
    }
}

void FlutterVlcPluginRegisterWithRegistrar(FlutterDesktopPluginRegistrarRef registrar) {
    FlutterVlcPlugin::RegisterWithRegistrar(flutter::PluginRegistrarManager::GetInstance()->GetRegistrar<flutter::PluginRegistrarWindows>(registrar));
}
