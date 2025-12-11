cmake_minimum_required(VERSION 3.20)
set(ENTRY "main")
project(${ENTRY})
option(BUILD_QUESTIONS "是否编译questions模块" OFF)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE BOOL "Generate compile_command.json" FORCE)


add_subdirectory(data_structure)

if(BUILD_QUESTIONS)
    add_subdirectory(questions/sqlist)
endif()

add_executable(
    ${ENTRY}
    main.c
)

if(BUILD_QUESTIONS)
    target_link_libraries(
        ${ENTRY}
        PRIVATE
        ${questions_sqlistLib}
    )
else()
    target_link_libraries(
        ${ENTRY}
        PRIVATE
        ${data_structureLib}
    )
endif()
