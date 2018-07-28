include(${CMAKE_CURRENT_LIST_DIR}/../../evoOs2/boards/STM32F4xx-Nucleo/board.cmake)

set(CONFIGURE_FATFS ON)
set(CONFIGURE_SDIO ON)
#set(CONFIGURE_USB_DEVICE ON)
#set(CONFIGURE_USB_DEVICE_MSC ON)
#set(CONFIGURE_USB_DEVICE_CDC ON)
set(CONFIGURE_UART ON)

set(DEVICE WS2811)



include_directories("${CMAKE_CURRENT_LIST_DIR}")
list(APPEND BOARD_SRC
    ${CMAKE_CURRENT_LIST_DIR}/clock.c
    ${CMAKE_CURRENT_LIST_DIR}/board_sd.c
    ${CMAKE_CURRENT_LIST_DIR}/RGBImageMap.c
)


if(CONFIGURE_USB_DEVICE)
list(APPEND BOARD_SRC
    ${CMAKE_CURRENT_LIST_DIR}/stm32f4xx_hal_pcd_msp.c
    
)
endif(CONFIGURE_USB_DEVICE)

