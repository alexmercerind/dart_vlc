#include <string>
#include <map>

#include <windows.h>
#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>
#include <flutter/standard_method_codec.h>


class Method {
public:
    const flutter::MethodCall<flutter::EncodableValue>* methodCall;
    std::string name;
    std::map<flutter::EncodableValue, flutter::EncodableValue> arguments;
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result;

    Method(const flutter::MethodCall<flutter::EncodableValue>* methodCall, std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
        this->methodCall = methodCall;
        this->name = std::string(this->methodCall->method_name());
        this->arguments = std::get<flutter::EncodableMap>(*methodCall->arguments());
        this->result = std::move(result);
    }

    template<class T>
    void returnValue(T value);

    template <>
    void returnValue<int>(int value) {
        this->result->Success(
            flutter::EncodableValue(value)
        );
    }

    template <>
    void returnValue<std::string>(std::string value) {
        const char* resultCString = value.c_str();
        this->result->Success(
            flutter::EncodableValue(resultCString)
        );
    }

    template <>
    void returnValue<std::map<std::string, std::string>>(std::map<std::string, std::string> value) {
        auto map = flutter::EncodableMap();
        for (std::pair pair : value) {
            map[flutter::EncodableValue(pair.first)] = flutter::EncodableValue(pair.second);
        }
        this->result->Success(map);
    }

    void returnNull() {
        this->result->Success(
            flutter::EncodableValue(nullptr)
        );
    }

    void returnNotImplemented() {
        this->result->NotImplemented();
    }

    template<class T>
    T getArgument(const char* argument);

    template <>
    int getArgument<int>(const char* argument) {
        flutter::EncodableValue value = this->arguments[flutter::EncodableValue(argument)];
        return std::get<int>(value);
    }

    template <>
    std::string getArgument<std::string>(const char* argument) {
        flutter::EncodableValue value = this->arguments[flutter::EncodableValue(argument)];
        return std::get<std::string>(value);
    }

    template <>
    float getArgument<float>(const char* argument) {
        flutter::EncodableValue value = this->arguments[flutter::EncodableValue(argument)];
        return static_cast<float>(std::get<double>(value));
    }

    template <>
    std::map<std::string, std::string> getArgument<std::map<std::string, std::string>>(const char* argument) {
        std::map<std::string, std::string> map;
        flutter::EncodableMap value = std::get<flutter::EncodableMap>(this->arguments[flutter::EncodableValue(argument)]);
        for (const auto &pair: value) {
            map[std::get<std::string>(pair.first)] = std::get<std::string>(pair.second);
        }
        return map;
    }

    template <>
    std::vector<std::map<std::string, std::string>> getArgument<std::vector<std::map<std::string, std::string>>>(const char* argument) {
        std::vector<std::map<std::string, std::string>> vector;
        std::vector<flutter::EncodableValue> value = std::get<flutter::EncodableList>(this->arguments[flutter::EncodableValue(argument)]);
        for (flutter::EncodableValue element: value) {
            std::map<flutter::EncodableValue, flutter::EncodableValue> flutterMap = std::get<flutter::EncodableMap>(element);
            std::map<std::string, std::string> standardMap;
            for (std::pair pair : flutterMap) {
                standardMap[std::get<std::string>(pair.first)] = std::get<std::string>(pair.second);
            }
            vector.emplace_back(standardMap);
        }
        return vector;
    }

    void returnResult() {}
};


namespace FlutterTypes {
    template<class T>
    flutter::EncodableValue getValue(T value);

    template <>
    flutter::EncodableValue getValue<std::map<std::string, std::string>>(std::map<std::string, std::string> value) {
        auto flutterMap = flutter::EncodableMap();
        for (std::pair pair : value) {
            flutterMap[flutter::EncodableValue(pair.first)] = flutter::EncodableValue(pair.second);
        }
        return flutter::EncodableValue(flutterMap);
    }

    template <>
    flutter::EncodableValue getValue<std::vector<std::map<std::string, std::string>>>(std::vector<std::map<std::string, std::string>> value) {
        flutter::EncodableList flutterVector = flutter::EncodableList();
        for (std::map<std::string, std::string> map : value) {
            flutter::EncodableMap flutterMap = flutter::EncodableMap();
            for (std::pair pair: map) {
                flutterMap[flutter::EncodableValue(pair.first)] = flutter::EncodableValue(pair.second);
            }
            flutterVector.emplace_back(flutterMap);
        }
        return flutter::EncodableValue(flutterVector);
    }
}