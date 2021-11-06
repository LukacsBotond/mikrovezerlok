#include "../include/ESP32ILI9341.h"

ESP32ILI9341::ESP32ILI9341(ISPI *spi)
{
    this->spi = spi;
    initialize();
}
ESP32ILI9341::~ESP32ILI9341()
{
    delete spi;
}

void ESP32ILI9341::set_command(const uint8_t cmd)
{
    this->spi->write_command(&cmd,1);
}
void ESP32ILI9341::command_param(const uint8_t *data, int lenght)
{
    this->spi->write_data(data, lenght);
}

void ESP32ILI9341::send_lines(int ypos, uint16_t *linedata){
    this->spi->send_lines(ypos,linedata);
}
void ESP32ILI9341::send_line_finish(){
    this->spi->send_line_finish();
}

void ESP32ILI9341::initialize()
{
    int cmd = 0;
    const lcd_init_cmd_t *lcd_init_cmds;

    //Initialize non-SPI GPIOs
    gpio_set_direction((gpio_num_t)PIN_NUM_DC, GPIO_MODE_OUTPUT);
    gpio_set_direction((gpio_num_t)PIN_NUM_RST, GPIO_MODE_OUTPUT);
    gpio_set_direction((gpio_num_t)PIN_NUM_BCKL, GPIO_MODE_OUTPUT);

    //Reset the display
    gpio_set_level((gpio_num_t)PIN_NUM_RST, 0);
    vTaskDelay(100 / portTICK_RATE_MS);
    gpio_set_level((gpio_num_t)PIN_NUM_RST, 1);
    vTaskDelay(100 / portTICK_RATE_MS);

    //detect LCD type
    //uint32_t lcd_id = lcd_get_id(spi);
    //int lcd_detected_type = 0;
    //int lcd_type;

    //printf("LCD ID: %08X\n", lcd_id);
    printf("LCD ILI9341 initialization.\n");
    lcd_init_cmds = ili_init_cmds;

    //Send all the commands
    while (lcd_init_cmds[cmd].databytes != 0xff)
    {

        set_command(lcd_init_cmds[cmd].cmd);
        command_param(lcd_init_cmds[cmd].data, lcd_init_cmds[cmd].databytes & 0x1F);
        if (lcd_init_cmds[cmd].databytes & 0x80)
        {
            vTaskDelay(100 / portTICK_RATE_MS);
        }
        cmd++;
    }

    ///Enable backlight
    gpio_set_level((gpio_num_t)PIN_NUM_BCKL, 0);
}
