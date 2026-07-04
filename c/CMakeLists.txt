cmake_minimum_required(VERSION 3.25)
project(C_DATA LANGUAGES C)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(
  GLOB_RECURSE
  LIB_SOURCES
  src/*.c
)

add_library(ds_lib STATIC ${LIB_SOURCES})

target_include_directories(
  ds_lib
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/list
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/list
)

add_executable(main main.c)
target_link_libraries(main PRIVATE ds_lib)
