// This file is a part of dart_vlc (https://github.com/alexmercerind/dart_vlc)
//
// Copyright (C) 2021-2022 Hitesh Kumar Saini <saini123hitesh@gmail.com>
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#include "include/dart_vlc/dart_vlc_plugin.h"

#include <unordered_map>

#include "core.h"
#include "include/dart_vlc/video_outlet.h"
#include "include/dart_vlc/win32_window.h"

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

DartVlcPlugin::~DartVlcPlugin() {
  for (const auto& [player_id, outlet] : outlets_) {
    auto player = g_players->Get(player_id);
    player->SetVideoFrameCallback(nullptr);
    g_players->Dispose(player_id);
  }
}

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
      auto player = g_players->Get(player_id);
      player->SetVideoFrameCallback(
          [outlet_ptr = it->second.get()](uint8_t* frame, int32_t width,
                                          int32_t height) -> void {
            outlet_ptr->MarkVideoFrameAvailable(frame, width, height);
          });
    }

    return result->Success(flutter::EncodableValue(it->second->texture_id()));
  } else if (method_call.method_name() == "PlayerUnregisterTexture") {
    flutter::EncodableMap arguments =
        std::get<flutter::EncodableMap>(*method_call.arguments());
    auto player_id =
        std::get<int>(arguments[flutter::EncodableValue("playerId")]);
    if (outlets_.find(player_id) == outlets_.end()) {
      return result->Error("-2", "Texture was not found.");
    }
    auto player = g_players->Get(player_id);
    player->SetVideoFrameCallback(nullptr);
    outlets_.erase(player_id);
    result->Success(flutter::EncodableValue(nullptr));
  } else if (method_call.method_name() == "PlayerCreateHWND") {
    flutter::EncodableMap arguments =
        std::get<flutter::EncodableMap>(*method_call.arguments());
    auto player_id =
        std::get<int>(arguments[flutter::EncodableValue("playerId")]);
    auto player = g_players->Get(player_id);
    auto window = CreateWin32Window();
    player->SetHWND(reinterpret_cast<int64_t>(window));
    result->Success(flutter::EncodableValue(reinterpret_cast<int64_t>(window)));
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
