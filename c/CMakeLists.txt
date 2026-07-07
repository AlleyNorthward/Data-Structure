cmake_minimum_required(VERSION 3.16)

project(C_DATA LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(data-structure)

option(BUILD_TESTS "build test program" OFF)
if(BUILD_TESTS)
  add_subdirectory(test)
else()
  add_executable(main main.c)
  target_link_libraries(main PRIVATE ds_lib)
endif()

