set(embedded_images)

idf_component_register(SRCS "i80_controller_example_main.cpp" ${embedded_images}
                       PRIV_REQUIRES
                            esp_lcd
                            spiffs
                            esp_psram
                            ion
                            omg
                            kandinsky
                            apps
                            escher
                            python
                            poincare
                       INCLUDE_DIRS "."
                            ${CMAKE_CURRENT_LIST_DIR}/../components
                            ${CMAKE_CURRENT_LIST_DIR}/../components/ion/include
                            ${CMAKE_CURRENT_LIST_DIR}/../components/ion/include/ion/keyboard/epsilon
                            ${CMAKE_CURRENT_LIST_DIR}/../components/ion/src/device
                            ${CMAKE_CURRENT_LIST_DIR}/../components/ion/src/device/include/n0120
                            ${CMAKE_CURRENT_SOURCE_DIR}/../components/omg/include
                            ${CMAKE_CURRENT_SOURCE_DIR}/../components/kandinsky/include
        )

if(CONFIG_EXAMPLE_LCD_IMAGE_FROM_FILE_SYSTEM)
    # Create a partition to store the image resources in the filesystem
    spiffs_create_partition_image(storage ./images/filesystem FLASH_IN_PROJECT)
endif()
