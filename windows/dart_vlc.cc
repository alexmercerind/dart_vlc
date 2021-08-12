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

/* Standard headers. */
#include <mutex>
#include <unordered_map>
/* Flutter headers */
#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>
#include <flutter/standard_method_codec.h>

#include "include/dart_vlc/dart_vlc_plugin.h"
/* Main FFI __declspec(dllexport) methods. */
#include "native/dart_vlc.cpp"

static std::unique_ptr<flutter::MethodChannel<flutter::EncodableValue>>
    g_channel = nullptr;

namespace {

class VideoOutlet {
 public:
  VideoOutlet(size_t width, size_t height);

  flutter::TextureVariant* texture() const { return texture_.get(); }
  void OnFrame(const uint8_t* frame);

 private:
  const size_t buffer_size_;
  mutable std::mutex mutex_;
  std::unique_ptr<flutter::TextureVariant> texture_;
  std::unique_ptr<uint8_t> buffer_;
  FlutterDesktopPixelBuffer flutter_pixel_buffer_;

  const FlutterDesktopPixelBuffer* CopyPixelBuffer() const;
};

VideoOutlet::VideoOutlet(size_t width, size_t height)
    : buffer_size_(width * height * 4 * sizeof(uint8_t)),
      buffer_(new uint8_t[buffer_size_]) {
  memset(buffer_.get(), 0, buffer_size_);

  flutter_pixel_buffer_.width = width;
  flutter_pixel_buffer_.height = height;
  flutter_pixel_buffer_.buffer = buffer_.get();

  texture_ =
      std::make_unique<flutter::TextureVariant>(flutter::PixelBufferTexture(
          [=](size_t width, size_t height) -> const FlutterDesktopPixelBuffer* {
            return CopyPixelBuffer();
          }));
}

void VideoOutlet::OnFrame(const uint8_t* buffer) {
  const std::lock_guard<std::mutex> lock(mutex_);
  memcpy(buffer_.get(), buffer, buffer_size_);
}

const FlutterDesktopPixelBuffer* VideoOutlet::CopyPixelBuffer() const {
  const std::lock_guard<std::mutex> lock(mutex_);
  return &flutter_pixel_buffer_;
}

class DartVlcPlugin : public flutter::Plugin {
 public:
  static void RegisterWithRegistrar(flutter::PluginRegistrarWindows* registrar);

  DartVlcPlugin(flutter::TextureRegistrar* textureRegistrar);

  virtual ~DartVlcPlugin();

 private:
  void HandleMethodCall(
      const flutter::MethodCall<flutter::EncodableValue>& method_call,
      std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
  flutter::TextureRegistrar* textureRegistrar = nullptr;
  std::unordered_map<int, std::pair<int64_t, std::shared_ptr<VideoOutlet>>>
      outlets;
};

void DartVlcPlugin::RegisterWithRegistrar(
    flutter::PluginRegistrarWindows* registrar) {
  g_channel = std::make_unique<flutter::MethodChannel<flutter::EncodableValue>>(
      registrar->messenger(), "dart_vlc",
      &flutter::StandardMethodCodec::GetInstance());
  auto plugin = std::make_unique<DartVlcPlugin>(registrar->texture_registrar());
  g_channel->SetMethodCallHandler([plugin_pointer = plugin.get()](
      const auto& call, auto result) {
    plugin_pointer->HandleMethodCall(call, std::move(result));
  });
  registrar->AddPlugin(std::move(plugin));
}

DartVlcPlugin::DartVlcPlugin(flutter::TextureRegistrar* textureRegistrar)
    : textureRegistrar(textureRegistrar) {}

DartVlcPlugin::~DartVlcPlugin() {}

void DartVlcPlugin::HandleMethodCall(
    const flutter::MethodCall<flutter::EncodableValue>& methodCall,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
  /* No platform g_channel implementation after migration to FFI except
   * Player::onVideo callbacks for Texture. */

  /* Sets lambda for Player::onVideo callbacks. Called after creating new
   * instance of Player. */
  if (methodCall.method_name() == "createTexture") {
    flutter::EncodableMap arguments =
        std::get<flutter::EncodableMap>(*methodCall.arguments());
    int player_id =
        std::get<int>(arguments[flutter::EncodableValue("playerId")]);

    auto[it, added] =
        outlets.try_emplace(player_id, std::make_pair(0, nullptr));
    if (added) {
      auto player = g_players->Get(player_id);
      auto outlet = std::make_shared<VideoOutlet>(player->video_width(),
                                                  player->video_height());
      auto texture_id = textureRegistrar->RegisterTexture(outlet->texture());

      it->second = std::make_pair(texture_id, std::move(outlet));
      // TODO: The weak_ptr might not be needed anymore once callbacks can be
      // unregistered.
      player->OnVideo(
          [ =, weak_outlet = std::weak_ptr<VideoOutlet>(it->second.second) ](
              uint8_t * frame)
              ->void {
                if (auto outlet = weak_outlet.lock()) {
                  outlet->OnFrame(frame);
                  textureRegistrar->MarkTextureFrameAvailable(texture_id);
                }
              });

      textureRegistrar->MarkTextureFrameAvailable(texture_id);
      return result->Success(flutter::EncodableValue(texture_id));
    }

    result->Error("Texture was already registered.");
  }
  /* Called after disposing a Player instance. */
  else if (methodCall.method_name() == "disposeTexture") {
    flutter::EncodableMap arguments =
        std::get<flutter::EncodableMap>(*methodCall.arguments());
    int player_id =
        std::get<int>(arguments[flutter::EncodableValue("playerId")]);

    auto it = outlets.find(player_id);
    if (it != outlets.end()) {
      auto texture_id = it->second.first;
      textureRegistrar->UnregisterTexture(texture_id);
      outlets.erase(it);
    }

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
