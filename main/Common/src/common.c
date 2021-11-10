#include "../include/common.h"


uint16_t swap_bytes(uint16_t color)
{
   color = (color & 0xFF00) >> 8 | (color & 0x00FF) << 8;
   color = (color & 0xF0F0) >> 4 | (color & 0x0F0F) << 4;
   color = (color & 0xCCCC) >> 2 | (color & 0x3333) << 2;
   color = (color & 0xAAAA) >> 1 | (color & 0x5555) << 1;
   return color;
}