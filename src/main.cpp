/**
 * @file main.cpp
 * @author applejxd
 * @brief https://zenn.dev/tototo/articles/4fbf581b33e984
 * @version 0.1
 * @date 2024-09-28
 *
 * @copyright Copyright (c) 2024
 *
 */
#include <M5Stack.h>
// https://github.com/m5stack/M5Stack/issues/97
#undef min

#include "ambient.hpp"
#include "dlight.hpp"
#include "line.hpp"
#include "network.hpp"
#include "slack.hpp"
#include "spiffs.hpp"
#include "timer.hpp"
#include "watering.hpp"

network::AmbientSender ambient_sender;
my_timer::Timer timer;
gpio::WateringUnit watering_unit;
gpio::DLightUnit dlight_unit;

void setup() {
  M5.begin();
  M5.Power.begin();
  SPIFFS.begin();
  Serial.begin(19200);
  M5.Lcd.setTextSize(2);
  M5.Lcd.println("Initiating.");

  watering_unit.init_pin();
  delay(2000);

  dlight_unit.begin();
  dlight_unit.set_mode(gpio::CONTINUOUSLY_H_RESOLUTION_MODE);
  delay(2000);

  spiffs::list_spiffs_files();
  spiffs::load_config("/config.csv");
  network::connect_wifi();
  my_timer::init_local_time();
  timer.stamp();
  ambient_sender.connect();
}

void watering(const String& message) {
  // Slack に投稿
  String payload;
  const auto status_code = network::post_line(message, payload);

  M5.Lcd.printf("HTTP Status=%d\n", status_code);
  Serial.printf("POST method payload: %s\n", payload.c_str());

  // 10秒間ポンプを動かす
  watering_unit.watering();
  // 水やり時刻を保存
  timer.stamp();
}

void loop() {
  //-----------//
  // データ取得 //
  //-----------//

  // 湿度センサ値取得
  const int raw_adc = watering_unit.get_adc();
  const double volt = gpio::adc2volt(raw_adc);
  const int moisture_level = gpio::adc2moisture(raw_adc);
  // 経過時刻
  const double passed_time = timer.get_passed_time();
  // 照度
  uint16_t lux = dlight_unit.get_lux();

  //---------//
  // 画面描画 //
  //---------//

  // 画面クリア
  Serial.println("Draw display");
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0, 0, 2);
  my_timer::print_strctured_time(passed_time);
  M5.Lcd.printf("V=%.2f[V]\nmoisture=Lv.%d\nLUX=%u\n", volt, moisture_level,
                lux);

  //----------------//
  // センサデータ送信 //
  //----------------//

  // ADC の最小値・最大値を更新
  constexpr int adc_th = 2000;
  static int low_d = adc_th, high_d = 0;
  low_d = std::min(low_d, raw_adc);
  high_d = std::max(high_d, raw_adc);

  static int send_counter = 0;
  Serial.print("send_counter=");
  Serial.println(send_counter);
  if (++send_counter == 60 * 5) {
    // インターバルでのセンサ値を Ambient に送信
    Serial.println("Send to Ambient.");
    ambient_sender.send(
        {String(low_d), String(high_d), String(lux), String(moisture_level)});

    // 変数リセット
    low_d = adc_th;
    high_d = 0;
    send_counter = 0;
  }

  //-----------//
  // 水やり判定 //
  //-----------//

  String message = "Watering: " + String(volt) + "[V], moisture Lv." +
                   String(moisture_level) + ", " + String(lux) + "[lux]";

  if (moisture_level == 0 & (passed_time > 60. * 60. * 24. * 3.)) {
    M5.Lcd.println("3 days passed & dry soil");
    watering(message);
  } else if (passed_time > 60. * 60. * 24. * 10.) {
    M5.Lcd.println("10 days passed");
    watering(message);
  } else if (M5.BtnB.wasPressed()) {
    M5.Lcd.println("B button pressed");
    watering(message);
  }

  M5.update();
  delay(3000);  // 3[s]
}
