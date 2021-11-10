/* SPI Master example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"

#include "pretty_effect.h"

#include "./SPI/Include/ESP32SPI.h"
#include "./Display/include/ESP32ILI9341.h"
#include "./Common/include/common.h"

//Simple routine to generate some patterns and send them to the LCD. Don't expect anything too
//impressive. Because the SPI driver handles transactions in the background, we can calculate the next line
//while the previous one is being sent.
/*
static void display_pretty_colors(spi_device_handle_t spi)
{
    uint16_t *lines[2];
    //Allocate memory for the pixel buffers
    for (int i = 0; i < 2; i++)
    {
        lines[i] = heap_caps_malloc(320 * PARALLEL_LINES * sizeof(uint16_t) , MALLOC_CAP_DMA);
        assert(lines[i] != NULL);
    }
    int frame = 0;
    //Indexes of the line currently being sent to the LCD and the line we're calculating.
    int sending_line = -1;
    int calc_line = 0;

    while (1)
    {
        frame++;
        for (int y = 0; y < 240; y += PARALLEL_LINES)
        {
            //Calculate a line.
            pretty_effect_calc_lines(lines[calc_line], y, frame, PARALLEL_LINES);
            //Finish up the sending process of the previous line, if any
            if (sending_line != -1)
                ESP32_send_line_finish(spi);
            //Swap sending_line and calc_line
            sending_line = calc_line;
            calc_line = (calc_line == 1) ? 0 : 1;
            //Send the line we currently calculated.
            ESP32_send_lines(spi, y, lines[sending_line]);
            //The line set is queued up for sending now; the actual sending happens in the
            //background. We can go on to calculate the next line set as long as we do not
            //touch line[sending_line]; the SPI sending process is still reading from that.
        }
    }
}
*/

void app_main(void)
{
    esp_err_t ret;
    spi_device_handle_t spi;
    spi_bus_config_t buscfg = {
        .miso_io_num = PIN_NUM_MISO,
        .mosi_io_num = PIN_NUM_MOSI,
        .sclk_io_num = PIN_NUM_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = PARALLEL_LINES * 320 * 2 + 8};
    spi_device_interface_config_t devcfg = {
#ifdef CONFIG_LCD_OVERCLOCK
        .clock_speed_hz = 26 * 1000 * 1000, //Clock out at 26 MHz
#else
        .clock_speed_hz = 10 * 1000 * 1000, //Clock out at 10 MHz
#endif
        .mode = 0,                                     //SPI mode 0
        .spics_io_num = PIN_NUM_CS,                    //CS pin
        .queue_size = 7,                               //We want to be able to queue 7 transactions at a time
        .pre_cb = ESP32_lcd_spi_pre_transfer_callback, //Specify pre-transfer callback to handle D/C line
    };
    //Initialize the SPI bus
    ret = spi_bus_initialize(LCD_HOST, &buscfg, DMA_CHAN);
    ESP_ERROR_CHECK(ret);
    //Attach the LCD to the SPI bus
    ret = spi_bus_add_device(LCD_HOST, &devcfg, &spi);
    ESP_ERROR_CHECK(ret);
    //Initialize the LCD
    ESP32_ILI9341_lcd_init(spi);
    //Initialize the effect displayed
    //ret = pretty_effect_init();
    //ESP_ERROR_CHECK(ret);
    printf("test\n");
    //Go do nice stuff.
    ESP32_ILI9341_fillScreen(spi, swap_bytes(0x0000));
    vTaskDelay(1000 / portTICK_PERIOD_MS);

    ESP32_ILI9341_fillScreen(spi, swap_bytes(0xFFFF));
    vTaskDelay(1000 / portTICK_PERIOD_MS);

    printf("piros\n");
    ESP32_ILI9341_fillScreen(spi,swap_bytes(0xF800));
    vTaskDelay(1000 / portTICK_PERIOD_MS);

    printf("Sarga\n");
    ESP32_ILI9341_fillScreen(spi,swap_bytes(0xFE60));
    vTaskDelay(1000 / portTICK_PERIOD_MS);

    printf("kek\n");
    ESP32_ILI9341_fillScreen(spi,swap_bytes(0x081F));
    //display_pretty_colors(spi);
    while (1)
    {
        printf("Snake color 0xff!\n");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
