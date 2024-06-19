#include "BMP390.h"
#include <iostream>
#include <pigpio.h>
#include <math.h>
#include "bmp3.h"

#define BMP3XX_DEFAULT_ADDRESS (0x77) 

BMP390::BMP390(void) {
  //_meas_end = 0;
  //_filterEnabled = _tempOSEnabled = _presOSEnabled = false;
  std::cout << "init";
}

int g_handler = -1;
// Our hardware interface functions

bool BMP390::begin_I2C(uint8_t addr){
  int handler = i2cOpen(1, addr, 0);
  g_handler = handler;
  if (handler == -1){
    std::cout << "Failed :(";
    return -1;
  }
  


  the_sensor.chip_id = addr;

  the_sensor.intf = BMP3_I2C_INTF;
  the_sensor.read = &i2c_read;
  the_sensor.write = &i2c_write;
  the_sensor.intf_ptr = &g_handler;
  the_sensor.dummy_byte = 0;

  return _init();
}

bool BMP390::_init(void) {
  the_sensor.delay_us = delay_usec;
  int8_t rslt = BMP3_OK;

  /* Reset the sensor */
  rslt = bmp3_soft_reset(&the_sensor);

   if (rslt == BMP3_OK) {
        printf("Sensor reset successful\n");
    } else {
        printf("Sensor reset failed with error code: %d\n", rslt);
    }
  rslt = bmp3_init(&the_sensor);


  rslt = validate_trimming_param(&the_sensor);

  if (rslt != BMP3_OK)
    return false;

  setTemperatureOversampling(BMP3_NO_OVERSAMPLING);
  setPressureOversampling(BMP3_NO_OVERSAMPLING);
  setIIRFilterCoeff(BMP3_IIR_FILTER_DISABLE);
  setOutputDataRate(BMP3_ODR_25_HZ);

  // don't do anything till we request a reading
  the_sensor.settings.op_mode = BMP3_MODE_FORCED;

  return true;
}

float BMP390::readTemperature(void) {
  performReading();
  return temperature;
}

uint8_t BMP390::chipID(void) { return the_sensor.chip_id; }

float BMP390::readPressure(void) {
  performReading();
  return pressure;
}

float BMP390::readAltitude(float seaLevel) {
  float atmospheric = readPressure() / 100.0F;
  return 44330.0 * (1.0 - pow(atmospheric / seaLevel, 0.1903));
}

bool BMP390::performReading(void) {
  int8_t rslt;
  /* Used to select the settings user needs to change */
  uint16_t settings_sel = 0;
  /* Variable used to select the sensor component */
  uint8_t sensor_comp = 0;

  /* Select the pressure and temperature sensor to be enabled */
  the_sensor.settings.temp_en = BMP3_ENABLE;
  settings_sel |= BMP3_SEL_TEMP_EN;
  sensor_comp |= BMP3_TEMP;
  if (_tempOSEnabled) {
    settings_sel |= BMP3_SEL_TEMP_OS;
  }

  the_sensor.settings.press_en = BMP3_ENABLE;
  settings_sel |= BMP3_SEL_PRESS_EN;
  sensor_comp |= BMP3_PRESS;
  if (_presOSEnabled) {
    settings_sel |= BMP3_SEL_PRESS_OS;
  }

  if (_filterEnabled) {
    settings_sel |= BMP3_SEL_IIR_FILTER;
  }

  if (_ODREnabled) {
    settings_sel |= BMP3_SEL_ODR;
  }

  // set interrupt to data ready
  // settings_sel |= BMP3_DRDY_EN_SEL | BMP3_LEVEL_SEL | BMP3_LATCH_SEL;

  /* Set the desired sensor configuration */
  rslt = bmp3_set_sensor_settings(settings_sel, &the_sensor);

  if (rslt != BMP3_OK)
    return false;

  /* Set the power mode */
  the_sensor.settings.op_mode = BMP3_MODE_FORCED;

  rslt = bmp3_set_op_mode(&the_sensor);
  if (rslt != BMP3_OK)
    return false;

  struct bmp3_data data;
  rslt = bmp3_get_sensor_data(sensor_comp, &data, &the_sensor);
  if (rslt != BMP3_OK)
    return false;

  temperature = data.temperature;
  pressure = data.pressure;

  return true;
}

bool BMP390::setTemperatureOversampling(uint8_t oversample) {
  if (oversample > BMP3_OVERSAMPLING_32X)
    return false;

  the_sensor.settings.odr_filter.temp_os = oversample;

  if (oversample == BMP3_NO_OVERSAMPLING)
    _tempOSEnabled = false;
  else
    _tempOSEnabled = true;

  return true;
}

bool BMP390::setPressureOversampling(uint8_t oversample) {
  if (oversample > BMP3_OVERSAMPLING_32X)
    return false;

  the_sensor.settings.odr_filter.press_os = oversample;

  if (oversample == BMP3_NO_OVERSAMPLING)
    _presOSEnabled = false;
  else
    _presOSEnabled = true;

  return true;
}

bool BMP390::setIIRFilterCoeff(uint8_t filtercoeff) {
  if (filtercoeff > BMP3_IIR_FILTER_COEFF_127)
    return false;

  the_sensor.settings.odr_filter.iir_filter = filtercoeff;

  if (filtercoeff == BMP3_IIR_FILTER_DISABLE)
    _filterEnabled = false;
  else
    _filterEnabled = true;

  return true;
}

bool BMP390::setOutputDataRate(uint8_t odr) {
  if (odr > BMP3_ODR_0_001_HZ)
    return false;

  the_sensor.settings.odr_filter.odr = odr;

  _ODREnabled = true;

  return true;
}

int8_t i2c_read(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *intf_ptr) {

  if (!i2c_write_then_read(reinterpret_cast<char*>(&reg_addr), 1, reinterpret_cast<char*>(reg_data), len)) {
    return 1;
  }

  return 0;
}

// Implementing the i2c_write_then_read function
bool i2c_write_then_read(char *write_buffer, size_t write_len, char *read_buffer, size_t read_len) {
  // Write to the device
  if (i2cWriteDevice(g_handler, write_buffer, write_len) != 0) {
    std::cerr << "I2C write failed" << std::endl;
    return false;
  }

  // Read from the device
  if (i2cReadDevice(g_handler, read_buffer, read_len) != read_len) {
    std::cerr << "I2C read failed" << std::endl;
    return false;
  }

  return true;
}

int8_t i2c_write(uint8_t reg_addr, const uint8_t *reg_data, uint32_t len, void *intf_ptr) {
  char write_buffer[len + 1];
  write_buffer[0] = reg_addr;
  std::copy(reg_data, reg_data + len, write_buffer + 1);

  if (i2cWriteDevice(g_handler, write_buffer, len + 1) != 0) {
    std::cerr << "I2C write failed" << std::endl;
    return 1;
  }
  return 0;
}


static void delay_usec(uint32_t us, void *intf_ptr) { gpioDelay(us); }

static int8_t validate_trimming_param(struct bmp3_dev *dev) {
  int8_t rslt;
  uint8_t crc = 0xFF;
  uint8_t stored_crc;
  uint8_t trim_param[21];
  uint8_t i;

  rslt = bmp3_get_regs(BMP3_REG_CALIB_DATA, trim_param, 21, dev);
  if (rslt == BMP3_OK) {
    for (i = 0; i < 21; i++) {
      crc = (uint8_t)cal_crc(crc, trim_param[i]);
    }

    crc = (crc ^ 0xFF);
    rslt = bmp3_get_regs(0x30, &stored_crc, 1, dev);
    if (stored_crc != crc) {
      rslt = -1;
    }
  }

  return rslt;
}

static int8_t cal_crc(uint8_t seed, uint8_t data) {
  int8_t poly = 0x1D;
  int8_t var2;
  uint8_t i;

  for (i = 0; i < 8; i++) {
    if ((seed & 0x80) ^ (data & 0x80)) {
      var2 = 1;
    } else {
      var2 = 0;
    }

    seed = (seed & 0x7F) << 1;
    data = (data & 0x7F) << 1;
    seed = seed ^ (uint8_t)(poly * var2);
  }

  return (int8_t)seed;
}