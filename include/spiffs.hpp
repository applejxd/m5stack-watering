#pragma once

#include <CSV_Parser.h>
#include <M5Stack.h>
#include <SPIFFS.h>

#include <map>

namespace spiffs {

std::map<String, String> config;

/**
 * @brief List files in SPIFFS and print their names and sizes.
 * https://logikara.blog/spiffs-esp32/#mokuji_5-
 */
void list_spiffs_files() {
  File root = SPIFFS.open("/");
  while (File entry = root.openNextFile()) {
    Serial.printf("%s: %zu bytes\n", entry.name(), entry.size());
  }
}

/**
 * @brief Print the contents of a file given its name.
 *
 * @param file_name Name of the file to print.
 */
void print_file(const String& file_name) {
  if (SPIFFS.exists(file_name)) {
    File file = SPIFFS.open(file_name, FILE_READ);
    while (file.available()) {
      Serial.print((char)file.read());
    }
    file.close();
  }
}

/**
 * @brief Load configuration from a CSV file into a map.
 *
 * @param file_path Path to the CSV file.
 */
void load_config(const String& file_path) {
  if (!SPIFFS.exists(file_path)) {
    return;  // Early return if the file does not exist.
  }

  File fp = SPIFFS.open(file_path, FILE_READ);
  CSV_Parser cp("ss", false, ',');

  if (!fp) {
    M5.Lcd.println("File open error.");
    return;
  }

  while (fp.available()) {
    cp << (char)fp.read();
  }
  fp.close();

  Serial.printf("Read CSV %d lines.\n", cp.getRowsCount());

  char** col0 = (char**)cp[0];
  char** col1 = (char**)cp[1];

  for (int row = 0; row < cp.getRowsCount(); ++row) {
    String key = col0[row];
    String value = col1[row];
    config[key] = value;
    Serial.printf("%s: %s\n", key.c_str(), value.c_str());
  }
}

}  // namespace spiffs
