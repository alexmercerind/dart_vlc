/*
 * dart_vlc: A media playback library for Dart & Flutter. Based on libVLC &
 * libVLC++.
 *
 * Hitesh Kumar Saini
 * https://github.com/alexmercerind
 * saini123hitesh@gmail.com; alexmercerind@gmail.com
 *
 * GNU Lesser General Public License v2.1
 */

#include "include/dart_vlc/dart_vlc_plugin.h"

#include <unordered_map>

#include "player.h"
#include "video_outlet.h"

namespace {

class DartVlcPlugin : public flutter::Plugin {
 public:
  static void RegisterWithRegistrar(flutter::PluginRegistrarWindows* registrar);

  flutter::MethodChannel<flutter::EncodableValue>* channel() const {
    return channel_.get();
  }

  DartVlcPlugin(
      std::unique_ptr<flutter::MethodChannel<flutter::EncodableValue>> channel,
      flutter::TextureRegistrar* texture_registrar);

  virtual ~DartVlcPlugin();

 private:
  void HandleMethodCall(
      const flutter::MethodCall<flutter::EncodableValue>& method_call,
      std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);

  flutter::TextureRegistrar* texture_registrar_;
  std::unique_ptr<flutter::MethodChannel<flutter::EncodableValue>> channel_;
  std::unordered_map<int, std::unique_ptr<VideoOutlet>> outlets_;
};

void DartVlcPlugin::RegisterWithRegistrar(
    flutter::PluginRegistrarWindows* registrar) {
  auto plugin = std::make_unique<DartVlcPlugin>(
      std::make_unique<flutter::MethodChannel<flutter::EncodableValue>>(
          registrar->messenger(), "dart_vlc",
          &flutter::StandardMethodCodec::GetInstance()),
      registrar->texture_registrar());
  plugin->channel()->SetMethodCallHandler(
      [plugin_pointer = plugin.get()](const auto& call, auto result) {
        plugin_pointer->HandleMethodCall(call, std::move(result));
      });
  registrar->AddPlugin(std::move(plugin));
}

DartVlcPlugin::DartVlcPlugin(
    std::unique_ptr<flutter::MethodChannel<flutter::EncodableValue>> channel,
    flutter::TextureRegistrar* texture_registrar)
    : channel_(std::move(channel)), texture_registrar_(texture_registrar) {}

DartVlcPlugin::~DartVlcPlugin() {}

void DartVlcPlugin::HandleMethodCall(
    const flutter::MethodCall<flutter::EncodableValue>& method_call,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
  if (method_call.method_name() == "PlayerRegisterTexture") {
    flutter::EncodableMap arguments =
        std::get<flutter::EncodableMap>(*method_call.arguments());
    int32_t player_id =
        std::get<int>(arguments[flutter::EncodableValue("playerId")]);

    auto [it, added] = outlets_.try_emplace(player_id, nullptr);
    if (added) {
      it->second = std::make_unique<VideoOutlet>(texture_registrar_);

      Player* player = g_players->Get(player_id);
      player->OnVideo([outlet_ptr = it->second.get()](uint8_t* frame,
                                                      int32_t width,
                                                      int32_t height) -> void {
        outlet_ptr->OnVideo(frame, width, height);
      });
    }

    return result->Success(flutter::EncodableValue(it->second->texture_id()));

  } else if (method_call.method_name() == "PlayerUnregisterTexture") {
    flutter::EncodableMap arguments =
        std::get<flutter::EncodableMap>(*method_call.arguments());
    int player_id =
        std::get<int>(arguments[flutter::EncodableValue("playerId")]);
    if (outlets_.find(player_id) == outlets_.end()) {
      return result->Error("-2", "Texture was not found.");
    }

    // The callback must be unregistered
    // before destroying the outlet.
    Player* player = g_players->Get(player_id);
    player->OnVideo(nullptr);

    outlets_.erase(player_id);

    result->Success(flutter::EncodableValue(nullptr));
  } else {
    result->NotImplemented();
  }
}
}  // namespace

void DartVlcPluginRegisterWithRegistrar(
    FlutterDesktopPluginRegistrarRef registrar) {
  DartVlcPlugin::RegisterWithRegistrar(
      flutter::PluginRegistrarManager::GetInstance()
          ->GetRegistrar<flutter::PluginRegistrarWindows>(registrar));
}
