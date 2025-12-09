cmake_minimum_required(VERSION 3.20)
set(ENTRY "main")
project(${ENTRY})

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE BOOL "Generate compile_command.json" FORCE)

add_executable(
    ${ENTRY}
    main.c
)

add_subdirectory(data_structure)
target_include_directories(
    ${ENTRY}
    PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/data_structure/include"
)

target_link_libraries(
    ${ENTRY}
    ${data_structureLib}
)











