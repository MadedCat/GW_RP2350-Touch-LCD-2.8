#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "inttypes.h"
#include "hardware/pio.h"

#define PIO_I2S pio1
#define I2S_DATA_PIN 4
#define I2S_CLK_BASE_PIN 2

void i2s_init();
void i2s_deinit();
void i2s_out(int16_t l_out,int16_t r_out);

#ifdef __cplusplus
}
#endif