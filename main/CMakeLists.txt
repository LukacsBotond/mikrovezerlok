set(srcs "spi_master_example_main.cpp"
"SPI/src/ESP32SPI.cpp"
"Display/src/ESP32ILI9341.cpp"
    )

idf_component_register(SRCS ${srcs}
                    INCLUDE_DIRS "."
                    #EMBED_FILES image.jpg
)
