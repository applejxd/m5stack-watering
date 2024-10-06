#pragma once

#include <M5Stack.h>

namespace gpio {

// センサー値取得、水分レベル変換
int adc2moisture(int adc) {
  // 水分レベルを計算し、安全な範囲 (0-5) に制約します
  if (adc >= 1650) {
    return constrain(map(adc, 1450, 1900, 2, 0), 0, 5);
  } else {
    return constrain(map(adc, 1350, 1450, 5, 3), 0, 5);
  }
}

double adc2volt(int adc) {
  constexpr double adc_max = 4095.;
  constexpr double volt_max = 3.3;
  double volt = (static_cast<double>(adc) + 1.0) * volt_max / (adc_max + 1.0);
  return volt;
}

class WateringUnit {
 public:
  WateringUnit() = default;

  void init_pin() {
    // watering モジュールのピン設定
    pinMode(_input_pin, INPUT);
    pinMode(_pump_pin, OUTPUT);
    pump_off();

    // スピーカ OFF & ノイズ対策
    // https://ghz2000.com/wordpress/?p=1418
    constexpr int speaker_pin = 25;
    pinMode(speaker_pin, OUTPUT);
    digitalWrite(speaker_pin, LOW);
  }

  // 10秒間ポンプを動かす
  void watering() {
    Serial.println("Watering.");
    digitalWrite(_pump_pin, HIGH);
    delay(10000);  // ポンプ作動中の待機
    digitalWrite(_pump_pin, LOW);
  }

  void pump_off() {
    Serial.println("Set pump pin as LOW.");
    digitalWrite(_pump_pin, LOW);
  }

  int get_adc() const { return analogRead(_input_pin); }

 private:
  // https://docs.m5stack.com/ja/unit/watering
  const int _input_pin = 36;
  const int _pump_pin = 26;
};

}  // namespace gpio