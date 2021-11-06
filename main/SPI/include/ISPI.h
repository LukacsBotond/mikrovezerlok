#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

class ISPI
{
public:
    virtual ~ISPI(){};
    virtual void changeFormat(bool dub) = 0;
    virtual void cs_select() = 0;
    virtual void cs_deselect() = 0;
    virtual void write_data(const uint8_t *data, int len);
    virtual void write_command(const uint8_t *data, int len);

    virtual void send_lines(int ypos, uint16_t *linedata) = 0;
    virtual void send_line_finish() = 0;
};
