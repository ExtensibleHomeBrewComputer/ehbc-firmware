cmake_minimum_required(VERSION 3.12)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
find_file(CMAKE_TOOLCHAIN_FILE
    EHBCToolchain.cmake
    HINTS ENV PATH
    PATH_SUFFIXES ../share/cmake/toolchains
    REQUIRED)

include(CheckSymbolExists)
include(CheckLanguage)
include(CMakeFindBinUtils)
include(ExternalProject)

include("${CMAKE_SOURCE_DIR}/cmake/ConfigDependency.cmake")

project("ehbc-firmware")

check_language(ASM)
if(${CMAKE_ASM_COMPILER_ID} NOT EQUAL "GNU")
    message(FATAL "GNU Assembler required.")
else()
    enable_language(ASM)
endif()

add_config_dependency("board")
add_config_dependency("src")
add_config_dependency("fs")

add_executable(firmware.elf ${CONFIGURED_SOURCES})
add_custom_target(firmware.bin ALL
    ${CMAKE_OBJCOPY}
        -I elf32-m68k
        -O binary
        --gap-fill 0x00
        --pad-to 0x100000
        $<TARGET_FILE:firmware.elf>
        firmware.bin
    DEPENDS firmware.elf
    COMMENT "Creating binary file")

target_compile_features(firmware.elf PUBLIC c_std_11)
target_compile_features(firmware.elf PUBLIC cxx_std_14)
target_compile_options(firmware.elf PUBLIC 
    $<$<COMPILE_LANGUAGE:C,CXX>:
        -Werror
        -Wall
        -Wno-unused-function
        -Wno-unused-variable
        -Wno-unused-but-set-variable
        -ffreestanding -nostdlib
        -pedantic
        -pedantic-errors>)

target_include_directories(firmware.elf PUBLIC "${CMAKE_SOURCE_DIR}/include")
target_include_directories(firmware.elf PRIVATE "${CMAKE_SOURCE_DIR}/")

target_link_options(firmware.elf PUBLIC -T "${CONFIGURED_LDSCRIPT}")
set_target_properties(firmware.elf PROPERTIES LINK_DEPENDS "${CONFIGURED_LDSCRIPT}")

message(STATUS "Configured Sources:")
foreach(SOURCE IN LISTS CONFIGURED_SOURCES)
    message(STATUS "- ${SOURCE}")
endforeach()

message(STATUS "Config Dependencies:")
foreach(CONFIG_DEPENDENCY IN LISTS CONFIG_DEPENDENCIES)
    message(STATUS "- ${CONFIG_DEPENDENCY}")
endforeach()
