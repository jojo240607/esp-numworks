set(embedded_images)

idf_component_register(SRCS "i80_controller_example_main.c" ${embedded_images}
                       PRIV_REQUIRES esp_lcd spiffs esp_psram
                       INCLUDE_DIRS ".")

if(CONFIG_EXAMPLE_LCD_IMAGE_FROM_FILE_SYSTEM)
    # Create a partition to store the image resources in the filesystem
    spiffs_create_partition_image(storage ./images/filesystem FLASH_IN_PROJECT)
endif()
