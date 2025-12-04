cmake_minimum_required(VERSION 3.15)

project(DataStructure LANGUAGES C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE BOOL "Generate compile_command.json" FORCE)


add_subdirectory(data_structure)
# add_subdirectory(debug)

add_executable(Main src/main.c)
target_link_libraries(Main PRIVATE data_structure)

