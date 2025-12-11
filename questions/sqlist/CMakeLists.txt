set(questions_sqlistLib "questions_sqlistlib" CACHE STRING "问题/题目库")
set(file_ "src/a.c")

file(
    GLOB
    LIB1_SRC
    ${file_}
)

file(
    GLOB
    LIB1_INCLUDE
    "include/*.h"
)

add_library(
    ${questions_sqlistLib}
    STATIC
    ${LIB1_SRC}
)

target_include_directories(
    ${questions_sqlistLib}
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(
    ${questions_sqlistLib}
    PUBLIC
    ${data_structureLib}
)

