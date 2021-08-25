#ifndef VIDEO_OUTLET_H_
#define VIDEO_OUTLET_H_

#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>
#include <flutter/standard_method_codec.h>

#include <mutex>

class VideoOutlet {
 public:
  VideoOutlet(int32_t player_id, flutter::TextureRegistrar* texture_registrar,
              flutter::MethodChannel<flutter::EncodableValue>* channel);

  int64_t texture_id() const { return texture_id_.value(); }

  void DeleteTexture();

  int64_t CreateNewTexture();

  void OnVideo(uint8_t* buffer, int32_t width, int32_t height);

  ~VideoOutlet();

 private:
  int32_t player_id_;
  FlutterDesktopPixelBuffer flutter_pixel_buffer_{};
  flutter::TextureRegistrar* texture_registrar_ = nullptr;
  flutter::MethodChannel<flutter::EncodableValue>* channel_;
  std::unique_ptr<flutter::TextureVariant> texture_ = nullptr;
  std::optional<int64_t> texture_id_ = std::nullopt;
  std::optional<int32_t> width_ = std::nullopt;
  std::optional<int32_t> height_ = std::nullopt;
  mutable std::mutex mutex_;
};

#endif
