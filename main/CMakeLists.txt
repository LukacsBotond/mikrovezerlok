set(srcs 
"SPI/src/ESP32SPI.c"
"Display/src/ESP32ILI9341.c"
"Common/src/common.c"
"GameEngine/src/snake.c"
#"pretty_effect.c"
"snake_main.c"
)

# Only ESP32 has enough memory to do jpeg decoding
if(IDF_TARGET STREQUAL "esp32")
    list(APPEND srcs "decode_image.c")
endif()

idf_component_register(SRCS ${srcs}
                    INCLUDE_DIRS "."
                    EMBED_FILES image.jpg)
