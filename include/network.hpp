#pragma once

#include <M5Stack.h>
#include <WiFi.h>

#include "spiffs.hpp"

namespace network {

void connect_wifi() {
  const String& ssid = spiffs::config["ssid"];
  const String& pass = spiffs::config["wifi_pass"];
  Serial.printf("SSID: %s\n", ssid.c_str());
  Serial.printf("Password: %s\n", pass.c_str());

  M5.Lcd.print("Connecting to WiFi.");
  WiFi.begin(ssid.c_str(), pass.c_str());

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    M5.Lcd.print(".");
  }
  M5.Lcd.println("");

  // Check and display connection status
  if (WiFi.status() == WL_CONNECTED) {
    M5.Lcd.println("WiFi connected");
    M5.Lcd.printf("IP address=%s\n", WiFi.localIP().toString().c_str());
  } else {
    M5.Lcd.println("WiFi connection failed.");
  }
}

}  // namespace network