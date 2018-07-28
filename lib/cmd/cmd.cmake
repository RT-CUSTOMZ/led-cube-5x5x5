#if(CONFIGURE_CLI_PARSER)

if(CONFIGURE_LCD)
  set(CMD_FLAGS ${CMD_FLAGS} " -D\ LCD")
endif(CONFIGURE_LCD)

 ADD_CUSTOM_COMMAND(
   OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/cparser_tree.h ${CMAKE_CURRENT_BINARY_DIR}/cparser_tree.c
   WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
   COMMAND python "${CMAKE_CURRENT_LIST_DIR}/../cli_parser/scripts/mk_parser.py" -o ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_LIST_DIR}/interface.cli
   DEPENDS ${CMAKE_CURRENT_LIST_DIR}/interface.cli ${CMAKE_CURRENT_LIST_DIR}/board.cli ${CMAKE_CURRENT_LIST_DIR}/fs.cli
   )

set(CMD_INCLUDE_DIRS
    "${CMAKE_CURRENT_LIST_DIR}"
)
include_directories(${CMD_INCLUDE_DIRS})
set(CMD_SRC
    ${CMAKE_CURRENT_LIST_DIR}/cparser_io_usb.c
    ${CMAKE_CURRENT_LIST_DIR}/interface_cli.c
    ${CMAKE_CURRENT_LIST_DIR}/framebuffer_cli.c
    ${CMAKE_CURRENT_LIST_DIR}/board_cli.c
    ${CMAKE_CURRENT_LIST_DIR}/cmd.c
    
    ${CMAKE_CURRENT_BINARY_DIR}/cparser_tree.c
    ${CMAKE_CURRENT_BINARY_DIR}/cparser_tree.h
)

if(CONFIGURE_USB_DEVICE_CDC)
    list(APPEND CMD_SRC
    ${CMAKE_CURRENT_LIST_DIR}/usbd_cdc_interface.c
    )
endif(CONFIGURE_USB_DEVICE_CDC)

if(CONFIGURE_LCD)
    list(APPEND CMD_SRC
        ${CMAKE_CURRENT_LIST_DIR}/lcd_cli.c
    )
endif(CONFIGURE_LCD)

if(CONFIGURE_FATFS)
    list(APPEND CMD_SRC
    ${CMAKE_CURRENT_LIST_DIR}/fs_cli.c
    )
endif(CONFIGURE_FATFS)
#endif(CONFIGURE_CLI_PARSER)