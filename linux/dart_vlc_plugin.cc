#include "include/dart_vlc/dart_vlc_plugin.h"
#include <flutter_linux/flutter_linux.h>
#include <gtk/gtk.h>
#include <cstring>
#define DART_VLC_PLUGIN(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), dart_vlc_plugin_get_type(), DartVlcPlugin))

struct _DartVlcPlugin {
    GObject parent_instance;
};

G_DEFINE_TYPE(DartVlcPlugin, dart_vlc_plugin, g_object_get_type())

FlMethodChannel* channel;


static void dart_vlc_plugin_handle_method_call(DartVlcPlugin* self, FlMethodCall* method_call) {
    FlMethodResponse* response = nullptr;
    /// No platform channel implementation after migration to FFI.
    response = FL_METHOD_RESPONSE(fl_method_not_implemented_response_new());
    fl_method_call_respond(method_call, response, nullptr);
}

static void dart_vlc_plugin_dispose(GObject* object) {
    G_OBJECT_CLASS(dart_vlc_plugin_parent_class)->dispose(object);
}

static void dart_vlc_plugin_class_init(DartVlcPluginClass* klass) {
    G_OBJECT_CLASS(klass)->dispose = dart_vlc_plugin_dispose;
}

static void dart_vlc_plugin_init(DartVlcPlugin* self) {}

static void method_call_cb(FlMethodChannel* channel, FlMethodCall* method_call, gpointer user_data) {
    DartVlcPlugin* plugin = DART_VLC_PLUGIN(user_data);
    dart_vlc_plugin_handle_method_call(plugin, method_call);
}

void dart_vlc_plugin_register_with_registrar(FlPluginRegistrar* registrar) {
    DartVlcPlugin* plugin = DART_VLC_PLUGIN(g_object_new(dart_vlc_plugin_get_type(), nullptr));
    g_autoptr(FlStandardMethodCodec) codec = fl_standard_method_codec_new();
    channel = fl_method_channel_new(fl_plugin_registrar_get_messenger(registrar), "dart_vlc", FL_METHOD_CODEC(codec));
    fl_method_channel_set_method_call_handler(channel, method_call_cb, g_object_ref(plugin), g_object_unref);
    g_object_unref(plugin);
}