#include "dlight.hpp"

namespace gpio {

DLightUnit::DLightUnit(uint8_t addr) { _addr = addr; }

/*! @brief Initialize the EXTIO2.*/
void DLightUnit::begin(TwoWire *wire, uint8_t sda, uint8_t scl, uint32_t freq) {
  _wire = wire;
  _sda = sda;
  _scl = scl;
  _freq = freq;
  _wire->begin((int)_sda, (int)_scl, _freq);
  power_on();
}

/*! @brief Write data to the register address. */
void DLightUnit::writeByte(byte cmd) {
  _wire->beginTransmission(_addr);
  _wire->write(cmd);
  _wire->endTransmission();
}

/*! @brief Write the specified length of data to the register.*/
void DLightUnit::writeBytes(uint8_t *buffer, size_t size) {
  _wire->beginTransmission(_addr);
  _wire->write(buffer, size);
  _wire->endTransmission();
}

/*! @brief Read a certain length of data to the specified register address.*/
void DLightUnit::readBytes(uint8_t *buffer, size_t size) {
  // DLight モジュール未接続時に warning
  _wire->requestFrom(_addr, size);
  for (uint16_t i = 0; i < size; i++) {
    buffer[i] = _wire->read();
  }
}

/*! @brief Reading light intensity.
    @return Light intensity value read.. */
uint16_t DLightUnit::get_lux() {
  uint16_t lux;
  uint8_t buffer[2];
  readBytes(buffer, 2);
  lux = buffer[0] << 8 | buffer[1];
  return lux / 1.2;
}

/*! @brief Turn on the power.*/
void DLightUnit::power_on() { writeByte(POWER_ON); }

/*! @brief Turn off the power.*/
void DLightUnit::power_off() { writeByte(POWER_DOWN); }

void DLightUnit::power_reset() { writeByte(RESET); }

/*! @brief Set the mode.*/
void DLightUnit::set_mode(byte mode) { writeByte(mode); }

}  // namespace gpio
