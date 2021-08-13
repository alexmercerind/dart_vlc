import Cocoa
import FlutterMacOS

public class DartVlcPlugin: NSObject, FlutterPlugin {
  public static func register(with registrar: FlutterPluginRegistrar) {
    let channel = FlutterMethodChannel(name: "dart_vlc", binaryMessenger: registrar.messenger)
    let instance = DartVlcPlugin()
    registrar.addMethodCallDelegate(instance, channel: channel)
  }

  public func handle(_ call: FlutterMethodCall, result: @escaping FlutterResult) {
    switch call.method {
    case "getPlatformVersion":
      result("macOS " + ProcessInfo.processInfo.operatingSystemVersionString)
    case "setVlcPluginPath":
        if let path = call.arguments as? String {
            print("Setting VLC_PLUGIN_PATH as ")
            print(path)
            fflush(stdout)
            setenv("VLC_PLUGIN_PATH", path, 1)
            result(nil)
        } else {
            result(FlutterError.init(
                code: "Invalid arguments",
                message: "Can't get path from arguments",
                details: nil))
        }
    default:
      result(FlutterMethodNotImplemented)
    }
  }
}
