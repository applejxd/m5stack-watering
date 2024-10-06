#pragma once

#include <M5Stack.h>
#include <WiFiClientSecure.h>

#include "spiffs.hpp"

namespace network {

int post_line(const String& message, String& payload) {
  // https://notify-bot.line.me/my/
  const String host = spiffs::config["line_host"];
  const String token = spiffs::config["line_token"];

  // リクエスト送信
  String query = "message=" + message;
  String request = String("") + "POST /api/notify HTTP/1.1\r\n" +
                   "Host: " + host.c_str() + "\r\n" + "Authorization: Bearer " +
                   token.c_str() + "\r\n" +
                   "Content-Length: " + String(query.length()) + "\r\n" +
                   "Content-Type: application/x-www-form-urlencoded\r\n\r\n" +
                   query + "\r\n";

  // HTTPSへアクセス（SSL通信）
  WiFiClientSecure client;
  client.setInsecure();
  Serial.println("Try Line Notify.");

  if (!client.connect(host.c_str(), 443)) {
    Serial.println("Line Notify connection failed.");
    return -1;  // 接続失敗
  }

  Serial.println("Line Notify connection success.");
  delay(2000);  // 通常は必要ないので削除検討

  int status = client.print(request);
  payload.clear();  // 初期化

  // レスポンスの受信
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") break;
    payload += line + '\n';
  }
  // 最終行を読み取る
  String last_line = client.readStringUntil('\n');
  payload += last_line;

  client.stop();

  return status;
};

}  // namespace network