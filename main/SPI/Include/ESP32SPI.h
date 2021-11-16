#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "../../GlobalDefine.h"

/*
 The LCD needs a bunch of command/argument values to be initialized. They are stored in this struct.
*/
void ESP32_lcd_cmd(spi_device_handle_t spi, const uint8_t cmd);
void ESP32_lcd_data(spi_device_handle_t spi, const uint8_t *data, int len);

void ESP32_lcd_spi_pre_transfer_callback(spi_transaction_t *t);

void ESP32_send_lines(spi_device_handle_t spi, int ypos, uint16_t *linedata);
void ESP32_send_line_finish(spi_device_handle_t spi);




