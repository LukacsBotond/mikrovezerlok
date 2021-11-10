#include "../include/ESP32ILI9341.h"

void ESP32_ILI9341_lcd_init(spi_device_handle_t spi)
{
    int cmd = 0;
    const lcd_init_cmd_t *lcd_init_cmds;
    line = heap_caps_malloc(lineSize, MALLOC_CAP_DMA);
    //Initialize non-SPI GPIOs
    gpio_set_direction(PIN_NUM_DC, GPIO_MODE_OUTPUT);
    gpio_set_direction(PIN_NUM_RST, GPIO_MODE_OUTPUT);
    gpio_set_direction(PIN_NUM_BCKL, GPIO_MODE_OUTPUT);

    //Reset the display
    gpio_set_level(PIN_NUM_RST, 0);
    vTaskDelay(100 / portTICK_RATE_MS);
    gpio_set_level(PIN_NUM_RST, 1);
    vTaskDelay(100 / portTICK_RATE_MS);
    printf("LCD ILI9341 initialization.\n");
    lcd_init_cmds = ili_init_cmds;

    //Send all the commands
    while (lcd_init_cmds[cmd].databytes != 0xff)
    {
        ESP32_lcd_cmd(spi, lcd_init_cmds[cmd].cmd);
        ESP32_lcd_data(spi, lcd_init_cmds[cmd].data, lcd_init_cmds[cmd].databytes & 0x1F);
        if (lcd_init_cmds[cmd].databytes & 0x80)
        {
            vTaskDelay(100 / portTICK_RATE_MS);
        }
        cmd++;
    }

    ///Enable backlight
    gpio_set_level(PIN_NUM_BCKL, 0);
}

void ESP32_ILI9341_fillScreen(spi_device_handle_t spi, uint16_t color)
{
    //memset(line,color,320 * PARALLEL_LINES * sizeof(uint16_t));
    //std::fill_n(line,320 * PARALLEL_LINES * sizeof(uint16_t),color);
    printf("%i\n", color);
    printf("%i\n", lineSize);
    for (int i = 0; i < lineSize; i++)
    {
        line[i] = color;
        //printf("%i ", line[i]);
    }

    printf("%i\n", line[0]);
    for (int i = 0; i < 240; i += PARALLEL_LINES)
    {
        ESP32_send_lines(spi, i, line);
        ESP32_send_line_finish(spi);
    }
}
