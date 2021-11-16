set(srcs 
"SPI/src/ESP32SPI.c"
"Display/src/ESP32ILI9341.c"
"Common/src/common.c"
"GameEngine/src/snake.c"
#"pretty_effect.c"
"snake_main.c"
)

idf_component_register(SRCS ${srcs}
                    INCLUDE_DIRS ".")
