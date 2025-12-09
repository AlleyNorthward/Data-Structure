set(data_structureLib "data_structurelib" CACHE STRING "数据结构库")

if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(IS_ROOT TRUE)
else()
    set(IS_ROOT FALSE)
endif()

if(IS_ROOT)
    cmake_minimum_required(VERSION 3.20)
    project(${data_structureLib})
    set(CMAKE_C_STANDARD 99)
    set(CMAKE_C_STANDARD_REQUIRED ON)
endif()

file(
    GLOB
    LIB1_SRC
    "src/*.c"
)

file(
    GLOB
    LIB1_INCLUDE
    "include/*.h"
)

add_library(
    ${data_structureLib}
    STATIC
    ${LIB1_SRC}
)

target_include_directories(
    ${data_structureLib}
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# ----------------------cut----------------------------------

if(IS_ROOT)
    install(
        TARGETS ${data_structureLib}
        EXPORT ${data_structureLib}Targets
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin
    )

    install(
#         DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include" 
        FILES ${LIB1_INCLUDE}
        DESTINATION "include"
    )

    install(
        EXPORT ${data_structureLib}Targets
        FILE ${data_structureLib}Config.cmake
        NAMESPACE ${data_structureLib}::
        DESTINATION lib/cmake
    )
endif()


