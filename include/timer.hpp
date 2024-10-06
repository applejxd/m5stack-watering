#pragma once

#include <M5Stack.h>

namespace my_timer {

/**
 * 時刻を初期化
 */
void init_local_time() {
  Serial.println("Set NTP server.");

  constexpr long gmt_offset_sec = 3600L * 9;  // JST
  configTime(gmt_offset_sec, 0, "ntp.nict.jp", "time.google.com",
             "ntp.jst.mfeed.ad.jp");
}

bool get_current_time_info(tm& current_time_info) {
  if (!getLocalTime(&current_time_info)) {
    M5.Lcd.println("Cannot get current time info.");
    return false;
  }
  return true;
}

time_t get_timestamp() {
  tm current_time_info;
  if (get_current_time_info(current_time_info)) {
    return mktime(&current_time_info);
  }
  return 0;  // タイムスタンプが取得できなかった場合は0を返す
}

String time2string(const tm& time_info) {
  char now[30];
  snprintf(now, sizeof(now), "%02d:%02d:%02d", time_info.tm_hour,
           time_info.tm_min, time_info.tm_sec);
  return String(now);
}

void print_strctured_time(double time_interval) {
  const int days = static_cast<int>(time_interval / (24 * 60 * 60));
  const int hours = static_cast<int>(time_interval / (60 * 60) - 24 * days);
  const int minuites =
      static_cast<int>(time_interval / 60 - 60 * hours - 24 * 60 * days);
  const int seconds = static_cast<int>(time_interval - 60 * minuites -
                                       60 * 60 * hours - 24 * 60 * 60 * days);
  M5.Lcd.printf("%02d days %02d:%02d:%02d passed\n", days, hours, minuites,
                seconds);
}

class Timer {
 public:
  Timer() : _last_time(0) {}

  // 水やり時刻を保存
  String stamp() {
    tm time_info;
    if (get_current_time_info(time_info)) {
      _last_time = mktime(&time_info);
      return time2string(time_info);
    }
    return "Invalid Time";  // 無効な時間の場合
  }

  double get_passed_time() const {
    const time_t current_timestamp = get_timestamp();
    return difftime(current_timestamp, _last_time);
  }

 private:
  time_t _last_time;
};

}  // namespace my_timer