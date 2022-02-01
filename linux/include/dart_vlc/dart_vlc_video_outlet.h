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

#ifndef VIDEO_OUTLET_H_
#define VIDEO_OUTLET_H_

#include <flutter_linux/flutter_linux.h>
#include <gtk/gtk.h>

#include "dart_vlc_plugin.h"

struct _VideoOutletClass {
  FlPixelBufferTextureClass parent_class;
};

struct VideoOutletPrivate {
  int64_t texture_id = 0;
  uint8_t* buffer = nullptr;
  int32_t video_width = 0;
  int32_t video_height = 0;
};

G_DECLARE_DERIVABLE_TYPE(VideoOutlet, video_outlet, DART_VLC, VIDEO_OUTLET,
                         FlPixelBufferTexture)

G_DEFINE_TYPE_WITH_CODE(VideoOutlet, video_outlet, fl_pixel_buffer_texture_get_type(), G_ADD_PRIVATE(VideoOutlet))

static gboolean video_outlet_copy_pixels(
    FlPixelBufferTexture* texture, const uint8_t** out_buffer, uint32_t* width,
    uint32_t* height, GError** error) {
  auto video_outlet_private = (VideoOutletPrivate*) video_outlet_get_instance_private(DART_VLC_VIDEO_OUTLET(texture));
  *out_buffer = video_outlet_private->buffer;
  *width = video_outlet_private->video_width;
  *height = video_outlet_private->video_height;
  return TRUE;
}

static VideoOutlet* video_outlet_new() {
  return DART_VLC_VIDEO_OUTLET(g_object_new(video_outlet_get_type(), nullptr));
}

static void video_outlet_class_init(VideoOutletClass* klass) {
  FL_PIXEL_BUFFER_TEXTURE_CLASS(klass)->copy_pixels = video_outlet_copy_pixels;
}

static void video_outlet_init(VideoOutlet* self) {}

#endif
