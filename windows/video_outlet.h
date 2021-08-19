#ifndef VIDEO_OUTLET_H_
#define VIDEO_OUTLET_H_

#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>
#include <flutter/standard_method_codec.h>

#include <mutex>

class VideoOutlet {
 public:
  VideoOutlet(flutter::TextureRegistrar* texture_registrar);

  int64_t texture_id() const { return texture_id_; }

  void OnVideo(uint8_t* buffer, int32_t width, int32_t height);

  ~VideoOutlet();

 private:
  FlutterDesktopPixelBuffer flutter_pixel_buffer_{};
  flutter::TextureRegistrar* texture_registrar_ = nullptr;
  std::unique_ptr<flutter::TextureVariant> texture_ = nullptr;
  int64_t texture_id_;
  mutable std::mutex mutex_;
};

#endif
