/*!
 * @brief A Digital Ambient Light Sensor From M5Stack
 * @copyright Copyright (c) 2022 by M5Stack[https://m5stack.com]
 *
 * @Links [Unit DLight](https://docs.m5stack.com/en/unit/dlight)
 * @Links [HAT DLight](https://docs.m5stack.com/en/hat/hat_dlight)
 * @version  V0.0.3
 * @date  2022-07-27
 */
#ifndef _M5_DLIGHT_H_
#define _M5_DLIGHT_H_

#include "Arduino.h"
#include "Wire.h"
#include "pins_arduino.h"

namespace gpio {

constexpr uint8_t DEVICE_I2C_ADDR = 0x23;

// CMD
constexpr byte POWER_DOWN = B00000000;
constexpr byte POWER_ON = B00000001;
constexpr byte RESET = B00000111;
constexpr byte CONTINUOUSLY_H_RESOLUTION_MODE = B00010000;
constexpr byte CONTINUOUSLY_H_RESOLUTION_MODE2 = B00010001;
constexpr byte CONTINUOUSLY_L_RESOLUTION_MODE = B00010011;
constexpr byte ONE_TIME_H_RESOLUTION_MODE = B00100000;
constexpr byte ONE_TIME_H_RESOLUTION_MODE2 = B00100001;
constexpr byte ONE_TIME_L_RESOLUTION_MODE = B00100011;

class DLightUnit {
 public:
  DLightUnit(uint8_t addr = 0x23);
  void begin(TwoWire *wire = &Wire, uint8_t sda = SDA, uint8_t scl = SCL,
             uint32_t freq = 4000000UL);

  void power_on();
  void power_off();
  void power_reset();
  void set_mode(byte mode);
  uint16_t get_lux();

 private:
  TwoWire *_wire;
  uint8_t _sda;
  uint8_t _scl;
  uint32_t _freq;
  uint8_t _addr;
  void writeByte(byte cmd);
  void writeBytes(uint8_t *buffer, size_t size);
  void readBytes(uint8_t *buffer, size_t size);
};
}  // namespace gpio

#endif