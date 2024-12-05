#pragma once

#include <HTTPClient.h>
#include <M5Stack.h>

#include "spiffs.hpp"

namespace network {

int post_slack(const String &slack_message, String &payload) {
  HTTPClient http;

  // Slack APIのルーム情報を取得
  // https://www.moonmile.net/blog/archives/9229
  // https://zenn.dev/kou_pg_0131/articles/slack-api-post-message
  const String slack_info_room = spiffs::config["slack_info_room"];
  Serial.print("slack_info_room=");
  Serial.println(slack_info_room);

  // リクエストの初期化
  if (!http.begin(slack_info_room)) {
    Serial.println("Failed to initialize HTTP connection.");
    return -1;  // 初期化失敗
  }

  http.addHeader("Content-Type", "application/json");

  // POSTリクエスト送信
  int status_code = http.POST(slack_message);

  // エラー時に再試行する必要がない場合はこの行を削除
  if (status_code < 0) {
    Serial.println("First POST failed, retrying...");
    status_code = http.POST(slack_message);
  }

  payload = http.getString();
  http.end();

  return status_code;
}

}  // namespace network