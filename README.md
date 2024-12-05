# M5Stack 給水モジュール

## 機能

- デフォルト機能
  - 側面ボタン(1回)：電源 ON・再起動
  - 側面ボダン(2回)：電源 OFF
- 表示
  - N/A
- 前面ボタン(左)：何もしない
- 前面ボタン(中央)：強制給水（長押し？）
- 前面ボタン(右)：何もしない

## センサ構成

- [Watering モジュール](https://docs.m5stack.com/en/unit/watering)
  - PORT B インタフェース
  - Capacitive センサ (GPIO36)
  - Pump (GPIO26)

## 監視機能

- [LINE Notify](https://www.ekit-tech.com/?p=3434)
- [Ambient](https://ambidata.io/ch/channels.html)

## ビルド方法

1. VSCode を起動
2. PlatformIO IDE をインストール → サイドバーから選択
3. `./data/config.csv` に設定ファイル保存（最終行は空行にしておく）
4. `config.csv` の SPIFFS 書き込み1: `Platform` -> `Build Filesystem Image`
5. `config.csv` の SPIFFS 書き込み2: `Platform` -> `Upload Filesystem Image`
6. ビルド：`m5stack-core-esp32` → `General` → `Build`
7. 書き込み：`m5stack-core-esp32` → `General` → `Upload`
8. デバッグ：`m5stack-core-esp32` → `General` → `Monitor`
   - Serial.println 関数の出力を確認可能
