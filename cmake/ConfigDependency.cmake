set(CONFIG_DEPENDENCIES "")
set(CONFIGURED_SOURCES "")

macro(add_config_dependency dep_dir)
    if(NOT "${dep_dir}" IN_LIST CONFIG_DEPENDENCIES)
        list(APPEND CONFIG_DEPENDENCIES "${dep_dir}")
        include("${CMAKE_SOURCE_DIR}/${dep_dir}/Config.cmake")
    endif()
endmacro()

macro(add_source source_file)
    if(NOT "${CMAKE_CURRENT_LIST_DIR}/${source_file}" IN_LIST CONFIGURED_SOURCES)
        list(APPEND CONFIGURED_SOURCES "${CMAKE_CURRENT_LIST_DIR}/${source_file}")
    endif()
endmacro()


