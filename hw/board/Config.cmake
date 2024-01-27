set(BOARD_TYPE "proto1" CACHE STRING "Board type")

if("${BOARD_TYPE}" STREQUAL "proto1")
    add_config_dependency("hw/escc")
    add_config_dependency("hw/ata")
    add_source("proto1.c")
endif()
