/*
 * dart_vlc: A media playback library for Dart & Flutter. Based on libVLC & libVLC++.
 * 
 * Hitesh Kumar Saini
 * https://github.com/alexmercerind
 * saini123hitesh@gmail.com; alexmercerind@gmail.com
 * 
 * GNU Lesser General Public License v2.1
 */

/* Standard headers. */
#include <unordered_map>
/* Flutter headers */
#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>
#include <flutter/standard_method_codec.h>

#include "include/dart_vlc/dart_vlc_plugin.h"
/* Main FFI __declspec(dllexport) methods. */
#include "native/dart_vlc.cpp"


std::unique_ptr<flutter::MethodChannel<flutter::EncodableValue>> channel;


namespace {


    class DartVlcPlugin : public flutter::Plugin {
    public:
        static void RegisterWithRegistrar(flutter::PluginRegistrarWindows *registrar);

        DartVlcPlugin(flutter::TextureRegistrar* textureRegistrar);

        virtual ~DartVlcPlugin();

    private:
        void HandleMethodCall(const flutter::MethodCall<flutter::EncodableValue> &method_call, std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
        flutter::TextureRegistrar *textureRegistrar = nullptr;
        std::unordered_map<int, uint8_t*> frames;
        std::unordered_map<int, FlutterDesktopPixelBuffer*> buffers;
        std::unordered_map<int, std::unique_ptr<flutter::TextureVariant>> textures;
    };


    void DartVlcPlugin::RegisterWithRegistrar(flutter::PluginRegistrarWindows *registrar) {
        channel = std::make_unique<flutter::MethodChannel<flutter::EncodableValue>>(registrar->messenger(), "dart_vlc", &flutter::StandardMethodCodec::GetInstance());
        auto plugin = std::make_unique<DartVlcPlugin>(registrar->texture_registrar());
        channel->SetMethodCallHandler(
            [plugin_pointer = plugin.get()](const auto &call, auto result) {
                plugin_pointer->HandleMethodCall(call, std::move(result));
            }
        );
        registrar->AddPlugin(std::move(plugin));
    }

    DartVlcPlugin::DartVlcPlugin(flutter::TextureRegistrar* textureRegistrar): textureRegistrar(textureRegistrar) {}

    DartVlcPlugin::~DartVlcPlugin() {}

    void DartVlcPlugin::HandleMethodCall(const flutter::MethodCall<flutter::EncodableValue> &methodCall, std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
        /* No platform channel implementation after migration to FFI except Player::onVideo callbacks for Texture. */

        /* Sets lambda for Player::onVideo callbacks. Called after creating new instance of Player. */
        if (methodCall.method_name() == "Player.onVideo") {
            flutter::EncodableMap arguments = std::get<flutter::EncodableMap>(*methodCall.arguments());
            int playerId = std::get<int>(arguments[flutter::EncodableValue("playerId")]);
            Player* player = players->get(playerId);
            this->frames.insert(
                std::make_pair(playerId, new uint8_t[player->videoHeight * player->videoWidth * 4])
            );
            memset(this->frames.at(playerId), 0, player->videoHeight * player->videoWidth * 4 * sizeof(uint8_t));
            this->buffers.insert(
                std::make_pair(playerId, new FlutterDesktopPixelBuffer())
            );
            this->buffers.at(playerId)->width = player->videoWidth;
            this->buffers.at(playerId)->height = player->videoHeight;
            this->buffers.at(playerId)->buffer = this->frames.at(playerId);
            this->textures[playerId] = std::make_unique<flutter::TextureVariant>(
                flutter::PixelBufferTexture(
                    [=](size_t width, size_t height) -> const FlutterDesktopPixelBuffer* {
                        return this->buffers.at(playerId);
                    }
                )
            );
            int64_t textureId = this->textureRegistrar->RegisterTexture(this->textures[playerId].get());
            this->textureRegistrar->MarkTextureFrameAvailable(textureId);
            player->onVideo(
                [=](uint8_t* frame) -> void {
                    memcpy(this->frames.at(playerId), frame, player->videoHeight * player->videoWidth * 4 * sizeof(uint8_t));
                    this->textureRegistrar->MarkTextureFrameAvailable(textureId);
                }
            );
            result->Success(flutter::EncodableValue(textureId));
        }
        /* Called after disposing a Player instance. */
        else if (methodCall.method_name() == "Player.dispose") {
            flutter::EncodableMap arguments = std::get<flutter::EncodableMap>(*methodCall.arguments());
            int playerId = std::get<int>(arguments[flutter::EncodableValue("playerId")]);
            delete this->frames.at(playerId);
            delete this->buffers.at(playerId);
            result->Success(flutter::EncodableValue(nullptr));
        }
        else {
            result->NotImplemented();
        }
    }
}

void DartVlcPluginRegisterWithRegistrar(FlutterDesktopPluginRegistrarRef registrar) {
    DartVlcPlugin::RegisterWithRegistrar(flutter::PluginRegistrarManager::GetInstance()->GetRegistrar<flutter::PluginRegistrarWindows>(registrar));
}
