# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

# Find the vasm assembler.

if(CMAKE_SYSTEM_PROCESSOR MATCHES "^m68k.*")
  set(VASM_EXECUTABLE_NAME vasmm68k_mot)
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "^(powerpc.*|ppc.*)")
  set(VASM_EXECUTABLE_NAME vasmppc_std)
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "^6502.*")
  set(VASM_EXECUTABLE_NAME vasm6502_std)
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "^arm.*")
  set(VASM_EXECUTABLE_NAME vasmarm_std)
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "^z80.*")
  set(VASM_EXECUTABLE_NAME vasmz80_std)
else()
  message(FATAL_ERROR "Could not match vasm executable name to CMAKE_SYSTEM_PROCESSOR: '${CMAKE_SYSTEM_PROCESSOR}'")
endif()

find_program(VASM ${VASM_EXECUTABLE_NAME})

if(VASM)
  set(CMAKE_ASM_VASM_COMPILER_ENV_VAR ${CMAKE_ASM_COMPILER_ENV_VAR}" " CACHE STRING "" FORCE)
else()
  message(FATAL_ERROR "Could not find VASM executable: '${VASM_EXECUTABLE_NAME}'")
endif()
set(VASM_EXECUTABLE_NAME)

set(ASM_DIALECT "ASM_VASM")

configure_file(${CMAKE_CURRENT_LIST_DIR}/CMake${ASM_DIALECT}Compiler.cmake.in
               ${CMAKE_PLATFORM_INFO_DIR}/CMake${ASM_DIALECT}Compiler.cmake)
