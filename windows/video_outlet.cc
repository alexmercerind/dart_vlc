#include "video_outlet.h"

VideoOutlet::VideoOutlet(
    int32_t player_id, flutter::TextureRegistrar* texture_registrar,
    flutter::MethodChannel<flutter::EncodableValue>* channel)
    : player_id_(player_id),
      texture_registrar_(texture_registrar),
      channel_(channel) {}

int64_t VideoOutlet::CreateNewTexture() {
  DeleteTexture();
  texture_ =
      std::make_unique<flutter::TextureVariant>(flutter::PixelBufferTexture(
          [=](size_t width, size_t height) -> const FlutterDesktopPixelBuffer* {
            const std::lock_guard<std::mutex> lock(mutex_);
            return &flutter_pixel_buffer_;
          }));
  texture_id_ = texture_registrar_->RegisterTexture(texture_.get());
  return texture_id_.value();
}

void VideoOutlet::DeleteTexture() {
  // Prevents calling `UnregisterTexture` if no texture was ever registered.
  if (texture_id_.has_value()) {
    channel_->InvokeMethod(
        "PlayerTextureId",
        std::make_unique<flutter::EncodableValue>(flutter::EncodableMap(
            {{"playerId", player_id_}, {"textureId", nullptr}})));
    texture_registrar_->UnregisterTexture(texture_id_.value());
    texture_id_ = std::nullopt;
  }
}

void VideoOutlet::OnVideo(uint8_t* buffer, int32_t width, int32_t height) {
  if (width_ != width && height_ != height) {
    channel_->InvokeMethod(
        "PlayerTextureId",
        std::make_unique<flutter::EncodableValue>(flutter::EncodableMap(
            {{"playerId", player_id_}, {"textureId", CreateNewTexture()}})));
    width_ = width;
    height_ = height;
  }
  const std::lock_guard<std::mutex> lock(mutex_);
  flutter_pixel_buffer_.buffer = buffer;
  flutter_pixel_buffer_.width = width;
  flutter_pixel_buffer_.height = height;
  texture_registrar_->MarkTextureFrameAvailable(texture_id_.value());
}

VideoOutlet::~VideoOutlet() { DeleteTexture(); }
