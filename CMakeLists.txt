cmake_minimum_required(VERSION 3.27)
project(raycc)

set(CMAKE_CXX_STANDARD 17)

if ($ENV{PWD} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "Prevent build in source directory")
endif ()

################################    MACROS    ################################

macro(project_add_inc)
    list(APPEND PROJECT_INCLUDES ${ARGV})
endmacro()

macro(project_add_src)
    list(APPEND PROJECT_SOURCES ${ARGV})
endmacro()

macro(project_add_inc_recursive)
    foreach (path ${ARGV})
        file(GLOB_RECURSE found_inc ${path} *.h)
        list(APPEND PROJECT_INCLUDES ${found_inc})
    endforeach ()
endmacro()

macro(project_add_src_recursive)
    foreach (path ${ARGV})
        file(GLOB_RECURSE found_src ${path} *.cc)
        list(APPEND PROJECT_SOURCES ${found_src})
    endforeach ()
endmacro()

################################    SOURCES    ################################

project_add_inc_recursive(include)
project_add_src_recursive(src/xcfg)

include_directories(xcfg "${CMAKE_CURRENT_LIST_DIR}/include")

################################    LIBRARY    ################################

add_library(xcfg STATIC
    ${PROJECT_INCLUDES}
    ${PROJECT_SOURCES}
)

################################    EXAMPLE    ################################

add_executable(xcfg_example
    src/example/example.cc
)

target_link_libraries(xcfg_example xcfg)
