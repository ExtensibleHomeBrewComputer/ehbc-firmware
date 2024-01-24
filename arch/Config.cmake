set(BUILD_ARCH_M68K true CACHE BOOL "Build firmware for m68k architecture")

if(BUILD_ARCH_M68K)
    add_config_dependency("arch/m68k")
endif()
