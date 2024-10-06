#pragma once

#include <Ambient.h>
#include <M5Stack.h>
#include <WiFi.h>

#include <cstdlib>
#include <vector>

#include "spiffs.hpp"

namespace {
unsigned int fetch_channel_id() {
  const char* channelIdStr = spiffs::config["ambient_channel_id"].c_str();
  // Default to 0 if not found
  return channelIdStr ? std::atoi(channelIdStr) : 0;
}

String fetch_write_key() {
  const char* writeKeyStr = spiffs::config["ambient_write_key"].c_str();
  // Default to empty string if not found
  return writeKeyStr ? String(writeKeyStr) : "";
}
}  // namespace

namespace network {

class AmbientSender {
 public:
  AmbientSender()
      : _channel_id(fetch_channel_id()), _write_key(fetch_write_key()) {}

  bool connect() {
    bool is_success =
        _ambient.begin(_channel_id, _write_key.c_str(), &_wclient);
    Serial.println(is_success ? "Ambient connection success."
                              : "Ambient connection failed.");
    return is_success;
  }

  bool send(const std::vector<String>& data) {
    for (size_t idx = 0; idx < data.size(); ++idx)
      if (!_ambient.set(static_cast<int>(idx) + 1, data[idx].c_str())) {
        Serial.printf("Ambient::set failed for index %zu: %s\n", idx,
                      data[idx].c_str());
        return false;
      }

    bool is_success = _ambient.send();
    Serial.println(is_success ? "Ambient messaging success."
                              : "Ambient messaging failed.");
    return is_success;
  };

 private:
  Ambient _ambient;
  // 通信のためにインスタンスの維持が必要
  WiFiClient _wclient;

  // https://ambidata.io/ch/channels.html
  const unsigned int _channel_id;
  const String _write_key;
};

}  // namespace network