#include <windows.h>

#include <map>
#include <memory>
#include <sstream>

#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>
#include <flutter/standard_method_codec.h>

#include "include/FlutterVLC.h"


std::unique_ptr<flutter::MethodChannel<flutter::EncodableValue>> channel;


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

    void FlutterVlcPlugin::HandleMethodCall(const flutter::MethodCall<flutter::EncodableValue> &method_call, std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    if (method_call.method_name() == "getPlatformVersion") {
        result->Success(flutter::EncodableValue("Hello World!"));
    } else {
        result->NotImplemented();
    }
    }

}

void FlutterVlcPluginRegisterWithRegistrar(FlutterDesktopPluginRegistrarRef registrar) {
    FlutterVlcPlugin::RegisterWithRegistrar(flutter::PluginRegistrarManager::GetInstance()->GetRegistrar<flutter::PluginRegistrarWindows>(registrar));
}
