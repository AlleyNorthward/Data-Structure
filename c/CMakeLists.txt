cmake_minimum_required(VERSION 3.16)

project(C_DATA LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB SOURCES
    "${PROJECT_SOURCE_DIR}/main.c"
    "${PROJECT_SOURCE_DIR}/src/*.c"
    "${PROJECT_SOURCE_DIR}/practice/*.c"
)

add_executable(main ${SOURCES})
