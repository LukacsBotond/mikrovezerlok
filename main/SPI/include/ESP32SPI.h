#pragma once
#include "./ISPI.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"

#define LCD_HOST HSPI_HOST
#define DMA_CHAN 2

#define PIN_NUM_MISO 19
#define PIN_NUM_MOSI 23
#define PIN_NUM_CLK 18
#define PIN_NUM_CS 5

#define PIN_NUM_DC 4
#define PIN_NUM_RST 15
#define PIN_NUM_BCKL 21

#define PARALLEL_LINES 8

class ESP32SPI : public ISPI
{
private:
    void lcd_spi_pre_transfer_callback(spi_transaction_t *t);
    spi_device_handle_t spi;

public:
    ESP32SPI();
    ~ESP32SPI(){}

    //not needed
    void changeFormat(bool dub) {}
    void cs_select() {}
    void cs_deselect() {}

    void write_data(const uint8_t *data, int len) override;
    void write_command(const uint8_t *data, int len) override;

    void send_lines(int ypos, uint16_t *linedata) override;
    void send_line_finish() override;
};
